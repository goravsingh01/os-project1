#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

#define FS_MAX_FILES 64
#define FS_MAX_FILENAME 32
#define FS_MAX_FILESIZE 4096
#define FS_MAX_DIRS 16
#define FS_MAX_DIRNAME 32

#define FS_TYPE_FILE 1
#define FS_TYPE_DIRECTORY 2

typedef struct {
    char name[FS_MAX_FILENAME];
    uint8_t type;
    uint32_t size;
    uint32_t parent_dir;  
    uint8_t data[FS_MAX_FILESIZE];
} fs_file_t;
typedef struct {
    char name[FS_MAX_DIRNAME];
    uint32_t parent;  
} fs_directory_t;

void fs_init();
int fs_create_file(const char* filename, uint32_t dir_index);
int fs_create_dir(const char* dirname, uint32_t parent_index);
int fs_write_file(uint32_t file_index, const uint8_t* data, uint32_t size);
int fs_read_file(uint32_t file_index, uint8_t* buffer);
int fs_delete_file(uint32_t file_index);
int fs_delete_dir(uint32_t dir_index);
int fs_find_file(const char* filename, uint32_t dir_index);
int fs_find_dir(const char* dirname, uint32_t parent_index);
void fs_list_dir(uint32_t dir_index);
uint32_t fs_get_current_dir();
void fs_set_current_dir(uint32_t dir_index);
const char* fs_get_current_path();
uint32_t fs_get_parent_dir(uint32_t dir_index);
#endif