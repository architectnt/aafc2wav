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


#ifdef _WIN32
    #ifdef BUILDING_SHARED_LIBRARY
        #define EXPORT inline __declspec(dllexport)
    #else
        #define EXPORT
    #endif
#else
    #if defined __GNUC__ || __clang__
        #define EXPORT __attribute__((visibility("default")))
    #else
        #define EXPORT
    #endif
#endif

#define AAFC_SIGNATURE 0xAAFC // aafc fits under hexadecimal !!!
#define AFT_SIGNATURE 0x0AF7

// FORMATTED AS 'big SMALL TINY'
#define AAFCVERSION 300
#define AFTVERSION 100

typedef struct {
    unsigned short signature, version;
    unsigned int freq;
    unsigned char channels, bps, sampletype;
    unsigned int samplelength, loopst, loopend;
} AAFC_HEADER;

typedef struct { // used for older versions
    char headr[5];
    unsigned int version;
    unsigned int freq;
    unsigned char channels;
    unsigned int samplelength;
    unsigned char bps;
    unsigned char sampletype;
} AAFC_LCHEADER;

typedef struct {
    size_t size;
    unsigned char* data;
} AAFCOUTPUT;

typedef enum { // brought tragedy
    OUTPUT_T_FLOAT,
    OUTPUT_T_UNSIGNED_BYTE,
    OUTPUT_T_BYTE,
    OUTPUT_T_SHORT,
    OUTPUT_T_INT,
} OUTPUTTYPE;

typedef struct decoutput {
    AAFC_HEADER header;
    float* data;
} AAFCDECOUTPUT;

#endif // AAFC_H