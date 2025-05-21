

struct net_cb {
    char wifi_path[256];
    int wifi_len;
    char ether_path[256];
    int ether_len;
};
// : wifi disabled
// : no wifi connection
// : wifi connection with quality
// : no ethernet
// : ethernet working
// : vpn is active


void *net_init() {
    static struct net_cb cb;
    int pathlen;
    cb.wifi_len = get_filepath("/sys/class/net/", "w", cb.wifi_path);
    cb.ether_len = get_filepath("/sys/class/net/", "e", cb.ether_path);
    return &cb;
}

void net(void *p, char *s) {

    static char *wifi_icons[] = {"\xf0\x9f\x93\xa1", "\xf0\x9f\x93\xb6",
                                 "\xe2\x9d\x8c"
                                };
    static char *ether_icons[] = {"\xe2\x9d\x8e", "\xf0\x9f\x8c\x90"};
    int fd, n, len;
    char c;
    char buff[64], *base;
    long link;
    struct net_cb cb = *(struct net_cb *)p;

    // WiFi
    if (cb.wifi_len > 0) {
        strcpy(cb.wifi_path + cb.wifi_len, "/carrier");
        fd = open(cb.wifi_path, O_RDONLY);
        n = read(fd, &c, 1);
        close(fd);
        c = n < 1 ? 2 : c - '0';
        len = 4 - (c == 2);
        memcpy(s, wifi_icons[c], len);
        s += len + 1;
        memset(s, ' ', 10);



        if (c == 1) {
            fd = open("/proc/net/wireless", O_RDONLY);
            lseek(fd, 163, SEEK_SET);
            n = read(fd, buff, 64);
            close(fd);
            buff[n - 1] = '\0';
            char *base = strchr(buff, ':') + 1;
            n = 2;
            while (n--) {
                link = strtol(base, &base, 10);
            }
            link = link * 100 / 70;
            len = 2 + (link > 99);
            memcpy(s, itos + link * 2, len);
            s += len;
            *s = '%';
            s += 1;
        }
    } else {
        net_init();
    }

    // Ethernet
    // if (cb.ether_len > 0) {
    //  strcpy(cb.ether_path + cb.ether_len, "/carrier");
    //  fd = open(cb.ether_path, O_RDONLY);
    //  n = read(fd, &c, 1);
    //  close(fd);
    //  c = n < 1 ? 2 : c - '0';
    //  len = 4 - (c == 0);
    //  memcpy(s + 2, ether_icons[c], len);
    //}
}

// https://unix.stackexchange.com/questions/193845/how-to-determine-interface-running-state-without-ifconfig


void net_close(void *p) {}

int net_click(void *p, char *s, int btn) {
    if (!fork())
        execl("/bin/iwgtk", "/bin/iwgtk", NULL);
    return 0;
}
