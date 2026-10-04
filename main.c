#include "ls.h"

int main(int argc, char *argv[]) {
    LsOptions opts = {0}; // Initialize all flags to 0 (off)
    int c;

    // Check if output is a terminal to set default non-printable character behavior
    if (isatty(STDOUT_FILENO)) {
        opts.non_printable_mode = 1; // Default to '?' for terminal
    } else {
        opts.non_printable_mode = 2; // Default to raw for piped output
    }

    // Parse command-line options using getopt
    while ((c = getopt(argc, argv, "aAilRrtSsFhncufdkqw")) != -1) {
        switch (c) {
            case 'a': opts.show_all = 1; break;
            case 'A': opts.show_almost_all = 1; break;
            case 'i': opts.show_inode = 1; break;
            case 'l': opts.show_long = 1; break;
            case 'R': opts.recursive = 1; break;
            case 'r': opts.reverse = 1; break;
            case 't': opts.sort_time = 1; break;
            case 'S': opts.sort_size = 1; break;
            case 's': opts.show_blocks = 1; break;
            case 'F': opts.show_type = 1; break;
            case 'h': opts.show_human = 1; opts.kilobytes_blocks = 0; break;
            case 'n': opts.numeric_id = 1; opts.show_long = 1; break;
            case 'c': opts.time_type = 2; break; // Use ctime
            case 'u': opts.time_type = 1; break; // Use atime
            case 'f': opts.disable_sort = 1; break;
            case 'd': opts.list_dir_as_file = 1; break;
            case 'k': opts.kilobytes_blocks = 1; opts.show_human = 0; break;
            case 'q': opts.non_printable_mode = 1; break; // Force '?'
            case 'w': opts.non_printable_mode = 2; break; // Raw output
            default:
                fprintf(stderr, "Usage: %s [-aAilRrtSsFhncufdkqw] [file...]\n", argv[0]);
                exit(1);
        }
    }

    // If no specific directories/files are provided, list the current directory
    if (optind == argc) {
        do_ls(".", opts);
    } else {
        // Iterate through all provided directories/files
        while (optind < argc) {
            // Print directory name header if multiple arguments are given
            if (argc - optind > 1 && !opts.list_dir_as_file) {
                printf("%s:\n", argv[optind]);
            }
            do_ls(argv[optind], opts);
            optind++;
        }
    }
    return 0;
}
