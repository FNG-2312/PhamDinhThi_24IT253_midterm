#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>

// Structure to store the state of command-line flags
typedef struct {
    int show_all;           // -a: Include hidden files
    int show_almost_all;    // -A: Include hidden files except . and ..
    int show_inode;         // -i: Print inode number
    int show_long;          // -l: Use a long listing format
    int recursive;          // -R: List subdirectories recursively
    int reverse;            // -r: Reverse the order of the sort
    int sort_time;          // -t: Sort by time
    int sort_size;          // -S: Sort by file size
    int show_blocks;        // -s: Print allocated block size
    int show_type;          // -F: Append indicator to entries
    int show_human;         // -h: Human-readable sizes
    int numeric_id;         // -n: List numeric UIDs and GIDs
    int time_type;          // 0: mtime, 1: atime (-u), 2: ctime (-c)
    int disable_sort;       // -f: Do not sort
    int list_dir_as_file;   // -d: List directories themselves, not their contents
    int kilobytes_blocks;   // -k: Block size in kilobytes
    int non_printable_mode; // 1: force '?' (-q), 2: raw (-w)
} LsOptions;

// Structure to hold file data for sorting and processing
typedef struct {
    char *name;
    char *path;
    struct stat info;
} FileEntry;

// Function prototypes
void do_ls(const char *dir_name, LsOptions opts);
void show_file_info(const char *filename, struct stat *info_p, LsOptions opts);
void mode_to_letters(int mode, char str[]);
void format_human_size(long size, char *buf);
char get_type_indicator(int mode);
void print_safe_name(const char *name, int mode);

#endif
