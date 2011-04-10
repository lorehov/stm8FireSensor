#include "inc/stm8s103f.h"
#include "inc/stm8s.h"

typedef enum {
    TRESHHOLD_LEVEL = 10,
    FIRE_LEVEL      = 20
} Signal_Levels;

typedef enum {
    SHORT_DELAY_500MCS = 0,
    LONG_DELAY_1S      = 1
} Delay_Type;

/*
* number of checks, when check_count >= 3,
* channel error will be triggered
*/
int check_count = 0;

void Periph_Init();

u8 makeConversion();
void channelError();
void channelErrorClear();

void delay1s();
void delay15mcs();
void delay500mcs();
void delayByType(u8 delay_type);
