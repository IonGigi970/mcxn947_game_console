/**
 * @file menu.h
 * @brief OLED Menu System for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum {
    MODE_MENU = 0,
    MODE_POTENTIOMETER = 1,
    MODE_TEMP_LIGHT = 2,
    MODE_ENCODER = 3,
    MODE_JOYSTICK = 4,
    MODE_INVALID = 255
} ui_mode_t;

/*******************************************************************************
 * API Functions
 ******************************************************************************/

/**
 * @brief Display main menu on OLED
 */
void Menu_ShowMain(void);

/**
 * @brief Display potentiometer mode screen
 */
void Menu_ShowPotentiometerMode(void);

/**
 * @brief Display temperature/light mode screen
 */
void Menu_ShowTempLightMode(void);

/**
 * @brief Display encoder mode screen
 */
void Menu_ShowEncoderMode(void);

/**
 * @brief Display joystick mode screen
 */
void Menu_ShowJoystickMode(void);

/**
 * @brief Display invalid mode screen
 */
void Menu_ShowInvalidMode(void);

/**
 * @brief Update display based on current mode
 * @param mode Current UI mode
 */
void Menu_UpdateDisplay(ui_mode_t mode);

/**
 * @brief Display sensor value on screen
 * @param label Label text (e.g., "Pot:", "Temp:", "Light:")
 * @param value Value to display
 */
void Menu_DisplaySensorValue(const char* label, uint32_t value);

/**
 * @brief Convert DIP switch value to UI mode
 * @param dip_value 8-bit DIP switch state
 * @return Corresponding UI mode
 */
ui_mode_t Menu_GetModeFromDIP(uint8_t dip_value);

#endif /* MENU_H_ */
