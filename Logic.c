//
// Created by josep on 10/04/2022.
//

#include <time.h>
#include "Logic.h"

int checkFileSystem(FILE* fd) {
    fseek(fd, 54, SEEK_CUR);
    int file = 0;

    char fat16[5];
    unsigned short ext2;

    fread(fat16, 5, 1, fd);

    if(!strcmp(fat16, "FAT16")) file = 1; else {

        fseek(fd, 1080, SEEK_SET);
        fread(&ext2, 2, 1, fd);

        if(ext2 == 0xEF53) file = 2;

    }

    return file;
}

char * timeToDate ( char* pre, time_t time) {
    char months[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char days[7][3] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    char *buffer = NULL;
    struct tm *ptm = localtime(&time);
    if (ptm == NULL) {
        buffer = "The localtime() function failed";
    } else {
        printf("%s%.3s %.3s %d %02d:%02d:%02d %d\n", pre, days[ptm->tm_wday], months[ptm->tm_mon], ptm->tm_mday, ptm->tm_hour,
                ptm->tm_min, ptm->tm_sec, 1900 + ptm->tm_year);
    }

    return buffer;
}
