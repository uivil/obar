

void *cpu_init() {
  int fd;
  fd = open("/proc/stat", O_RDONLY);

  return (void *)fd;
}

void cpu(void *p, char *s) {
  int fd = (int)p;

  static long ta[16], tb[16];
  static long ia[16], ib[16];
  static int idx = 0;
  long *total[2] = {ta, tb}, *idle[2] = {ia, ib};
  const uint32_t blk = 0xe29681;
  uint32_t cblk;

  static char buff[16 * 96];
  char *base;
  int i, off, n, k, previdx;

  previdx = (idx ^ 1) & 1;

  k = get_nprocs();
  n = read(fd, buff, (k + 1) * 96);
  base = strchr(buff, '\n') + 1;
  // padding 1 space
  s += 1;
  while (k--) {
    base = strchr(base, ' ') + 1;
    total[idx][k] = 0;
    i = 4;
    while (i--) {
      idle[idx][k] = strtol(base, &base, 10);
      total[idx][k] += idle[idx][k];
    }
    float idle_delta = idle[idx][k] - idle[previdx][k];
    float total_delta = total[idx][k] - total[previdx][k];

    float usage = 100.0 * (1.0 - idle_delta / total_delta);

    cblk = __bswap_32((blk + (int)(usage / 12.5)) << 8);
    memcpy(s, &cblk, 3);
    s += 3;

    base = strchr(base, '\n') + 1;
  }

  idx = previdx;

  s += 1;
  lseek(fd, 0, SEEK_SET);
}

int cpu_click(void *p, char *s, int btn) {
  if (!fork())
    execl("/bin/xterm", "/bin/xterm", "-s", "btop", NULL);
  return 0;
}

void cpu_close(void *p) {
  int fd = (int)p;
  close(fd);
}
