//
// Created by josep on 10/04/2022.
//

#ifndef SHOOTER_FAT16UTILITIES_H
#define SHOOTER_FAT16UTILITIES_H

#include "Logic.h"

typedef struct Fat16{
    char systemName[8];
    unsigned short bytsPerSec;
    char secPerClus;
    unsigned short rsvdSecCnt;
    char numFATs;
    unsigned short rootEntCnt;
    unsigned short fatSz16;
    char volLab[11];
} Fat16;

typedef struct DirectoryEntryTableFat{
    char filename[8];
    char extension[3];
    unsigned char dirAttr;
    unsigned char nothing[14];
    unsigned short firstClust;
    unsigned int fileSize;
} DirectoryEntryTableFat;

Fat16 getFatInfo(FILE* fd);

void printFatInfo(Fat16 fat16);

unsigned int findFatFile(FILE* fd, char* filename, Fat16 fat16, int delete);

unsigned int recursiveFatFind(FILE* fd, Fat16  fat16, char * filename, int root, int delete);

char* parseFileName(DirectoryEntryTableFat directoryEntryTable);

#endif //SHOOTER_FAT16UTILITIES_H
