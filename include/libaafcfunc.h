/*

    Library import functions for tests between AAFC

*/

#include "aafc.h"
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
#define LIB_AAFC_RPATH "aafc.dll"
#elif __APPLE__
#define LIB_AAFC_RPATH "./libaafc.dylib"
#else
#define LIB_AAFC_RPATH "./libaafc.so"
#endif

#ifndef LIBAAFCFUNC_H
#define LIBAAFCFUNC_H 1

typedef AAFCDECOUTPUT (*AAFCImport)(const unsigned char*);
typedef AAFC_HEADER* (*AAFCGetHeader)(const unsigned char*);
typedef void* (*AAFCFloatToInt)(float* arr, long size, unsigned char type);


class LibHandler{
private:
    void* handle;

    LibHandler(const char* path) : handle(nullptr) {
#ifdef _WIN32
        handle = LoadLibrary(path);
        if (!handle) std::cerr << "couldn't load dll: " << path << std::endl;
#else
        handle = dlopen(path, RTLD_LAZY);
        if (!handle) std::cerr << "couldn't load .so: " << path << std::endl;
#endif
    }

    ~LibHandler() {
        if (handle) {
#ifdef _WIN32
            FreeLibrary((HMODULE)handle);
#else
            dlclose(handle);
#endif
        }
    }

    LibHandler(const LibHandler&) = delete;
    LibHandler& operator=(const LibHandler&) = delete;
public:
    static LibHandler& getInstance(const char* path) {
        static LibHandler instance(path);
        return instance;
    }

    // o        h                .                   .
    template<typename T> T getFunc(const char* name) {
        if (!handle)
            return nullptr;
#ifdef _WIN32
        T func = (T)GetProcAddress((HMODULE)handle, name);
        if (!func) std::cerr << "function get failure: " << name << std::endl;
#else
        T func = (T)dlsym(handle, name);
        const char* error = dlerror();
        if (error) {
            std::cerr << "function get failure: " << name << " - " << error << std::endl;
            func = nullptr;
        }
#endif
        return func;
    }
};

AAFCDECOUTPUT LoadAAFC(const unsigned char* data) {
    AAFCImport aimport = LibHandler::getInstance(LIB_AAFC_RPATH).getFunc<AAFCImport>("aafc_import");
    if (!aimport) {
        perror("Could not initialize AAFC functions.");
        return {};
    }
    return aimport(data);
}

AAFC_HEADER* GrabHeader(const unsigned char* data) {
    AAFCGetHeader aheader = LibHandler::getInstance(LIB_AAFC_RPATH).getFunc<AAFCGetHeader>("aafc_getheader");
    if (aheader == NULL) {
        perror("Could not initialize AAFC functions.");
        return NULL;
    }
    return aheader(data);
}

void* FloatToInt(float* arr, long size, unsigned char type) {
    AAFCFloatToInt aheader = LibHandler::getInstance(LIB_AAFC_RPATH).getFunc<AAFCFloatToInt>("aafc_float_to_int");
    if (aheader == NULL) {
        perror("Could not initialize AAFC functions.");
        return NULL;
    }
    return aheader(arr, size, type);
}

#endif