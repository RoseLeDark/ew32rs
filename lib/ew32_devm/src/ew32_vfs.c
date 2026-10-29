#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/dirent.h>

#include "ew32_devm/ew32_api.h"
#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_vfs.h"
#include "ew32_devm/ew32_device.h"



struct ew32_vfs_db_regist {
    ew32_dev_t      *device;
    char            path[64];
    uint8_t         state;              // 0 = free, 1 = regist, 
};

static struct ew32_vfs_db_regist   __vfs_file_db[EW32_DEVICE_MANAGER_DEVICES];


int32_t __ew32_vfsdb_getidByPath(const char * path, uint8_t* canRead, uint8_t *canWrite) {
    ew32_file_t _ret = -1;

    for(ew32_file_t i = 0; i < EW32_DEVICE_MANAGER_DEVICES ; i++) {
        if(strcmp(__vfs_file_db[i].path, path) == 0) {
            _ret = i;

            if(canRead != 0) *canRead = ew32_device_can_read(__vfs_file_db[i].device);
            if(canWrite != 0) *canWrite = ew32_device_can_send(__vfs_file_db[i].device);

            break;
        }
    }
    return _ret;
}
int8_t ew32_vfsdb_getbyid(int32_t id, ew32_dev_t** device) {
    if(__vfs_file_db[id].state == 0) return ENODEV;

    if(device == NULL) return EIO;
    *device = __vfs_file_db[id].device;

    return 0;
}
int32_t ew32_vfsdb_regist_device(const char* path, ew32_dev_t* device, uint8_t state) {
    int32_t _ret = -1;
    
    for(int32_t i = 0; i < EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__vfs_file_db[i].state == 1) continue;

        _ret = i;

        __vfs_file_db[i].state = 1;
        __vfs_file_db[i].device = device;

        char* _p = (state == 1) ? EW32_VFS_DEV_PATH : EW32_VFS_SYS_PATH;     

        sniprintf(__vfs_file_db[i].path, sizeof(__vfs_file_db[i].path), "%s%c%s", _p, EW32_VFS_DELIMETER, path);

        break;

    }

    return _ret;
}

/// ============= DB_ENDE =============

struct ew32_file_fd {
    uint8_t         state;                  // 0 => unjused
    int16_t         open_count;             // Wie oft offen, jeder zugriff ++, bei close -- == 0 dann richtig close
    int32_t         dev_index;
    
    union  {
        struct  {
            uint8_t    read : 1;
            uint8_t    write : 1;
            uint8_t    is_exclusive : 1;
            uint8_t    reserv;
            uint8_t    id : 4;
        };
        uint8_t        f;
    };
    ew32_semaphore_t lock;         // Optional: Plattform-unabhängiges Mutex/Semaphor für den Zugriff
};

static struct ew32_file_fd         __fd_table[EW32_MAX_OPEN_FILES];
static ew32_semaphore_t __ew32_lock_open;

