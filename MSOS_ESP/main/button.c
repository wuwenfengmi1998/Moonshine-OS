#include "button.h"
#include "esp_timer.h"

struct button button_ems;
struct button button_power;
struct button button_beep;

void button_init(void)
{

}

void button_set_pass(struct button *b)
{
    b->pass = 1;
}

void button_set_longpass(struct button *b)
{
    b->longpass = 1;
}

char button_pass(struct button *b)
{
    if (b->pass == 1) {
        b->passread = 0;
        b->pass = 0;
        return 1;
    }
    return 0;
}

char button_pass_(struct button *b)
{
    if (b->pass_ == 1) {
        b->passread = 0;
        b->pass_ = 0;
        return 1;
    }
    return 0;
}

char button_longpass(struct button *b)
{
    if (b->longpass == 1) {
        b->longpassread = 0;
        b->longpass = 0;
        b->passread = 0;
        b->pass_ = 0;
        return 1;
    }
    return 0;
}

void button_reset(struct button *b)
{
    b->lock = 0;
    b->pass = 0;
    b->longpass = 0;
    b->passread = 0;
    b->pass_ = 0;
    b->longpassread = 0;
}

void button_updata(struct button *b, char a)
{
    uint32_t now = (uint32_t)(esp_timer_get_time() / 1000);

    if (b->lock == 0) {
        if (a == 0) {
            b->time = now;
            b->lock = 1;
            b->passread = 1;
            b->longpassread = 1;
        }
    } else {
        if (a == 0) {
            if (now > b->time + BUTTON_PASS_TIMES && b->passread == 1) {
                if (b->pass == 0) {
                    
                }
                b->pass = 1;
            }
            if (now > b->time + BUTTON_LONGPASS_TIMES && b->longpassread == 1) {
                if (b->longpass == 0) {
                    
                }
                b->longpass = 1;
                b->passread = 0;
                b->pass = 0;
                b->pass_ = 0;
            }
        } else {
            if (now > b->time + BUTTON_PASS_TIMES) {
                if (b->passread == 1) {
                    if (b->pass_ == 0) {
                        
                    }
                    b->pass_ = 1;
                }
            }
            b->lock = 0;
        }
    }
}
