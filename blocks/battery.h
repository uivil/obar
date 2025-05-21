void *battery_init() {
  static char d[256];
  int pathlen = get_filepath("/sys/class/power_supply/", "BAT", d + 4);
  memcpy(d, &pathlen, 4);
  return d;
}

void battery(void *p, char *s) {
  static const char *const text[] = {
      "\xe2\x9a\xa1",     0, 0, "\xe2\x99\xbb\xef\xb8\x8f",
      "\xf0\x9f\x94\x8c", 0, 0, "\xf0\x9f\x94\x8b",
      "\xf0\x9f\x9b\x91"};
  static int lengths[] = {3, 0, 0, 6, 4, 0, 0, 4, 4};

  int fd;
  int *len, n, i;
  char *path;
  char buff[16];

  path = (char *)p + 4;
  len = p;

  strcpy(path + *len, "/status");
  fd = open(path, O_RDONLY);
  n = read(fd, buff, 16);
  close(fd);
  buff[n] = '\0';
  i = strchr(buff, '\n') - buff - 4;
  s += 1;
  memcpy(s, text[i], lengths[i]);

  strcpy(path + *len, "/capacity");
  fd = open(path, O_RDONLY);
  n = read(fd, buff, 16);
  close(fd);
  buff[n] = '\0';
  n = strchr(buff, '\n') - buff;
  s += lengths[i];
  s[0] = ' ';
  s += 1;
  memcpy(s, buff, n);
  s[n] = '%';
  s[n + 1] = ' ';
}