uint8_t ew32_vfs_init()  {
    static uint8_t _created = 0;
    if(_created == 1) return 0;
    _created = 1;
    ew32_semaphore_create(EW32_SEMAPHORE_MUTEX, &__ew32_lock_open);
    return 0;
}
uint8_t __ew32_vfs_cheakother_exclusiv(ew32_file_t me) {
    uint8_t _ret = 0;

    for(ew32_file_t i = 0; i < EW32_MAX_OPEN_FILES; i++) {
        if(i == me) continue;

        if( __fd_table[i].is_exclusive == 1) {
            _ret = 1;
            break;
        }
    }
    return _ret;
}
ew32_file_t _ew32_vfs_get_entry() {
    ew32_file_t _ret = -1;

    for(ew32_file_t i = 0; i < EW32_MAX_OPEN_FILES; i++) {
        if( __fd_table[i].state == 0) {
            __fd_table[i].state = 1;
            _ret = i;
            break;
        }
    }

    return _ret;
}
uint8_t ew32_vfs_fopen(ew32_file_t* ctx, const char * path, int flags, int mode) {
    if(ctx == 0) return EIO;

    ew32_file_t _newfd = -1;
    uint8_t canRead = 0;
    uint8_t canWrite = 0;
    uint8_t _ret = 1;

    
    ew32_semaphore_lock(__ew32_lock_open, EW32_INFINITY);

    do
    {   
        int32_t _db_entry = __ew32_vfsdb_getidByPath(path, &canRead, &canWrite);
        if (_db_entry == -1) { _ret = ENXIO; break; }  // not a device


        if (flags &  O_RDONLY && canRead == 0) { _ret = EINVAL; break; } // O_RDONLY
        if (flags &  O_WRONLY && canWrite == 0){ _ret = EINVAL; break; }  // O_WRONLY
        if (flags &  O_RDWR && (!canRead || !canWrite)) { _ret = EINVAL; break; }  // O_RDWR
        if (flags & O_CREAT) { _ret = EPERM; break; } 

        _newfd = _ew32_vfs_get_entry();
        if(_newfd == -1)  { _ret = ENFILE; break; } 

        if(mode == EW32FILE_O_EXCLUSIVE && __ew32_vfs_cheakother_exclusiv(_newfd) == 1) {
            __fd_table[_newfd].state = 0;
            _ret = EBUSY; break; // Already exclusive
        } 

        ew32_semaphore_create(EW32_SEMAPHORE_MUTEX, &__fd_table[_newfd].lock);

        __fd_table[_newfd].state = 1;
        __fd_table[_newfd].open_count = 1;
        __fd_table[_newfd].dev_index = _db_entry;
        __fd_table[_newfd].is_exclusive = (mode == EW32FILE_O_EXCLUSIVE) ? 1 : 0;
        __fd_table[_newfd].read = canRead ? 1 : 0;
        __fd_table[_newfd].write = canWrite ? 1 : 0;

        *ctx = _newfd;
        _ret = 0;

        break;
    } while (0);

    ew32_semaphore_unlock(__ew32_lock_open);

    return _ret;
}
ssize_t ew32_vfs_fread (ew32_file_t* ctx, void * dst, size_t size) {
    if(ctx == 0) return -EIO;

    
    uint8_t _ret = 1;
    ew32_file_t _fd = *ctx;

    ew32_semaphore_lock(__fd_table[_fd].lock, EW32_INFINITY);

    while (1)
    {
        if(__fd_table[_fd].open_count == 0 || __fd_table[_fd].state != 1) 
            { _ret = -EPROTO; break; }

        if(__fd_table[_fd].is_exclusive == 0 && __ew32_vfs_cheakother_exclusiv(_fd) == 1) 
            { _ret = -EBUSY; break; }

        ew32_dev_t* _dev = 0;
        uint8_t _reet = ew32_vfsdb_getbyid(__fd_table[_fd].dev_index, &_dev);
        if(_reet != 0 || _dev == 0) { _ret = -ENODEV; break; }

        _ret = ew32_device_read(_dev, dst, size);
        break;
    };

    ew32_semaphore_unlock(__fd_table[_fd].lock);

    return _ret;
    
}

ssize_t ew32_vfs_fwrite (ew32_file_t* ctx, void * dst, size_t size) {
    if(ctx == 0) return -EIO;

    
    uint8_t _ret = 1;
    ew32_file_t _fd = *ctx;


    ew32_semaphore_lock(__fd_table[_fd].lock, EW32_INFINITY);

    while (1)
    {
        if(__fd_table[_fd].open_count == 0 || __fd_table[_fd].state != 1) 
            { _ret = -EPROTO; break; }

        if(__fd_table[_fd].is_exclusive == 0 && __ew32_vfs_cheakother_exclusiv(_fd) == 1) 
            { _ret = -EBUSY; break; }

        ew32_dev_t* _dev = 0;
        uint8_t _reet = ew32_vfsdb_getbyid(__fd_table[_fd].dev_index, &_dev);
        if(_reet != 0 || _dev == 0) { _ret = -ENODEV; break; }

        _ret = ew32_device_send(_dev, dst, size);
        break;
    };

    ew32_semaphore_unlock(__fd_table[_fd].lock);

    return _ret;
}
off_t ew32_vfs_fseek(ew32_file_t* ctx, off_t size, int mode) {
    return -ENOSYS;
}
uint8_t ew32_vfs_fclose(ew32_file_t* ctx) {
    if(ctx == 0) return -EIO;
    uint8_t _ret = 1;

    ew32_file_t _fd = *ctx;

    while (1)
    {
        if(__fd_table[_fd].state != 1) { _ret = EPROTO; break; }

        ew32_semaphore_lock(__fd_table[_fd].lock, EW32_INFINITY);
        __fd_table[_fd].open_count--;
        ew32_semaphore_unlock(__fd_table[_fd].lock);
        
        if(__fd_table[_fd].open_count == 0) {
            ew32_semaphore_lock(__ew32_lock_open, EW32_INFINITY);
            __fd_table[_fd].state = 0;
            __fd_table[_fd].is_exclusive = 0;
            __fd_table[_fd].dev_index = -1;
            ew32_semaphore_destroy_ex(__fd_table[_fd].lock,1);
            ew32_semaphore_unlock(__ew32_lock_open);
        }
        
    }
 
    return 0;
}