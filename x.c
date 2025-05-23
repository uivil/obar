
int get_filepath(char *path, char *keyword, char *dev);
#include "all.h"
#include "config.h"

struct {
    Display *d;
    int w, h;
} dpy;

int scr;
Window win;
XftFont *font;

struct block blocks[NUMBLOCKS];

int running = 1;

int get_filepath(char *path, char *keyword, char *dev) {
    struct dirent *dir;
    DIR *d;
    int pathlen, fd, n;
    d = opendir(path);
    if (!d)
        return -1;
    while ((dir = readdir(d)) != NULL && strstr(dir->d_name, keyword) == NULL)
        ;
    if (dir == NULL) {
        closedir(d);
        return -1;
    }
    strcpy(dev, path);
    pathlen = strlen(path);
    fd = open(path, O_RDONLY);
    n = readlinkat(fd, dir->d_name, dev + pathlen, 256);
    close(fd);
    closedir(d);
    if (n == -1)
        return -1;
    n += pathlen;
    dev[n] = '\0';
    return n;
}

int string_to_glyphs_width(const char *string, int size) {
    XGlyphInfo e;
    XftTextExtentsUtf8(dpy.d, font, string, size, &e);
    return e.xOff;
}

struct timespec timespecsub(struct timespec end, struct timespec start) {
    struct timespec result;

    // Check if we need to borrow from seconds
    int borrow = (end.tv_nsec < start.tv_nsec);

    result.tv_sec = end.tv_sec - start.tv_sec - borrow;
    result.tv_nsec = end.tv_nsec - start.tv_nsec + (borrow * 1000000000L);

    return result;
}

int process(struct timespec now, struct timespec *last_exec, struct timespec *wakeup) {

    int i = 0, off = 0, width = 0, dirty = 0;
    time_t sec;
    for (off = 0, i = 0; i < NUMBLOCKS; i++) {
        blocks[i].str = bar.str + off;
        if (timespecsub(now, last_exec[i]).tv_sec >= interval[i] &&
                interval_clbk[i]) {
            interval_clbk[i](user_ptr[i], blocks[i].str);
            last_exec[i] = now;
            dirty = 1;
        }

        blocks[i].w = string_to_glyphs_width(blocks[i].str, block_size[i]);
        width += blocks[i].w;
        off += block_size[i];
        if (i == NUMBLOCKS - 1)
            break;
        memcpy(bar.str + off, delim.str, delim.size);
        off += delim.size;
    }
    bar.str[off] = '\0';
    bar.size = off;
    bar.w = width + delim.w * (NUMBLOCKS - 1);
    return dirty;
}

void sighandler(int sig) {
    running = 0;
}

int main() {
    int fd, n, i, btn, maxsize = 0;
    XEvent xev;
    struct input_event evt;
    char dev[128];
    struct timespec now, wakeup;
    int clock_type = CLOCK_REALTIME;
    int mininterval = INT_MAX, maxinterval = 0;

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGHUP, sighandler);

    dpy.d = XOpenDisplay(0);
    scr = XDefaultScreen(dpy.d);
    win = XDefaultRootWindow(dpy.d);
    dpy.w = DisplayWidth(dpy.d, scr);

    font = XftFontOpenName(dpy.d, scr, fontname);
    bar.w = DisplayWidth(dpy.d, scr);
    bar.h = barheight;

    delim.str = delimiter;
    delim.size = strlen(delim.str);
    delim.w = string_to_glyphs_width(delim.str, delim.size);

    clock_gettime(clock_type, &now);
    struct timespec last_exec[NUMBLOCKS];
    i = NUMBLOCKS;
    while (i--) {
        if (init_clbk[i])
            user_ptr[i] = init_clbk[i]();
        maxsize += block_size[i];
        if (interval[i] > 0) {
            mininterval = MIN(interval[i], mininterval);
            maxinterval = MAX(interval[i], maxinterval);
        }

        last_exec[i].tv_sec = now.tv_sec - 100000;
        last_exec[i].tv_nsec = 0;
    }

    bar.size = NUMBLOCKS * (maxsize + 1);
    bar.str = malloc(bar.size);
    memset(bar.str, ' ', bar.size);

    wakeup.tv_sec = 0;
    wakeup.tv_nsec = 100000000;

    while (running) {
        if (process(now, last_exec, &wakeup)) {
            XStoreName(dpy.d, win, bar.str);
            XFlush(dpy.d);
        }

        clock_nanosleep(clock_type, TIMER_ABSTIME, &wakeup, NULL);
        clock_gettime(clock_type, &now);
    }



    i = NUMBLOCKS;
    while (i--) {
        if (close_clbk[i])
            close_clbk[i](user_ptr[i]);
    }
    XftFontClose(dpy.d, font);
    XFlush(dpy.d);
    XCloseDisplay(dpy.d);
    free(bar.str);
    return 0;
}
