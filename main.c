/*
* see main.h for prototypes, enums and constants
*/
#include "main.h"

/*
* number of checks, when check_count >= 3,
* channel error will be triggered
*/
int check_count = 0;

int main()
{
    u8 conversions_buffer[2];
    u8 J = 0; //counter for fire check measurements
    u8 i = 0; //empty tries (when tresh on line) counter
    u8 k = 0; //empty cycles counter
    
    Periph_Init();
    while(1) {
        initializeAdc(TRESHHOLD_CHANNEL);
        if (TRESHHOLD_LEVEL >= makeConversion()) {
            //no treshhold here
            //make conversion
            initializeAdc(FIRE_CHECK_CHANNEL);
            pulse();
            if (0 == J) {
                conversions_buffer[0] = makeConversion();
                J++;
            } else if (1 == J) {
                conversions_buffer[1] = makeConversion();
                J++;
            }
            
            //if both conversions are made - check these
            if (2 == J) {
                if (FIRE_LEVEL < conversions_buffer[0] 
                    && FIRE_LEVEL < conversions_buffer[1]) {
                    //if fire - activate LED and break the cycle
                    GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
                    break;
                } else {
                    blink();
                    conversions_buffer[0] = 0;
                    conversions_buffer[1] = 0;
                    J = 0;
                    i = 0;
                    k = 0;
                    delay1s();
                }
            }
        } else {
            //tresh on line
            i++;
            if (100 == i) {
                i = 0;
                J = 0;
                k++;
                if (3 > k) {
                    pulse();
                }
                delay1s();
                if (255 == k) {
                    k = 0;
                }
            }
        }
    }
    return 0;
}

void Periph_Init()
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOC);
    
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT); //adc fire
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT); //adc treshhold
    GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_OD_LOW_FAST); //led
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); //pulse

    GPIO_WriteLow(GPIOA, GPIO_PIN_2);
    GPIO_WriteLow(GPIOC, GPIO_PIN_7);
}

void initializeAdc(ADC_Source channel)
{
    //as we need to take ADC from different channels,
    //we place initialization here
    ADC1_DeInit();
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, channel,
              ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_GPIO, DISABLE,
              ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL1,
              DISABLE);
    ADC1_Cmd(ENABLE);
}

u8 makeConversion()
{
    u8 value;
    ADC1_StartConversion();
    while(RESET == ADC1_GetFlagStatus(ADC1_IT_EOC)) {
    }
    
    value = ADC1_GetConversionValue() >> 8;
    return value;
}

void blink()
{
    GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
    GPIO_WriteLow(GPIOA, GPIO_PIN_2);

}

void pulse()
{
    GPIO_WriteHigh(GPIOC, GPIO_PIN_7);
    delay10mcs();
    GPIO_WriteLow(GPIOC, GPIO_PIN_7);
    delay50mcs();
}

void delay1s()
{
    int i;
    u8 j;
    for (j=0; j<100; j++) {
        for (i=0; i<30119; i++){
        }
    }
}

void delay100ms()
{
    int i;
    u8 j;
    for (j=0; j<10; j++) {
        for (i=0; i<30119; i++){
        }
    }
}

void delay50mcs()
{
    u8 i;
    for (i=0; i<150; i++){
    }
}

void delay10mcs()
{
    u8 i;
    for (i=0; i<30; i++){
    }
}
