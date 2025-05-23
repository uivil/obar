#include <locale.h>


const char itos[] = {"00010203040506070809"
                     "10111213141516171819"
                     "20212223242526272829"
                     "30313233343536373839"
                     "40414243444546474849"
                     "50515253545556575859"
                     "60616263646566676869"
                     "70717273747576777879"
                     "80818283848586878889"
                     "90919293949596979899100"
                    };

#include "blocks/battery.h"

void datetime(void *p, char *s) {

    time_t tm;
    struct tm *timeinfo;
    time(&tm);
    timeinfo = localtime(&tm);
    s += 1;
    memcpy(s, itos + timeinfo->tm_hour * 2, 2);
    s += 2;
    s[0] = ':';
    s += 1;
    memcpy(s, itos + timeinfo->tm_min * 2, 2);
}

#include "blocks/cpu.h"
#include "blocks/net.h"
#include "blocks/temp.h"
#include "blocks/weather.h"
