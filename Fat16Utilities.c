//
// Created by josep on 10/04/2022.
//

#include "Fat16Utilities.h"


Fat16 getFatInfo(FILE* fd) {
    fseek(fd, 3, SEEK_SET);
    Fat16 fat16;

    fread(fat16.systemName, 8, 1, fd);
    fread(&fat16.bytsPerSec, 2, 1, fd);
    fread(&fat16.secPerClus, 1, 1, fd);
    fread(&fat16.rsvdSecCnt, 2, 1, fd);
    fread(&fat16.numFATs, 1, 1, fd);
    fread(&fat16.rootEntCnt, 2, 1, fd);

    fseek(fd, 3, SEEK_CUR);
    fread(&fat16.fatSz16, 2, 1, fd);

    fseek(fd, 19, SEEK_CUR);
    fread(fat16.volLab, 11, 1, fd);

    return fat16;
}

void printFatInfo(Fat16 fat16) {
    printf("System Name: %s\n"
           "size: %d\n"
           "Sectors Per Cluster: %d\n"
           "Reserved sectors: %d\n"
           "Number of FATs: %d\n"
           "MaxRootEntries: %d\n"
           "Sectors per FAT: %d\n"
           "Label: %s\n", fat16.systemName, fat16.bytsPerSec, (int) fat16.secPerClus,
           fat16.rsvdSecCnt, (int) fat16.numFATs, fat16.rootEntCnt, fat16.fatSz16, fat16.volLab);
}

unsigned int findFatFile(FILE* fd, char* filename, Fat16 fat16, int delete) {
    unsigned int firstRootDir = (unsigned int) fat16.rsvdSecCnt + ((unsigned int) fat16.numFATs * (unsigned int) fat16.fatSz16);

    return recursiveFatFind(fd, fat16, filename, firstRootDir * fat16.bytsPerSec, delete);
}

unsigned int recursiveFatFind(FILE* fd, Fat16  fat16, char * filename, int root, int delete) {
    DirectoryEntryTableFat directoryEntryTable[fat16.rootEntCnt];
    unsigned int found;

    for(int i = 0 ; i < fat16.rootEntCnt; i++) {
        fseek (fd, root + (i * 32), SEEK_SET);
        fread(&directoryEntryTable[i], sizeof (DirectoryEntryTableFat), 1, fd);
    }

    for (int i = 0; i < fat16.rootEntCnt; i++) {
        if (directoryEntryTable[i].dirAttr == 0x10 && directoryEntryTable[i].filename[0] != '.' && directoryEntryTable[i].filename[0] != '\xE5') {
            printf("%s is a directory\n", directoryEntryTable[i].filename);
            int dir = fat16.rootEntCnt * 32 + ((directoryEntryTable[i].firstClust - 2) * fat16.secPerClus * fat16.bytsPerSec)
                    + (fat16.rsvdSecCnt * fat16.bytsPerSec) + (fat16.fatSz16 * fat16.numFATs * fat16.bytsPerSec);

            found = recursiveFatFind(fd, fat16, filename, dir, delete);
            if(found == 1) return found;
        }
        else if (directoryEntryTable[i].dirAttr == 0x20  && directoryEntryTable[i].filename[0] != '\xE5') {
            char* aux = parseFileName(directoryEntryTable[i]);
            printf("%s is a file\n", aux);
            if (!strcmp(aux, filename)) {
                if(delete == 0) {
                    printf("File found, it is %d bytes in size\n", directoryEntryTable[i].fileSize);
                }
                else {
                    fseek (fd, root + (i * 32), SEEK_SET);
                    char aux[11] = {'\xE5', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
                    fwrite(aux,  11,  1, fd);
                    printf("File removed succesfully\n");
                }
                return 1;
            }
        }
    }

    return 0;
}

char* parseFileName(DirectoryEntryTableFat directoryEntryTable) {
    char* aux = (char*) malloc(sizeof (char));
    int k = 0;
    for(int j = 0; j < 8; j++) {
        if(directoryEntryTable.filename[j] != ' ') {
            if(directoryEntryTable.filename[j] >= 'A' && directoryEntryTable.filename[j] <= 'Z') {
                aux[k] = directoryEntryTable.filename[j] + 32;
            }
            else {
                aux[k] = directoryEntryTable.filename[j];
            }

            k++;
            aux = realloc(aux, sizeof (char) * (k + 1));
        }

    }

    if(strlen(directoryEntryTable.extension) != 0 && directoryEntryTable.extension[0] != ' ') {
        aux[k] = '.';
        aux = realloc(aux, sizeof (char) * (k + 1));
        k++;
        for(int j = 0; j < 3; j++) {
            if(directoryEntryTable.extension[j] != ' ') {
                if(directoryEntryTable.extension[j] >= 'A' && directoryEntryTable.extension[j] <= 'Z') {
                    aux[k] = directoryEntryTable.extension[j] + 32;
                }
                else {
                    aux[k] = directoryEntryTable.extension[j];
                }
                k++;
                aux = realloc(aux, sizeof (char) * (k + 1));
            }
        }
    }

    return aux;
}
