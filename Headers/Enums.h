#ifndef _CH_ENUMS_
#define _CH_ENUMS_

typedef enum {
    NAME = 0,
    RECORD = 1
} Token;


typedef enum {
    MOD_NONE = 0,
    MOD_OPEN = 1,
    MOD_CLOSED = 2,
    MOD_FINAL = 3,
    MOD_SHARED = 4
} Mod;

#endif
