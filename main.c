#include "Fat16Utilities.h"
#include "Ext2Utilities.h"
#include "Logic.h"

int main(int argc, char *argv[]) {

    FILE* fd = fopen(argv[2], "rb+");

    if (fd == NULL) {
        printf("There has been a problem opening the file\n");
    }
    else if(!strcmp(argv[1], "/info")) {
       if (argc != 3) {
           printf("There has been a problem with the number of arguments\n");
           printf("%d", argc);
           return 0;
       }
        int type = checkFileSystem(fd);

        printf("------ Filesystem Information ------\n\n");

        switch (type) {
            case 1:
                printf("Filesystem: FAT 16\n\n");
                printFatInfo(getFatInfo(fd));
                break;
            case 2:
                printf("Filesystem: EXT 2\n\n");
                printExtInfo(getExtInfo(fd));
                break;
            default:
                printf("File system is neither EXT2 nor FAT16.\n");
        }

        fclose(fd);
    }
    else if(!strcmp(argv[1], "/find")) {
       if (argc != 4) {
           printf("There has been a problem with the number of arguments\n");
           printf("%d", argc);
           return 0;
       }
        int type = checkFileSystem(fd);
        unsigned int size = 0;

        switch (type) {
            case 1:
                size = findFatFile(fd, argv[3], getFatInfo(fd), 0);
                if(size == 0) {
                    printf("Error. File not found.\n");
                }
                break;
            case 2:
                size = findExtFile(fd, argv[3], getExtInfo(fd), 0);
                if(size == 0) {
                    printf("Error. File not found.\n");
                }
                break;
            default:
                printf("Error. Volume not formatted in FAT16 or EXT2.\n");
        }
    }

    else if(!strcmp(argv[1], "/delete")) {
        if (argc != 4) {
            printf("There has been a problem with the number of arguments\n");
            return 0;
        }
        int type = checkFileSystem(fd);
        unsigned int size = 0;

        switch (type) {
            case 1:
                size = findFatFile(fd, argv[3], getFatInfo(fd) , 1);
                if(size == 0) {
                    printf("Error. File not found.\n");
                }
                break;
            case 2:
                size = findExtFile(fd, argv[3], getExtInfo(fd), 1);
                if(size == 0) {
                    printf("Error. File not found.\n");
                }
                break;
            default:
                printf("Error. Volume not formatted in FAT16 or EXT2.\n");
        }
    }
    else {
        printf("Error. Command not found\n");
    }


    return 0;
}
