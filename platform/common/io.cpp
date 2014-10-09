#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "io.h"


#ifdef C_IO_FUNCTIONS

struct FileHandle {
    FILE* file;
    char name[MAX_FILENAME_LEN];
    char flags[10];
};

FileHandle* file_open(const char* filename, const char* params) {
    FileHandle* h = (FileHandle*)malloc(sizeof(FileHandle));
    h->file = fopen(filename, params);

    if (h->file == NULL) {
        free(h);
        h = NULL;
    }

    strncpy(h->name, filename, MAX_FILENAME_LEN);
    h->name[MAX_FILENAME_LEN-1] = '\0';
    strcpy(h->flags, params);
    return h;
}

void file_close(FileHandle* h) {
    fclose(h->file);
    free(h);
}
void file_read(void* buf, int bs, int size, FileHandle* h) {
    fread(buf, bs, size, h->file);
}
void file_write(const void* buf, int bs, int size, FileHandle* h) {
    fwrite(buf, bs, size, h->file);
}
void file_gets(char* buf, int size, FileHandle* h) {
    fgets(buf, size, h->file);
}
void file_putc(char c, FileHandle* h) {
    fputc(c, h->file);
}

void file_rewind(FileHandle* h) {
    rewind(h->file);
}
int file_tell(FileHandle* h) {
    return ftell(h->file);
}
void file_seek(FileHandle* h, int pos, int flags) {
    fseek(h->file, pos, flags);
}
int file_getSize(FileHandle* h) {
    int pos = ftell(h->file);
    fseek(h->file, 0, SEEK_END);
    int ret = ftell(h->file);
    fseek(h->file, pos, SEEK_SET);
    return ret;
}
void file_setSize(FileHandle* h, size_t neededSize) {
    size_t fileSize = file_getSize(h);
    h->file = freopen(h->name, "ab", h->file);
    for (; fileSize<neededSize; fileSize++)
        fputc(0, h->file);
    h->file = freopen(h->name, h->flags, h->file);
}

void file_printf(FileHandle* h, const char* s, ...) {
    va_list args;
    va_start(args, s);

    char buf[512];
    vsprintf(buf, s, args);

    fputs(buf, h->file);
}

bool file_exists(const char* filename) {
    return access(filename, R_OK) == 0;
}


DIR* fs_opendir(const char* s) {
    return opendir(s);
}
void fs_closedir(DIR* dir) {
    closedir(dir);
}
struct dirent* fs_readdir(DIR* d) {
    return readdir(d);
}

void fs_getcwd(char* dest, size_t maxLen) {
    getcwd(dest, maxLen);
}
void fs_chdir(const char* s) {
    chdir(s);
}

#endif