#include "inc/stm8s103f.h"
#include "inc/stm8s.h"

typedef enum {
    TRESHHOLD_LEVEL = 10,
    FIRE_LEVEL      = 50
} Signal_Levels;

typedef enum {
    TRESHHOLD_CHANNEL  = ADC1_CHANNEL_6,
    FIRE_CHECK_CHANNEL = ADC1_CHANNEL_5
} ADC_Source;

void Periph_Init();

void initializeAdc(ADC_Source channel);
u8 makeConversion();
void blink();
void pulse();

void delay1s();
void delay100ms();
void delay50mcs();
void delay10mcs();
