
#ifndef CONFIG_H
#define CONFIG_H

#include "callback.h"

#define NUMBLOCKS 6
const char *fontname = "monospace:size=10";
char *const delimiter = "\xe2\x96\x95";
char barheight = 20; // px

#define BLOCK(loop, interval_value, click, size, init, close)                  \
  loop, interval_value, click, size, init, close

#define BLOCK_LIST                                                             \
  BLOCK(net, 1, net_click, 15, net_init, 0),                                   \
      BLOCK(temp, 1, 0, 9, temp_init, temp_close),                             \
      BLOCK(cpu, 1, cpu_click, 25, cpu_init, cpu_close),                       \
      BLOCK(battery, 2, 0, 10, battery_init, 0),                               \
      BLOCK(datetime, 16, 0, 7, 0, 0),                                         \
      BLOCK(weather, 1 << 11, 0, 30, weather_init, weather_close)







#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) name
interval_func interval_clbk[] = {BLOCK_LIST};

// Extract click function pointers
#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) click
click_func click_clbk[] = {BLOCK_LIST};

// Extract initialization function pointers
#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) init
init_func init_clbk[] = {BLOCK_LIST};

// Extract close function pointers
#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) close
close_func close_clbk[] = {BLOCK_LIST};

// Extract interval values
#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) interval_value
int interval[] = {BLOCK_LIST};

// Extract block sizes
#undef BLOCK
#define BLOCK(name, interval_value, click, size, init, close) size
int block_size[] = {BLOCK_LIST};

void *user_ptr[NUMBLOCKS];

#endif
