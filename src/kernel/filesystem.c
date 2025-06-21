#include "../include/filesystem.h"
#include "../include/string.h"
#include "../include/screen.h"

// File system storage
static fs_file_t files[FS_MAX_FILES];
static fs_directory_t directories[FS_MAX_DIRS];
static uint32_t file_count = 0;
static uint32_t dir_count = 0;
static uint32_t current_dir = 0; // Root directory

// Initialize the filesystem
void fs_init() {
    // Clear all files and directories
    for (uint32_t i = 0; i < FS_MAX_FILES; i++) {
        files[i].name[0] = '\0';
        files[i].type = 0;
        files[i].size = 0;
    }
    
    for (uint32_t i = 0; i < FS_MAX_DIRS; i++) {
        directories[i].name[0] = '\0';
        directories[i].parent = 0;
    }
    
    // Create root directory
    strcpy(directories[0].name, "/");
    directories[0].parent = 0; // Root is its own parent
    dir_count = 1;
    
    // Set current directory to root
    current_dir = 0;
}

// Create a new file
int fs_create_file(const char* filename, uint32_t dir_index) {
    if (file_count >= FS_MAX_FILES) {
        return -1; // No space for new files
    }
    
    if (dir_index >= FS_MAX_DIRS || directories[dir_index].name[0] == '\0') {
        return -2; // Invalid directory
    }
    
    // Check if file already exists
    if (fs_find_file(filename, dir_index) != -1) {
        return -3; // File already exists
    }
    
    // Create the file
    strcpy(files[file_count].name, filename);
    files[file_count].type = FS_TYPE_FILE;
    files[file_count].size = 0;
    files[file_count].parent_dir = dir_index;
    
    return file_count++;
}

// Create a new directory
int fs_create_dir(const char* dirname, uint32_t parent_index) {
    if (dir_count >= FS_MAX_DIRS) {
        return -1; // No space for new directories
    }
    
    if (parent_index >= FS_MAX_DIRS || directories[parent_index].name[0] == '\0') {
        return -2; // Invalid parent directory
    }
    
    // Check if directory already exists
    if (fs_find_dir(dirname, parent_index) != -1) {
        return -3; // Directory already exists
    }
    
    // Create the directory
    strcpy(directories[dir_count].name, dirname);
    directories[dir_count].parent = parent_index;
    
    return dir_count++;
}

// Write data to a file
int fs_write_file(uint32_t file_index, const uint8_t* data, uint32_t size) {
    if (file_index >= FS_MAX_FILES || files[file_index].type != FS_TYPE_FILE) {
        return -1; // Invalid file
    }
    
    if (size > FS_MAX_FILESIZE) {
        return -2; // File too large
    }
    
    // Copy data to file
    for (uint32_t i = 0; i < size; i++) {
        files[file_index].data[i] = data[i];
    }
    
    files[file_index].size = size;
    return size;
}

// Read data from a file
int fs_read_file(uint32_t file_index, uint8_t* buffer) {
    if (file_index >= FS_MAX_FILES || files[file_index].type != FS_TYPE_FILE) {
        return -1; // Invalid file
    }
    
    // Copy file data to buffer
    for (uint32_t i = 0; i < files[file_index].size; i++) {
        buffer[i] = files[file_index].data[i];
    }
    
    return files[file_index].size;
}

// Delete a file
int fs_delete_file(uint32_t file_index) {
    if (file_index >= FS_MAX_FILES || files[file_index].type != FS_TYPE_FILE) {
        return -1; // Invalid file
    }
    
    // Mark file as deleted
    files[file_index].name[0] = '\0';
    files[file_index].type = 0;
    
    return 0;
}

// Delete a directory
int fs_delete_dir(uint32_t dir_index) {
    if (dir_index == 0) {
        return -1; // Can't delete root directory
    }
    
    if (dir_index >= FS_MAX_DIRS || directories[dir_index].name[0] == '\0') {
        return -2; // Invalid directory
    }
    
    // Check if directory is empty
    for (uint32_t i = 0; i < file_count; i++) {
        if (files[i].parent_dir == dir_index && files[i].type != 0) {
            return -3; // Directory not empty
        }
    }
    
    for (uint32_t i = 0; i < dir_count; i++) {
        if (directories[i].parent == dir_index && i != dir_index) {
            return -3; // Directory not empty
        }
    }
    
    // Mark directory as deleted
    directories[dir_index].name[0] = '\0';
    
    return 0;
}

// Find a file by name in a directory
int fs_find_file(const char* filename, uint32_t dir_index) {
    for (uint32_t i = 0; i < file_count; i++) {
        if (files[i].parent_dir == dir_index && 
            files[i].type == FS_TYPE_FILE && 
            strcmp(files[i].name, filename) == 0) {
            return i;
        }
    }
    
    return -1; // File not found
}

// Find a directory by name
int fs_find_dir(const char* dirname, uint32_t parent_index) {
    for (uint32_t i = 0; i < dir_count; i++) {
        if (directories[i].parent == parent_index && 
            strcmp(directories[i].name, dirname) == 0) {
            return i;
        }
    }
    
    return -1; // Directory not found
}

// List contents of a directory
void fs_list_dir(uint32_t dir_index) {
    if (dir_index >= FS_MAX_DIRS || directories[dir_index].name[0] == '\0') {
        print("Invalid directory\n");
        return;
    }
    
    // Print directories first
    for (uint32_t i = 0; i < dir_count; i++) {
        if (directories[i].parent == dir_index && i != dir_index) {
            print(directories[i].name);
            print("/\n");
        }
    }
    
    // Then print files
    for (uint32_t i = 0; i < file_count; i++) {
        if (files[i].parent_dir == dir_index && files[i].type == FS_TYPE_FILE) {
            print(files[i].name);
            print("\n");
        }
    }
}

// Get current directory index
uint32_t fs_get_current_dir() {
    return current_dir;
}

// Set current directory
void fs_set_current_dir(uint32_t dir_index) {
    if (dir_index < FS_MAX_DIRS && directories[dir_index].name[0] != '\0') {
        current_dir = dir_index;
    }
}

// Get current path as string
const char* fs_get_current_path() {
    static char path[256];
    path[0] = '\0';
    
    // Special case for root
    if (current_dir == 0) {
        path[0] = '/';
        path[1] = '\0';
        return path;
    }
    
    // Build path by traversing up to root
    uint32_t dir_indices[32];
    uint32_t depth = 0;
    uint32_t dir = current_dir;
    
    while (dir != 0) {
        dir_indices[depth++] = dir;
        dir = directories[dir].parent;
    }
    
    // Construct path string
    strcpy(path, "/");
    
    for (int i = depth - 1; i >= 0; i--) {
        strcat(path, directories[dir_indices[i]].name);
        if (i > 0) {
            strcat(path, "/");
        }
    }
    
    return path;
}
// Get parent directory of a directory
uint32_t fs_get_parent_dir(uint32_t dir_index) {
    if (dir_index < FS_MAX_DIRS && directories[dir_index].name[0] != '\0') {
        return directories[dir_index].parent;
    }
    return 0; // Default to root
}