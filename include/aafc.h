/*
    Copyright (C) 2024 Architect Enterprises
    This file is apart of AAFC and is licenced under the MIT Licence.
*/

#ifndef AAFC_H
#define AAFC_H 1

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h> // shucks
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    unsigned short signature, version;
    unsigned int freq;
    unsigned char channels, bps, sampletype;
    unsigned int samplelength, loopst, loopend;
} AAFC_HEADER;

typedef struct {
    size_t size;
    unsigned char* data;
} AAFCOUTPUT;

typedef struct decoutput {
    AAFC_HEADER header;
    float* data;
} AAFCDECOUTPUT;

#endif // AAFC_H