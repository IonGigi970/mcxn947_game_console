/**
 * @file sensors.c
 * @brief Sensor Reading Implementation for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#include "sensors.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_lpadc.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool _isconverting_pot = false;
static bool _isconverting_therm = false;
static bool _isconverting_light = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void Sensor_TriggerConversion(sensor_type_t type)
{
    switch (type) {
        case SENSOR_POTENTIOMETER:
            if (!_isconverting_pot) {
                LPADC_DoSoftwareTrigger(ADC0, 1);
                _isconverting_pot = true;
            }
            break;
        case SENSOR_THERMISTOR:
            if (!_isconverting_therm) {
                LPADC_DoSoftwareTrigger(ADC0, 1 << 1);
                _isconverting_therm = true;
            }
            break;
        case SENSOR_LIGHT:
            if (!_isconverting_light) {
                LPADC_DoSoftwareTrigger(ADC0, 1 << 2);
                _isconverting_light = true;
            }
            break;
    }
}

bool Sensor_GetPotValue(uint16_t *value)
{
    lpadc_conv_result_t result;
    
    if (!_isconverting_pot) {
        Sensor_TriggerConversion(SENSOR_POTENTIOMETER);
    }
    
    if (LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_pot) {
        *value = result.convValue >> 3;
        _isconverting_pot = false;
        
        // Avoid small values that cause constant resets
        if (*value <= 100) {
            *value = 50;
        }
        return true;
    }
    return false;
}

bool Sensor_GetThermistorValue(uint32_t *value)
{
    lpadc_conv_result_t result;
    
    if (!_isconverting_therm) {
        Sensor_TriggerConversion(SENSOR_THERMISTOR);
    }
    
    if (LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_therm) {
        *value = result.convValue;
        _isconverting_therm = false;
        return true;
    }
    return false;
}

bool Sensor_GetLightValue(uint32_t *value)
{
    lpadc_conv_result_t result;
    
    if (!_isconverting_light) {
        Sensor_TriggerConversion(SENSOR_LIGHT);
    }
    
    if (LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_light) {
        *value = result.convValue;
        _isconverting_light = false;
        return true;
    }
    return false;
}

uint32_t Sensor_MapThermistorToMatch(uint32_t therm_value)
{
    uint32_t therm_match;
    
    if (therm_value <= 460) {
        therm_match = 4095;
    } else if (therm_value >= 620) {
        therm_match = 0;
    } else {
        // Map 460-620 to 4095-0
        therm_match = 4095 - ((therm_value - 460) * 4095) / 160;
    }
    
    return therm_match;
}

uint8_t Sensor_ReadDIPSwitch(void)
{
    uint8_t dip1 = GPIO_PinRead(GPIO2, SHIELD_DIP_1_GPIO_PIN);
    uint8_t dip2 = GPIO_PinRead(GPIO1, SHIELD_DIP_2_GPIO_PIN);
    uint8_t dip3 = GPIO_PinRead(GPIO2, SHIELD_DIP_3_GPIO_PIN);
    uint8_t dip4 = GPIO_PinRead(GPIO0, SHIELD_DIP_4_GPIO_PIN);
    uint8_t dip5 = GPIO_PinRead(GPIO1, SHIELD_DIP_5_GPIO_PIN);
    uint8_t dip6 = GPIO_PinRead(GPIO0, SHIELD_DIP_6_GPIO_PIN);
    uint8_t dip7 = GPIO_PinRead(GPIO0, SHIELD_DIP_7_GPIO_PIN);
    uint8_t dip8 = GPIO_PinRead(GPIO0, SHIELD_DIP_8_GPIO_PIN);
    
    return (dip8 << 7) | (dip7 << 6) | (dip6 << 5) | (dip5 << 4) | 
           (dip4 << 3) | (dip3 << 2) | (dip2 << 1) | dip1;
}

uint32_t Sensor_Limit(uint32_t value, uint32_t low, uint32_t high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}
