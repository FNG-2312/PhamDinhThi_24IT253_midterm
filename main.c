#include "ls.h"

void do_ls(const char *dir_name) {
    printf("Dang liet ke thu muc: %s\n", dir_name);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_ls("."); 
    } else {
        while (--argc) {
            printf("%s:\n", *++argv);
            do_ls(*argv);
        }
    }
    return 0;
}
