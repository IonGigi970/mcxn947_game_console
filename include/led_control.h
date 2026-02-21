/**
 * @file led_control.h
 * @brief LED Ring Control Module for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include <stdint.h>
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief LED Pin Definition Structure
 */
typedef struct {
    GPIO_Type *gpio;
    uint32_t pin;
} LED_TypeDef_t;

/*******************************************************************************
 * API Functions
 ******************************************************************************/

/**
 * @brief Get pointer to LED array
 * @return Pointer to LED definitions array
 */
LED_TypeDef_t* LED_GetArray(void);

/**
 * @brief Get number of LEDs
 * @return Number of LEDs in the ring
 */
uint8_t LED_GetCount(void);

/**
 * @brief Turn on a specific LED
 * @param led_index Index of LED to turn on (0-7)
 */
void LED_On(uint8_t led_index);

/**
 * @brief Turn off a specific LED
 * @param led_index Index of LED to turn off (0-7)
 */
void LED_Off(uint8_t led_index);

/**
 * @brief Turn off all LEDs
 */
void LED_ClearAll(void);

/**
 * @brief Rotate LED to next position
 * @param current Current LED index
 * @param clockwise Direction (true = clockwise, false = counter-clockwise)
 * @return New LED index
 */
uint8_t LED_Rotate(uint8_t current, uint8_t clockwise);

#endif /* LED_CONTROL_H_ */
