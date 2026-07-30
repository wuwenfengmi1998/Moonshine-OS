#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include "state.h"

#define BUTTON_PASS_TIMES       50
#define BUTTON_LONGPASS_TIMES   1000

struct button {
    uint8_t      lock;
    uint8_t      pass;
    uint8_t      longpass;
    uint8_t      passread;
    uint8_t      pass_;
    uint8_t      longpassread;
    uint32_t     time;
};


void  button_init(void);
void  button_set_pass(struct button *b);
void  button_set_longpass(struct button *b);
char  button_pass(struct button *b);
char  button_pass_(struct button *b);
char  button_longpass(struct button *b);
void  button_reset(struct button *b);
void  button_updata(struct button *b, char a);

#endif
