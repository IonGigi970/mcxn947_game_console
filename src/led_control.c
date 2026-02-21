/**
 * @file led_control.c
 * @brief LED Ring Control Implementation for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#include "led_control.h"
#include "pin_mux.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

static LED_TypeDef_t LEDs[] = {
    {GPIO4, SHIELD_LED1_GPIO_PIN},
    {GPIO0, SHIELD_LED2_GPIO_PIN},
    {GPIO0, SHIELD_LED3_GPIO_PIN},
    {GPIO0, SHIELD_LED4_GPIO_PIN},
    {GPIO2, SHIELD_LED5_GPIO_PIN},
    {GPIO2, SHIELD_LED6_GPIO_PIN},
    {GPIO2, SHIELD_LED7_GPIO_PIN},
    {GPIO2, SHIELD_LED8_GPIO_PIN},
};

static const uint8_t NUM_LEDS = sizeof(LEDs) / sizeof(LED_TypeDef_t);

/*******************************************************************************
 * Code
 ******************************************************************************/

LED_TypeDef_t* LED_GetArray(void)
{
    return LEDs;
}

uint8_t LED_GetCount(void)
{
    return NUM_LEDS;
}

void LED_On(uint8_t led_index)
{
    if (led_index < NUM_LEDS) {
        GPIO_PinWrite(LEDs[led_index].gpio, LEDs[led_index].pin, 1);
    }
}

void LED_Off(uint8_t led_index)
{
    if (led_index < NUM_LEDS) {
        GPIO_PinWrite(LEDs[led_index].gpio, LEDs[led_index].pin, 0);
    }
}

void LED_ClearAll(void)
{
    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
        GPIO_PinWrite(LEDs[i].gpio, LEDs[i].pin, 0);
    }
}

uint8_t LED_Rotate(uint8_t current, uint8_t clockwise)
{
    if (clockwise) {
        return (current + 1) % NUM_LEDS;
    } else {
        return (current == 0) ? (NUM_LEDS - 1) : (current - 1);
    }
}
