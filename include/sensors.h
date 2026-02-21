/**
 * @file sensors.h
 * @brief Sensor Reading Module for MCXN947 Game Console
 * @author User
 * @date 2025
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum {
    SENSOR_POTENTIOMETER = 0,
    SENSOR_THERMISTOR,
    SENSOR_LIGHT
} sensor_type_t;

/*******************************************************************************
 * API Functions
 ******************************************************************************/

/**
 * @brief Trigger ADC conversion for a specific sensor
 * @param type Type of sensor to read
 */
void Sensor_TriggerConversion(sensor_type_t type);

/**
 * @brief Get ADC result for potentiometer
 * @param value Pointer to store the result
 * @return true if conversion complete
 */
bool Sensor_GetPotValue(uint16_t *value);

/**
 * @brief Get ADC result for thermistor
 * @param value Pointer to store the result
 * @return true if conversion complete
 */
bool Sensor_GetThermistorValue(uint32_t *value);

/**
 * @brief Get ADC result for light sensor
 * @param value Pointer to store the result
 * @return true if conversion complete
 */
bool Sensor_GetLightValue(uint32_t *value);

/**
 * @brief Map thermistor value to match value for timer
 * @param therm_value Raw thermistor ADC value
 * @return Mapped value for CTimer match
 */
uint32_t Sensor_MapThermistorToMatch(uint32_t therm_value);

/**
 * @brief Read DIP switch state
 * @return 8-bit value representing DIP switch positions
 */
uint8_t Sensor_ReadDIPSwitch(void);

/**
 * @brief Limit value to a range
 * @param value Input value
 * @param low Lower bound
 * @param high Upper bound
 * @return Limited value
 */
uint32_t Sensor_Limit(uint32_t value, uint32_t low, uint32_t high);

#endif /* SENSORS_H_ */
