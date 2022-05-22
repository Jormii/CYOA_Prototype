#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include <pspiofilemgr.h>

SceUID fd = -1;
char buffer[1024 + 1];
#define LOG(format, ...)                                                       \
    {                                                                          \
        if (fd < 0)                                                            \
        {                                                                      \
            fd = sceIoOpen("umd0:/log.txt", PSP_O_WRONLY | PSP_O_CREAT, 0777); \
        }                                                                      \
                                                                               \
        int l = snprintf(buffer, 1024, format, ##__VA_ARGS__);                 \
        sceIoWrite(fd, buffer, l);                                             \
    }

#define LOG_CLOSE()         \
    {                       \
        if (fd >= 0)        \
        {                   \
            sceIoClose(fd); \
            fd = -1;        \
        }                   \
    }

#endif