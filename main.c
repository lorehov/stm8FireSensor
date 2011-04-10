/*
* possible ADC channels (AINs): GPIOB(0-3), 
* we using PB0 as AIN for ADC
* PC1 - check pulse
* PC2 - channel error flag
* PC3 - fire flag
* see main.h for prototypes, enums and constants
*/
#include "main.h"

int main()
{
    u8 conversions_buffer[2];
    u8 delay_type = LONG_DELAY_1S;
    
    Periph_Init();
    while(1) {
        u8 value = makeConversion();
        if (TRESHHOLD_LEVEL >= value) {
            GPIO_WriteHigh(GPIOC, GPIO_PIN_1);
            delay15mcs();
            conversions_buffer[0] = makeConversion();
            delay500mcs();
            conversions_buffer[1] = makeConversion();
            if (FIRE_LEVEL == conversions_buffer[0] 
                && FIRE_LEVEL == conversions_buffer[0]) {
                GPIO_WriteHigh(GPIOC, GPIO_PIN_3); //achtung! fire!
            }
            GPIO_WriteLow(GPIOC, GPIO_PIN_1);
            channelErrorClear();
            delay_type = LONG_DELAY_1S;
        } else {
            //We don't want to organize two similar cycles one inside
            //another, so, we just change a delay value.
            if (LONG_DELAY_1S == delay_type) {
                delay_type = SHORT_DELAY_500MCS;
            } else {
                delay_type = LONG_DELAY_1S;
                channelError();
            }
        }
        delayByType(delay_type);
    }
    return 0;
}

void Periph_Init()
{
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	GPIO_DeInit(GPIOB);
   	GPIO_DeInit(GPIOC);
    
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT); //adc
	GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_OD_LOW_FAST); //check pulse
    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_OD_LOW_FAST); //channel error flag
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_OD_LOW_FAST); //fire flag

    GPIO_WriteLow(GPIOC, GPIO_PIN_1);
    GPIO_WriteLow(GPIOC, GPIO_PIN_2);
    GPIO_WriteLow(GPIOC, GPIO_PIN_3);
}

u8 makeConversion()
{
    u8 value;
   	ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_1,
						ADC1_PRESSEL_FCPU_D8, ADC1_EXTTRIG_GPIO, DISABLE,
						ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL1,
						DISABLE);
	ADC1_Cmd(ENABLE);
    ADC1_StartConversion();

    while(RESET == ADC1_GetFlagStatus(ADC1_IT_EOC)) {
    }
    
    value = ADC1_GetConversionValue() >> 8;
    return value;
}

void channelError()
{
    if (check_count <3) {
        check_count++;
    }
    
    if (check_count == 3) {
        GPIO_WriteHigh(GPIOC, GPIO_PIN_2);
    }
}

void channelErrorClear()
{
    check_count = 0;
    GPIO_WriteLow(GPIOC, GPIO_PIN_2);
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

void delay15mcs()
{
   	u8 i;
	for (i=0; i<45; i++){
	}
}

void delay500mcs()
{
   	u16 i;
	for (i=0; i<1500; i++){
	}
}

void delayByType(u8 delay_type)
{
    switch(delay_type) {
        case LONG_DELAY_1S: delay1s(); break;
        case SHORT_DELAY_500MCS: delay500mcs(); break;
        default: delay1s(); break;
    }
}