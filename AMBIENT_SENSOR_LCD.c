
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <stdio.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "ambient_LCD_libraries/HAL_I2C.h"
#include "ambient_LCD_libraries/HAL_OPT3001.h"
#include "ambient_LCD_libraries/LcdDriver/Crystalfontz128x128_ST7735.h"
#include "ambient_LCD_libraries/LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "AMBIENT_SENSOR_LCD.h"

/* Graphic library context */
Graphics_Context g_sContext;

void init_LCD_Ambient(){

         /* Initializes display */
         Crystalfontz128x128_Init();

         /* Set default screen orientation */
         Crystalfontz128x128_SetOrientation(0);

         /* Initializes graphics context */
         Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
         Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
         GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
         Graphics_clearDisplay(&g_sContext);
         Graphics_drawStringCentered(&g_sContext,
                                         (int8_t *)"Ambient Sensor: ",
                                         AUTO_STRING_LENGTH,
                                         64,
                                         30,
                                         OPAQUE_TEXT);


//       __delay_cycles(100000);
         /* Initialize I2C communication */
          Init_I2C_GPIO();
          I2C_init();

          /* Initialize OPT3001 digital ambient light sensor */
          OPT3001_init();

//          __delay_cycles(100000);

}

void display_temperature_value (float lux) {


            char string[20];
            sprintf(string, "%f", lux);



            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            Graphics_drawStringCentered(&g_sContext,
                                                  (int8_t *)"MOTION DETECTED!! ",
                                                   AUTO_STRING_LENGTH,
                                                   64,
                                                   30,
                                                   OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext,
                                            (int8_t *)string,
                                            6,
                                            48,
                                            70,
                                            OPAQUE_TEXT);

            sprintf(string, "lux");
            Graphics_drawStringCentered(&g_sContext,
                                            (int8_t *)string,
                                            3,
                                            86,
                                            70,
                                            OPAQUE_TEXT);

}

void display_basic_message(){
    Graphics_clearDisplay(&g_sContext);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_drawStringCentered(&g_sContext,
                                       (int8_t *)"Ambient Sensor: ",
                                        AUTO_STRING_LENGTH,
                                        64,
                                        30,
                                        OPAQUE_TEXT);
}

float get_lux(){
    return OPT3001_getLux();

}

