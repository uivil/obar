
#ifndef DT_H
#define DT_H
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xmu/CurUtil.h>
#include <X11/Xutil.h>

#include <alsa/asoundlib.h>
#include <byteswap.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

struct block {
  int w, h;
  char *str;
};

struct {
  int w, h, size;
  char *str;
} bar, delim;

typedef void (*interval_func)(void *, char *);
typedef int (*click_func)(void *, char *, int);
typedef void *(*init_func)();
typedef void (*close_func)(void *);

#endif
