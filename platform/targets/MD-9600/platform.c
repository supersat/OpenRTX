/***************************************************************************
 *   Copyright (C) 2021 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN                             *
 *                         Silvano Seva IU2KWO                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <interfaces/gpio.h>
#include <interfaces/nvmem.h>
#include <interfaces/platform.h>
#include <hwconfig.h>
#include <string.h>
#include <ADC1_MDx.h>
#include <calibInfo_MDx.h>
#include <toneGenerator_MDx.h>
#include <interfaces/rtc.h>

hwInfo_t hwInfo;

void platform_init()
{
    gpio_setMode(LCD_BKLIGHT, ALTERNATE);
    gpio_setAlternateFunction(LCD_BKLIGHT, 3);

    gpio_setMode(CH_SELECTOR_0, INPUT_PULL_UP);
    gpio_setMode(CH_SELECTOR_1, INPUT_PULL_UP);

    gpio_setMode(PTT_SW, INPUT);

    gpio_setMode(PWR_SW, OUTPUT);

    /*
     * Initialise ADC1, for vbat, RSSI, ...
     * Configuration of corresponding GPIOs in analog input mode is done inside
     * the driver.
     */
//     adc1_init();

    memset(&hwInfo, 0x00, sizeof(hwInfo));

//     nvm_init();                      /* Initialise non volatile memory manager */
    toneGen_init();                  /* Initialise tone generator              */
    rtc_init();                      /* Initialise RTC                         */

    /*
     * Configure TIM8 for backlight PWM: Fpwm = 100kHz with 8 bit of resolution.
     * APB2 freq. is 84MHz, but timer runs at twice this frequency.
     * Then: PSC = 655 to have Ftick = 256.097kHz
     * With ARR = 256, Fpwm is 100kHz;
     * Backlight pin is connected to TIM8 CR1.
     */
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    __DSB();

    TIM8->ARR = 255;
    TIM8->PSC = 654;
    TIM8->CNT = 0;
    TIM8->CR1   |= TIM_CR1_ARPE;    /* LCD backlight is on PC6, TIM8-CH1 */
    TIM8->CCMR1 |= TIM_CCMR1_OC1M_2
                |  TIM_CCMR1_OC1M_1
                |  TIM_CCMR1_OC1PE;
    TIM8->CCER  |= TIM_CCER_CC1E;
    TIM8->BDTR  |= TIM_BDTR_MOE;
    TIM8->CCR1 = 0;
    TIM8->EGR  = TIM_EGR_UG;        /* Update registers */
    TIM8->CR1 |= TIM_CR1_CEN;       /* Start timer */
}

void platform_terminate()
{
    /* Shut down backlight */
    gpio_setMode(LCD_BKLIGHT, OUTPUT);
    gpio_clearPin(LCD_BKLIGHT);

    /* Shut down timer */
    RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;
    __DSB();

    /* Shut down all the modules */
//     adc1_terminate();
    toneGen_terminate();
    rtc_terminate();

    /* Finally, remove power supply */
    gpio_clearPin(PWR_SW);
}

float platform_getVbat()
{
    /*
     * Battery voltage is measured through an 1:3 voltage divider and
     * adc1_getMeasurement returns a value in mV. Thus, to have effective
     * battery voltage multiply by three and divide by 1000
     */
    return 0.0f;
}

float platform_getMicLevel()
{
    return 0.0f;
}

float platform_getVolumeLevel()
{
    return 0.0f;
}

uint8_t platform_getChSelector()
{
    static const uint8_t rsPositions[] = { 1, 4, 2, 3};
    int pos = gpio_readPin(CH_SELECTOR_0)
            | (gpio_readPin(CH_SELECTOR_1) << 1);

    return rsPositions[pos];
}

bool platform_getPttStatus()
{
    /* PTT line has a pullup resistor with PTT switch closing to ground */
    return (gpio_readPin(PTT_SW) == 0) ? true : false;
}

void platform_ledOn(led_t led)
{
    /* No LEDs on this platform */
    (void) led;
}

void platform_ledOff(led_t led)
{
    /* No LEDs on this platform */
    (void) led;
}

void platform_beepStart(uint16_t freq)
{
    /* TODO */
    (void) freq;
}

void platform_beepStop()
{
    /* TODO */
}

void platform_setBacklightLevel(uint8_t level)
{
    TIM8->CCR1 = level;
}

const void *platform_getCalibrationData()
{
    return NULL;
}

const hwInfo_t *platform_getHwInfo()
{
    return &hwInfo;
}
