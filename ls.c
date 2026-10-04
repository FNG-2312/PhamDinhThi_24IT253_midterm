#include "ls.h"

// Global variables for the qsort comparison function
int sort_reverse = 0;
int sort_time = 0;
int sort_size = 0;
int sort_time_type = 0;

// Replace non-printable characters with '?' if mode == 1
void print_safe_name(const char *name, int mode) {
    int i;
    if (mode == 1) {
        for (i = 0; name[i] != '\0'; i++) {
            if ((unsigned char)name[i] >= 32 && (unsigned char)name[i] < 127) {
                putchar(name[i]);
            } else {
                putchar('?');
            }
        }
    } else {
        printf("%s", name);
    }
}

// Convert stat mode bits into a rwxrwxrwx permission string
void mode_to_letters(int mode, char str[]) {
    strcpy(str, "----------");
    
    // Determine file type
    if (S_ISDIR(mode))  str[0] = 'd';
    if (S_ISCHR(mode))  str[0] = 'c';
    if (S_ISBLK(mode))  str[0] = 'b';
    if (S_ISLNK(mode))  str[0] = 'l';
    if (S_ISFIFO(mode)) str[0] = 'p';
    if (S_ISSOCK(mode)) str[0] = 's';

    // Owner permissions
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_ISUID) str[3] = (mode & S_IXUSR) ? 's' : 'S';

    // Group permissions
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_ISGID) str[6] = (mode & S_IXGRP) ? 's' : 'S';

    // Other permissions
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
    if (mode & S_ISVTX) str[9] = (mode & S_IXOTH) ? 't' : 'T';
}

// Format file size into human-readable format (e.g., 1.5K, 2M)
void format_human_size(long size, char *buf) {
    const char *units[] = {"B", "K", "M", "G"};
    int i = 0;
    double calc_size = size;

    while (calc_size >= 1024 && i < 3) {
        calc_size /= 1024;
        i++;
    }
    
    if (i == 0) {
        sprintf(buf, "%4ldB", size);
    } else {
        sprintf(buf, "%4.1f%s", calc_size, units[i]);
    }
}

// Return special character indicator for -F flag
char get_type_indicator(int mode) {
    if (S_ISDIR(mode)) return '/';
    if (S_ISLNK(mode)) return '@';
    if (S_ISSOCK(mode)) return '=';
    if (S_ISFIFO(mode)) return '|';
    if (mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH) return '*';
    return '\0';
}

// Print detailed file information for long format (-l)
void show_file_info(const char *filename, struct stat *info_p, LsOptions opts) {
    char modestr[11];
    struct passwd *pw_ptr;
    struct group *grp_ptr;
    char datestr[256];
    struct tm *tm_info;
    char sizebuf[32];
    time_t target_time;

    // Print permissions and links
    mode_to_letters(info_p->st_mode, modestr);
    printf("%s ", modestr);
    printf("%4d ", (int)info_p->st_nlink);

    // Print owner and group (numeric or name)
    if (opts.numeric_id) {
        printf("%-8d %-8d ", info_p->st_uid, info_p->st_gid);
    } else {
        if ((pw_ptr = getpwuid(info_p->st_uid)) != NULL) {
            printf("%-8s ", pw_ptr->pw_name);
        } else {
            printf("%-8d ", info_p->st_uid);
        }

        if ((grp_ptr = getgrgid(info_p->st_gid)) != NULL) {
            printf("%-8s ", grp_ptr->gr_name);
        } else {
            printf("%-8d ", info_p->st_gid);
        }
    }

    // Print size
    if (opts.show_human) {
        format_human_size((long)info_p->st_size, sizebuf);
        printf("%8s ", sizebuf);
    } else {
        printf("%8ld ", (long)info_p->st_size);
    }

    // Determine which time to display
    if (opts.time_type == 1) {
        target_time = info_p->st_atime;
    } else if (opts.time_type == 2) {
        target_time = info_p->st_ctime;
    } else {
        target_time = info_p->st_mtime;
    }

    // Format and print time
    tm_info = localtime(&target_time);
    strftime(datestr, sizeof(datestr), "%b %d %H:%M", tm_info);
    printf("%s ", datestr);
    
    // Print filename and indicator
    print_safe_name(filename, opts.non_printable_mode);
    if (opts.show_type) {
        char indicator = get_type_indicator(info_p->st_mode);
        if (indicator) printf("%c", indicator);
    }
    printf("\n");
}

// Comparison function for qsort
int compare_entries(const void *a, const void *b) {
    FileEntry *entry_a = (FileEntry *)a;
    FileEntry *entry_b = (FileEntry *)b;
    int result = 0;
    time_t time_a, time_b;

    // Select the correct time variable based on options
    if (sort_time_type == 1) {
        time_a = entry_a->info.st_atime;
        time_b = entry_b->info.st_atime;
    } else if (sort_time_type == 2) {
        time_a = entry_a->info.st_ctime;
        time_b = entry_b->info.st_ctime;
    } else {
        time_a = entry_a->info.st_mtime;
        time_b = entry_b->info.st_mtime;
    }

    // Sort by size, time, or lexicographically
    if (sort_size) {
        if (entry_a->info.st_size < entry_b->info.st_size) result = 1;
        else if (entry_a->info.st_size > entry_b->info.st_size) result = -1;
        else result = strcmp(entry_a->name, entry_b->name);
    } else if (sort_time) {
        if (time_a < time_b) result = 1;
        else if (time_a > time_b) result = -1;
        else result = strcmp(entry_a->name, entry_b->name);
    } else {
        result = strcmp(entry_a->name, entry_b->name);
    }

    // Apply reverse sort if requested
    if (sort_reverse) {
        return -result;
    }
    return result;
}

