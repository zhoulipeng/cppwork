#ifndef __COMMANDOPTIONS_H
#define __COMMANDOPTIONS_H
#include <string.h>
// https://www.ibm.com/developerworks/cn/linux/l-gperf.html
struct CommandOptionCode {
    enum {
        HELPVERBOSE = 1,
        PASSWORD = 2,
        NOCOPYRIGHT = 3,
        NOLOG = 4,
        _64BIT = 5
    };
};
#endif
