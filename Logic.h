//
// Created by josep on 10/04/2022.
//

#ifndef SHOOTER_LOGIC_H
#define SHOOTER_LOGIC_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int checkFileSystem(FILE* fd);

char * timeToDate ( char* pre, time_t time);

#endif //SHOOTER_LOGIC_H