// Core function to list directory contents
void do_ls(const char *dir_name, LsOptions opts) {
    DIR *dir_ptr;
    struct dirent *direntp;
    FileEntry entries[1024];
    int count = 0;
    int i;
    char *dirs_to_visit[1024];
    int dir_count = 0;
    struct stat target_stat;

    // Synchronize global sorting variables
    sort_reverse = opts.reverse;
    sort_time = opts.sort_time;
    sort_size = opts.sort_size;
    sort_time_type = opts.time_type;

    // 1. Check if target exists and if it is a directory or a regular file
    if (stat(dir_name, &target_stat) == -1) {
        fprintf(stderr, "myls: cannot access '%s'\n", dir_name);
        return;
    }

    // 2. If it is NOT a directory, OR if the -d flag is used, treat it as a single file
    if (opts.list_dir_as_file || !S_ISDIR(target_stat.st_mode)) {
        if (opts.show_inode) {
            printf("%llu ", (unsigned long long)target_stat.st_ino);
        }
        if (opts.show_blocks) {
            long long b = target_stat.st_blocks;
            if (opts.kilobytes_blocks) b = (b + 1) / 2;
            printf("%llu ", (unsigned long long)b);
        }
        if (opts.show_long) {
            show_file_info(dir_name, &target_stat, opts);
        } else {
            print_safe_name(dir_name, opts.non_printable_mode);
            if (opts.show_type) {
                char indicator = get_type_indicator(target_stat.st_mode);
                if (indicator) printf("%c", indicator);
            }
            printf("\n");
        }
        return;
    }

    // 3. If it is a directory, open it
    if ((dir_ptr = opendir(dir_name)) == NULL) {
        fprintf(stderr, "myls: cannot open directory '%s'\n", dir_name);
        return; // Edge case: permission denied
    }

    // Read directory entries
    while ((direntp = readdir(dir_ptr)) != NULL) {
        // Handle hidden files based on -a and -A flags
        if (direntp->d_name[0] == '.') {
            if (!opts.show_all && !opts.show_almost_all) {
                continue;
            }
            if (opts.show_almost_all && !opts.show_all) {
                if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {
                    continue;
                }
            }
        }
        
        // Store entry data
        entries[count].name = strdup(direntp->d_name);
        entries[count].path = malloc(strlen(dir_name) + strlen(direntp->d_name) + 2);
        sprintf(entries[count].path, "%s/%s", dir_name, direntp->d_name);
        
        if (stat(entries[count].path, &entries[count].info) == -1) {
            free(entries[count].name);
            free(entries[count].path);
            continue;
        }
        count++;
    }
    closedir(dir_ptr);

    // Sort entries unless -f is specified
    if (!opts.disable_sort) {
        qsort(entries, count, sizeof(FileEntry), compare_entries);
    }

    // Calculate and print total block size if needed
    long long total_blocks = 0;
    if (opts.show_long || opts.show_blocks) {
        for (i = 0; i < count; i++) {
            long long b = entries[i].info.st_blocks;
            if (opts.kilobytes_blocks) {
                b = (b + 1) / 2;
            }
            total_blocks += b;
        }
        if (opts.show_long && count > 0) {
            printf("total %llu\n", (unsigned long long)total_blocks);
        }
    }

    // Output formatting and preparation for recursion
    for (i = 0; i < count; i++) {
        if (opts.show_inode) {
            printf("%llu ", (unsigned long long)entries[i].info.st_ino);
        }
        if (opts.show_blocks) {
            long long b = entries[i].info.st_blocks;
            if (opts.kilobytes_blocks) b = (b + 1) / 2;
            printf("%llu ", (unsigned long long)b);
        }
        if (opts.show_long) {
            show_file_info(entries[i].name, &entries[i].info, opts);
        } else {
            print_safe_name(entries[i].name, opts.non_printable_mode);
            if (opts.show_type) {
                char indicator = get_type_indicator(entries[i].info.st_mode);
                if (indicator) printf("%c", indicator);
            }
            printf("  ");
        }

        // Check if entry is a directory for recursive listing
        if (opts.recursive && S_ISDIR(entries[i].info.st_mode)) {
            if (strcmp(entries[i].name, ".") != 0 && strcmp(entries[i].name, "..") != 0) {
                dirs_to_visit[dir_count] = strdup(entries[i].path);
                dir_count++;
            }
        }
        
        // Free allocated memory
        free(entries[i].name);
        free(entries[i].path);
    }
    
    if (!opts.show_long) {
        printf("\n");
    }

    // Perform recursive calls
    for (i = 0; i < dir_count; i++) {
        printf("\n%s:\n", dirs_to_visit[i]);
        do_ls(dirs_to_visit[i], opts);
        free(dirs_to_visit[i]);
    }
}
