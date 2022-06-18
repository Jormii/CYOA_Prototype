#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include <pspiofilemgr.h>

SceUID log_fd;
char log_buffer[1024];

#define LOG(format, ...)                                                           \
    {                                                                              \
        if (log_fd < 0)                                                            \
        {                                                                          \
            log_fd = sceIoOpen("umd0:/log.txt", PSP_O_WRONLY | PSP_O_CREAT, 0777); \
        }                                                                          \
                                                                                   \
        int l = snprintf(log_buffer, 1024, format, ##__VA_ARGS__);                 \
        sceIoWrite(log_fd, log_buffer, l);                                         \
    }

#define LOG_CLOSE()             \
    {                           \
        if (log_fd >= 0)        \
        {                       \
            sceIoClose(log_fd); \
            log_fd = -1;        \
        }                       \
    }

#endif