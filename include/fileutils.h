#include <aafc.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>

// cross-platform defines
#define mkdir(dir, mode) _mkdir(dir)
#define strdup _strdup

#else
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

static char* concat_path(const char* filename) {
	if (!filename || *filename == '\0')
		return NULL;

	size_t len = (strlen(filename) + 1) + 5;
	char* result = (char*)malloc(len);
	if (result)
		snprintf(result, len, "%s.wav", filename);
	return result;
}

static AAFCOUTPUT ReadFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if (file == NULL) {
		perror("cant open aafc file :(");
		return {};
	}

	fseek(file, 0, SEEK_END);
	size_t fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* data = (unsigned char*)malloc(fsize);
	if (data == NULL) {
		perror("MEMORY ALLOC INTERNAL ERROR");
		fclose(file);
		return {};
	}

	if (fread(data, 1, fsize, file) != fsize) {
		perror("error trying to load the file");
		fclose(file);
		return {};
	}

	fclose(file);
	return { fsize, data };
}