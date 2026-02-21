/**
 * @file menu.c
 * @brief OLED Menu System Implementation for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#include "menu.h"
#include "oled.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void Menu_ShowMain(void)
{
    fillOLED(0);
    writeString("=== GAME CONSOLE ===", 1, 0, 0);
    writeString("1. Potentiometer", 1, 0, 1);
    writeString("2. Temp/Light", 1, 0, 2);
    writeString("3. Encoder", 1, 0, 3);
    writeString("4. Joystick", 1, 0, 4);
    writeString("Use DIP switch", 1, 0, 6);
}

void Menu_ShowPotentiometerMode(void)
{
    fillOLED(0);
    writeString("POTENTIOMETER MODE", 1, 0, 0);
    writeString("SW1: direction", 1, 0, 1);
    writeString("Reset DIP->menu", 1, 0, 2);
}

void Menu_ShowTempLightMode(void)
{
    fillOLED(0);
    writeString("TEMP/LIGHT MODE", 1, 0, 0);
    writeString("SW1: direction", 1, 0, 1);
    writeString("SW2: toggle sensor", 1, 0, 2);
    writeString("Reset DIP->menu", 1, 0, 3);
}

void Menu_ShowEncoderMode(void)
{
    fillOLED(0);
    writeString("ENCODER MODE", 1, 0, 0);
    writeString("Rotate to select LED", 1, 0, 1);
    writeString("Reset DIP->menu", 1, 0, 2);
}

void Menu_ShowJoystickMode(void)
{
    fillOLED(0);
    writeString("JOYSTICK MODE", 1, 0, 0);
    writeString("Move to light LEDs", 1, 0, 1);
    writeString("Reset DIP->menu", 1, 0, 2);
}

void Menu_ShowInvalidMode(void)
{
    fillOLED(0);
    writeString("INVALID DIP COMBO", 1, 0, 0);
    writeString("All OFF -> menu", 1, 0, 1);
}

void Menu_UpdateDisplay(ui_mode_t mode)
{
    switch (mode) {
        case MODE_MENU:
            Menu_ShowMain();
            break;
        case MODE_POTENTIOMETER:
            Menu_ShowPotentiometerMode();
            break;
        case MODE_TEMP_LIGHT:
            Menu_ShowTempLightMode();
            break;
        case MODE_ENCODER:
            Menu_ShowEncoderMode();
            break;
        case MODE_JOYSTICK:
            Menu_ShowJoystickMode();
            break;
        default:
            Menu_ShowInvalidMode();
            break;
    }
}

void Menu_DisplaySensorValue(const char* label, uint32_t value)
{
    writeString((char*)label, 1, 0, 4);
    printVar("%6d", value, 1, 40, 4);
}

ui_mode_t Menu_GetModeFromDIP(uint8_t dip_value)
{
    switch (dip_value) {
        case 0b11111111:
            return MODE_MENU;
        case 0b11111110:
            return MODE_POTENTIOMETER;
        case 0b11111101:
            return MODE_TEMP_LIGHT;
        case 0b11110111:
            return MODE_ENCODER;
        case 0b01111111:
            return MODE_JOYSTICK;
        default:
            return MODE_INVALID;
    }
}
