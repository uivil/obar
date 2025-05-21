

void temp(void *p, char *s) {
  static char icon[] = "\xf0\x9f\x8c\xa1";
  int n, i = 0, fd = (int)p;
  long vals[2];
  char buff[48];
  char *base;

  lseek(fd, 0, SEEK_SET);

  n = read(fd, buff, 48);
  buff[n] = '\0';

  base = strchr(buff, '\t');
  while (base) {
    if (i == 2)
      break;
    vals[i] = strtol(base, &base, 10);
    if (vals[i] > 0)
      i++;
  }

  memcpy(s, icon, 4);
  s += 4;
  if (vals[0] < 100)
    memcpy(s, itos + vals[0] * 2, 2);
  s += 3;
  if (vals[1] < 100)
    memcpy(s, itos + vals[1] * 2, 2);
}

void temp_close(void *p) {
  int fd = (int)p;
  close(fd);
}

void *temp_init() {
  int fd = open("/proc/acpi/ibm/thermal", O_RDONLY);
  return (void *)fd;
}
