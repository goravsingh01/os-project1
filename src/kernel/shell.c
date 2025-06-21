#include "../include/shell.h"
#include "../include/screen.h"
#include "../include/string.h"
#include "../include/filesystem.h"

// Maximum command arguments
#define MAX_ARGS 10

// Parse command into arguments
static int parse_command(const char* command, char args[MAX_ARGS][32]) {
    int arg_count = 0;
    int i = 0;
    int j = 0;
    
    while (command[i] != '\0' && arg_count < MAX_ARGS) {
        // Skip spaces
        while (command[i] == ' ') {
            i++;
        }
        
        // End of command
        if (command[i] == '\0') {
            break;
        }
        
        // Copy argument
        j = 0;
        while (command[i] != ' ' && command[i] != '\0' && j < 31) {
            args[arg_count][j++] = command[i++];
        }
        args[arg_count][j] = '\0';
        arg_count++;
    }
    
    return arg_count;
}

void shell_start() {
    print("Simple OS Shell v0.1\n");
    print("Type 'help' for available commands\n");
    print(fs_get_current_path());
    print("> ");
}

void shell_process_command(const char* command) {
    char args[MAX_ARGS][32];
    int arg_count = parse_command(command, args);
    
    if (arg_count == 0) {
        // Empty command, just show prompt
        print(fs_get_current_path());
        print("> ");
        return;
    }
    
    // Process commands
    if (strcmp(args[0], "help") == 0) {
        print("Available commands:\n");
        print("  help    - Display this help message\n");
        print("  clear   - Clear the screen\n");
        print("  echo    - Echo the arguments\n");
        print("  version - Display OS version\n");
        print("  ls      - List directory contents\n");
        print("  mkdir   - Create a directory\n");
        print("  cd      - Change directory\n");
        print("  pwd     - Print working directory\n");
        print("  touch   - Create an empty file\n");
        print("  cat     - Display file contents\n");
        print("  rm      - Remove a file or directory\n");
        print("  write   - Write content to a file\n");
    }
    else if (strcmp(args[0], "clear") == 0) {
        clear_screen();
    }
    else if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; i < arg_count; i++) {
            print(args[i]);
            print(" ");
        }
        print("\n");
    }
    else if (strcmp(args[0], "version") == 0) {
        print("Simple OS v0.1\n");
    }
    else if (strcmp(args[0], "ls") == 0) {
        fs_list_dir(fs_get_current_dir());
    }
    else if (strcmp(args[0], "mkdir") == 0) {
        if (arg_count < 2) {
            print("Usage: mkdir <dirname>\n");
        } else {
            int result = fs_create_dir(args[1], fs_get_current_dir());
            if (result < 0) {
                print("Error creating directory\n");
            }
        }
    }
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count < 2) {
            // Change to root directory
            fs_set_current_dir(0);
        } else {
            if (strcmp(args[1], "..") == 0) {
                // Go up one directory
                uint32_t current = fs_get_current_dir();
                if (current != 0) { // Not already at root
                    // Use fs_get_parent_dir function instead of accessing directories directly
                    fs_set_current_dir(fs_get_parent_dir(current));
                }
            } else {
                // Find and change to specified directory
                int dir_index = fs_find_dir(args[1], fs_get_current_dir());
                if (dir_index >= 0) {
                    fs_set_current_dir(dir_index);
                } else {
                    print("Directory not found: ");
                    print(args[1]);
                    print("\n");
                }
            }
        }
    }
    else if (strcmp(args[0], "pwd") == 0) {
        print(fs_get_current_path());
        print("\n");
    }
    else if (strcmp(args[0], "touch") == 0) {
        if (arg_count < 2) {
            print("Usage: touch <filename>\n");
        } else {
            int result = fs_create_file(args[1], fs_get_current_dir());
            if (result < 0) {
                print("Error creating file\n");
            }
        }
    }
    else if (strcmp(args[0], "cat") == 0) {
        if (arg_count < 2) {
            print("Usage: cat <filename>\n");
        } else {
            int file_index = fs_find_file(args[1], fs_get_current_dir());
            if (file_index >= 0) {
                uint8_t buffer[FS_MAX_FILESIZE + 1];
                int size = fs_read_file(file_index, buffer);
                if (size >= 0) {
                    buffer[size] = '\0'; // Null-terminate
                    print((char*)buffer);
                    print("\n");
                } else {
                    print("Error reading file\n");
                }
            } else {
                print("File not found: ");
                print(args[1]);
                print("\n");
            }
        }
    }
    else if (strcmp(args[0], "rm") == 0) {
        if (arg_count < 2) {
            print("Usage: rm <filename/dirname>\n");
        } else {
            // Try to delete as file first
            int file_index = fs_find_file(args[1], fs_get_current_dir());
            if (file_index >= 0) {
                int result = fs_delete_file(file_index);
                if (result < 0) {
                    print("Error deleting file\n");
                }
            } else {
                // Try to delete as directory
                int dir_index = fs_find_dir(args[1], fs_get_current_dir());
                if (dir_index >= 0) {
                    int result = fs_delete_dir(dir_index);
                    if (result < 0) {
                        print("Error deleting directory (may not be empty)\n");
                    }
                } else {
                    print("File or directory not found: ");
                    print(args[1]);
                    print("\n");
                }
            }
        }
    }
    else if (strcmp(args[0], "write") == 0) {
        if (arg_count < 3) {
            print("Usage: write <filename> <content>\n");
        } else {
            int file_index = fs_find_file(args[1], fs_get_current_dir());
            if (file_index >= 0) {
                // Combine all remaining arguments as content
                char content[FS_MAX_FILESIZE];
                content[0] = '\0';
                
                for (int i = 2; i < arg_count; i++) {
                    if (i > 2) strcat(content, " ");
                    strcat(content, args[i]);
                }
                
                int result = fs_write_file(file_index, (uint8_t*)content, strlen(content));
                if (result < 0) {
                    print("Error writing to file\n");
                }
            } else {
                print("File not found: ");
                print(args[1]);
                print("\n");
            }
        }
    }
    else if (command[0] != '\0') {
        print("Unknown command: ");
        print(args[0]);
        print("\n");
    }
    
    // Show prompt with current path
    print(fs_get_current_path());
    print("> ");
}
