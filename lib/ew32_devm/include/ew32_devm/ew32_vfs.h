#pragma once

#define    EW32FILE_O_EXCLUSIVE         24 // for open int mode 

#include "ew32_device.h"

#define ew32_vfs_open_ex(CTX, PATH, FLAGS)  ew32_vfs_fopen( (CTX), (PATH), (FLAGS), EW32FILE_O_EXCLUSIVE)
#define ew32_vfs_open(CTX, PATH, FLAGS)     ew32_vfs_fopen( (CTX), (PATH), (FLAGS), 0)
#define ew32_vfs_read(CTX, BUFFER, SIZE)    ew32_vfs_fread( (CTX), (BUFFER), (SIZE) )
#define ew32_vfs_write(CTX, BUFFER, SIZE)   ew32_vfs_fwrite( (CTX), (BUFFER), (SIZE) )
#define ew32_vfs_seek(CTX, SIZE, MODE)      ew32_vfs_fseek( (CTX), (SIZE), (MODE) ) 
#define ew32_vfs_close(CTX)                 ew32_vfs_fclose( (CTX) )

#ifdef __cplusplus
extern "C" {
#endif

// only call from ew_devm!!
int8_t ew32_vfsdb_getbyid(int32_t id, ew32_dev_t** device);
int32_t ew32_vfsdb_regist_device(const char* path, ew32_dev_t* device, uint8_t state);

uint8_t ew32_vfs_init();
uint8_t ew32_vfs_fopen(ew32_file_t* ctx, const char * path, int flags, int mode) ;
ssize_t ew32_vfs_fread (ew32_file_t* ctx, void * buffer, size_t size);
ssize_t ew32_vfs_fwrite (ew32_file_t* ctx, void * buffer, size_t size);
off_t ew32_vfs_fseek(ew32_file_t* ctx, off_t size, int mode);
uint8_t ew32_vfs_fclose(ew32_file_t* ctx);



#ifdef __cplusplus
}
#endif