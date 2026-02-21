/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include <stdio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MCXN947_cm33_core0.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "oled.h"

//GLOBAL variables
uint8_t direction = 1; 	//clockwise = 1


typedef struct {
	GPIO_Type *gpio;
	uint32_t pin;
} LED_TypeDef_t;

//Define an array of LEDs

LED_TypeDef_t LEDs[] = {
		{GPIO4, SHIELD_LED1_GPIO_PIN},
		{GPIO0, SHIELD_LED2_GPIO_PIN},
		{GPIO0, SHIELD_LED3_GPIO_PIN},
		{GPIO0, SHIELD_LED4_GPIO_PIN},
		{GPIO2, SHIELD_LED5_GPIO_PIN},
		{GPIO2, SHIELD_LED6_GPIO_PIN},
		{GPIO2, SHIELD_LED7_GPIO_PIN},
		{GPIO2, SHIELD_LED8_GPIO_PIN},
};

// Define variables for interrupts
volatile uint32_t sw1_int_flag = 0;
volatile uint32_t sw2_int_flag = 0;
volatile uint32_t sensor_flag = 1;
volatile uint32_t last_sensor_flag = 1; 
volatile uint32_t encoder = 0;
volatile uint32_t joystick_up = 0;
volatile uint32_t joystick_down = 0;
volatile uint32_t joystick_left = 0;
volatile uint32_t joystick_right = 0;
volatile uint32_t joystick_button = 0;
volatile int8_t counter = 0;
volatile uint8_t last_rot_a = 0;
volatile uint32_t last_dip_value = 0xFFFF; 
volatile uint8_t ui_mode = 0; // 0 - menu | 1 - pot | 2 - temp/light | 3 - encoder | 4 - joystick | 255 - invalid

bool _isconverting_pot = false;
bool _isconverting_therm = false;
bool _isconverting_light = false;


/*
 * Interrupt Handler Definition
 */
/* GPIO40_IRQn interrupt handler */
void GPIO4_INT_0_IRQHANDLER(void)
{
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO4, 0U);

  /* Place your interrupt code here */
  sw1_int_flag = 1;

  //direction = !direction; //change direction

  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO4, pin_flags0, 0U);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/* GPIO30_IRQn interrupt handler */
void GPIO3_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO3, 0U);

  /* Place your interrupt code here */
	sensor_flag = !sensor_flag; //SW2
	joystick_up = 1;

	_isconverting_therm = false;
    _isconverting_light = false;
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO3, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}



/* GPIO31_IRQn interrupt handler */
void GPIO3_INT_1_IRQHANDLER(void) {
  /* Get pin flags 1 */
  uint32_t pin_flags1 = GPIO_GpioGetInterruptChannelFlags(GPIO3, 1U);

  /* Place your interrupt code here */

    uint8_t A = GPIO_PinRead(GPIO3, SHIELD_ROTARY_1_GPIO_PIN);
    uint8_t B = GPIO_PinRead(GPIO3, SHIELD_ROTARY_2_GPIO_PIN);

	if (A != last_rot_a) 
    {
        if (B != A) 
		{
			counter++;
		}
        else 
		{
			counter--;
		}

		last_rot_a = A;
    }

	encoder = 1; //SW4
	joystick_left = 1;
  /* Clear pin flags 1 */
  GPIO_GpioClearInterruptChannelFlags(GPIO3, pin_flags1, 1U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/* GPIO00_IRQn interrupt handler */
void GPIO0_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO0, 0U);

  /* Place your interrupt code here */
	joystick_down = 1; 
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO0, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/* GPIO10_IRQn interrupt handler */
void GPIO1_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO1, 0U);

  /* Place your interrupt code here */
	joystick_right = 1;
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO1, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/* GPIO20_IRQn interrupt handler */
void GPIO2_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO2, 0U);

  /* Place your interrupt code here */
	joystick_button = 1;
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO2, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


void ctimer_match_callback(uint32_t flags)
{
	sw2_int_flag = 1; //set timer flag
}

void clearAllShieldLEDs(const LED_TypeDef_t *leds, uint8_t count)
{
    for (uint8_t i = 0; i < count; ++i) {
        GPIO_PinWrite(leds[i].gpio, leds[i].pin, 0);
    }
}

static inline uint32_t limit(uint32_t value, uint32_t low, uint32_t high) {

    if (value < low) 
	{
		return low;
	}

    if (value > high) 
	{
		return high;
	}

    return value;
}



/*
 * @brief   Application entry point.
 */
int main(void) {

    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

	

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    // Init FSL debug console
    BOARD_InitDebugConsole();
#endif
    //Init board and shield LEDs and buttons
    BOARD_InitLEDsPins();		//Initialise the on-board LEDs
    BOARD_InitBUTTONsPins();	//Initialise the on-board buttons
    SHIELD_InitLEDsPins();		//initialise the shield LEDs
    SHIELD_InitBUTTONsPins();	//Initialise the shield buttons
	SHIELD_RotaryPins();		//Initialise the rotary encoder pins
	SHIELD_NAVSwitchPins();		//Initialise the joystick pins
	SHIELD_DIPSwitchPins();		//Initialise the DIP switch pins

	/* Attach FROM 12M to FLEXCOMM2 for I2C */
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    initOLED();

    //printfOLED("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec feugiat arcu quis rutrum elementum. Duis ultricies euismod velit in posuere. Proin convallis non nisl in.");
    sendOLED(frame, 1024, OLED_DATA);

	SDK_DelayAtLeastUs(2000000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	fillOLED(0);
   
    uint8_t current_led = 0; //LEDs are zero indexed so LED1 = 0, LED2 = 1 etc.
	uint8_t old_led = 0;  //to store all LED and turn off when updated current LED
    const uint8_t num_leds = sizeof(LEDs) / sizeof(LED_TypeDef_t); //Calculate the number of LEDs

    ctimer_match_config_t matchConfig = CTIMER0_Match_0_config;	//timer match config, will be initialised with the values set in MCUXpresso Config Tools

	
	 /*	CTimer Match Config */ // This section is commented because matchConfig will be initialised with CTIMER0_Match_0_config values
	//  matchConfig.enableCounterReset = true;
	//  matchConfig.enableCounterStop  = false;
	//  matchConfig.matchValue         = CTIMER0_TICK_FREQ / 2;  //Initial value
	//  matchConfig.outControl         = kCTIMER_Output_Toggle;
	//  matchConfig.outPinInitState    = false;
	//  matchConfig.enableInterrupt    = true;

	//CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_0_CHANNEL, &matchConfig);	//set timer configuration - commented as it will have the same values as in CTIMER0_init
	CTIMER_StartTimer(CTIMER0_PERIPHERAL);	//Start timer

	while(1)
	{      
		lpadc_conv_result_t result;

		// Read every DIP switch state and concatenate to a single byte
		volatile uint8_t dip1 = GPIO_PinRead(GPIO2, SHIELD_DIP_1_GPIO_PIN);
		volatile uint8_t dip2 = GPIO_PinRead(GPIO1, SHIELD_DIP_2_GPIO_PIN);
		volatile uint8_t dip3 = GPIO_PinRead(GPIO2, SHIELD_DIP_3_GPIO_PIN);
		volatile uint8_t dip4 = GPIO_PinRead(GPIO0, SHIELD_DIP_4_GPIO_PIN);
		volatile uint8_t dip5 = GPIO_PinRead(GPIO1, SHIELD_DIP_5_GPIO_PIN);
		volatile uint8_t dip6 = GPIO_PinRead(GPIO0, SHIELD_DIP_6_GPIO_PIN);
		volatile uint8_t dip7 = GPIO_PinRead(GPIO0, SHIELD_DIP_7_GPIO_PIN);
		volatile uint8_t dip8 = GPIO_PinRead(GPIO0, SHIELD_DIP_8_GPIO_PIN);
		volatile uint32_t dip_value = (dip8 << 7) | (dip7 << 6) | (dip6 << 5) | (dip5 << 4) | (dip4 << 3) | (dip3 << 2) | (dip2 << 1) | dip1;

		if (dip_value != last_dip_value) 
		{
			switch(dip_value)
			{
				case 0b11111111:
				{
					ui_mode = 0; 
					break;
				} 

				case 0b11111110:
				{
					ui_mode = 1; 
					break;
				}

				case 0b11111101:
				{
					ui_mode = 2; 
					break;
				}

				case 0b11110111:
				{
					ui_mode = 3; 
					break;
				}

				case 0b01111111:
				{
					ui_mode = 4; 
					break;
				}

				default:
				{
					ui_mode = 255; 
					break;
				}
			}

			fillOLED(0);
			clearAllShieldLEDs(LEDs, num_leds);
			old_led = 0;
			current_led = 0;
			
			switch(ui_mode)
			{
				case 0:
				{
					writeString("Select an option:", 1, 0, 0);
					writeString("1. Potentiometer", 1, 0, 1);
					writeString("2. Temp/Light", 1, 0, 2);
					writeString("3. Encoder", 1, 0, 3);
					writeString("4. Joystick", 1, 0, 4);
					break;
				}
				
				case 1:
				{
					writeString("Potentiometer mode", 1, 0, 0);
					writeString("SW1: direction", 1, 0, 1);
					writeString("Reset DIP->menu", 1, 0, 2);
					break;
				}
				
				case 2:
				{	
					writeString("Temp/Light mode", 1, 0, 0);
					writeString("SW1: direction", 1, 0, 1);
					writeString("Reset DIP->menu", 1, 0, 2);

					_isconverting_pot = false;
					_isconverting_therm = false;
					_isconverting_light = false;
					break;
				}

				case 3:
				{
					writeString("Encoder mode", 1, 0, 0);
					writeString("Reset DIP->menu", 1, 0, 1);
					break;
				}

				case 4:
				{
					writeString("Joystick mode", 1, 0, 0);
					writeString("Reset DIP->menu", 1, 0, 1);
					break;
				}
				
				default:
				{
					writeString("Invalid DIP combo", 1, 0, 0);
					writeString("All OFF -> menu", 1, 0, 1);
					break;
				}
			}	
			last_dip_value = dip_value;
		}

		switch(dip_value)
		{
			case 0b11111110: //DIP 1 ON
			{	
				uint16_t pot_value = 0;
				
				if(sw1_int_flag == 1)
				{
					direction = !direction; //change direction
					sw1_int_flag = 0;	// clear interrupt flag
				}

				if(ui_mode == 1 && sw2_int_flag == 1)
				{
					if (!_isconverting_pot)
					{
						// Trigger conversion
						LPADC_DoSoftwareTrigger(ADC0, 1);
						_isconverting_pot = true;
					}

					if(LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_pot)
					{
						// result.convValue is 16-bit raw ADC value
						pot_value = result.convValue>>3;
						_isconverting_pot = false;  


						if (pot_value <= 100)    //Avoid a small pot value to prevent constant timer reset
						{
							pot_value = 50;
						}
						
						uint32_t newMatchValue = pot_value << 12;   
						if (newMatchValue < 400) newMatchValue = 400;
						if (newMatchValue > 18000000) newMatchValue = 18000000;

						matchConfig.matchValue = newMatchValue;	//shift for better LED blink visibility
						CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_0_CHANNEL, &matchConfig);	//set new timer config with pot value as count
						CTIMER0_PERIPHERAL->TC = 0;

						GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
						GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);

						writeString("Pot:", 1, 0, 4);
						printVar("%4d", pot_value, 1, 40, 4);

						if(direction)
						{
							old_led = current_led;
							current_led = (current_led + 1) % num_leds;
						}
						else
						{
							old_led = current_led;
							current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
						}
							sw2_int_flag = 0;	
					}
				}
				break;
			}

			case 0b11111101: //DIP 2 ON
			{				
				if(sw1_int_flag == 1)
				{
					direction = !direction; //change direction
					sw1_int_flag = 0;	// clear interrupt flag
				}
				
				// command 1 + trigger 0 -> pot
				// command 2 + trigger 1 -> thermistor
				// command 3 + trigger 2 -> light sensor
				
				if(ui_mode == 2 && sw2_int_flag == 1)
				{	
					uint32_t thermistor_value = 0;
					uint32_t light_value = 0;

					if(sensor_flag == 1) // Thermistor
					{	
						if (!_isconverting_therm){
							// Trigger conversion
							LPADC_DoSoftwareTrigger(ADC0, 1 << 1);
							_isconverting_therm = true;
						}
						
						// non blocking read. ADC result will be read when conversion is finished
						if(LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_therm)
						{
							// result.convValue is 16-bit raw ADC value
							thermistor_value = result.convValue;
							_isconverting_therm = false;  

							uint32_t therm_match;

							if (thermistor_value <= 460) 
							{
								therm_match = 4095;
							} 
							else if (thermistor_value >= 620) 
							{
								therm_match = 0;
							} 
							else 
							{
								therm_match = 4095 - ((thermistor_value - 460) * 4095) / 160; //map 460-620 to 4095-0
							}

							uint32_t newMatchValue = therm_match << 6;           
							newMatchValue = limit(newMatchValue, 400, 280000);

							matchConfig.matchValue = newMatchValue;	//shift for better LED blink visibility
							CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_0_CHANNEL, &matchConfig);	//set new timer config with pot value as count
							CTIMER0_PERIPHERAL->TC = 0;

							GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
							GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);

							writeString("Temp: ", 1, 0, 4);
							printVar("%3d", thermistor_value, 1, 40, 4);
							writeString("      ", 1, 58, 4);

							if(direction)
							{
								old_led = current_led;
								current_led = (current_led + 1) % num_leds;
							}
							else
							{
								old_led = current_led;
								current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
							}
								sw2_int_flag = 0;		//clear interrupt flag
						}

					}
					else 
					{
						if (!_isconverting_light)
						{
							// Trigger conversion
							LPADC_DoSoftwareTrigger(ADC0, 1 << 2);
							_isconverting_light = true;
						}
						
						if(LPADC_GetConvResult(ADC0, &result, 0) && _isconverting_light)
						{

							light_value = result.convValue;
							_isconverting_light = false;  

							uint32_t newMatchValue = light_value << 8	; 
							newMatchValue = limit(newMatchValue, 400, 6000000);

							matchConfig.matchValue = newMatchValue;	//shift for better LED blink visibility
							CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_0_CHANNEL, &matchConfig);	//set new timer config with pot value as count
							CTIMER0_PERIPHERAL->TC = 0;

							GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
							GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);
							
							writeString("Light:", 1, 0, 4);
							printVar("%6d", light_value, 1, 40, 4);							

							if(direction)
							{
								old_led = current_led;
								current_led = (current_led + 1) % num_leds;
							}
							else
							{
								old_led = current_led;
								current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
							}
								sw2_int_flag = 0;		//clear interrupt flag
						}
					}
				}
				break;
			}

			case 0b11110111: //DIP 4 ON 
			{
				while (counter != 0)
				{
					int8_t step = (counter > 0) ? 1 : -1;
					counter -= step;

					direction = (step > 0) ? 0 : 1;

					old_led = current_led;
					if (direction)
						current_led = (current_led + 1) % num_leds;
					else
						current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);

					GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
					GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);
				}

				encoder = 0;
				break;
			}
			
			case 0b01111111: // DIP 8 ON
			{
				if (joystick_up == 1) 
				{
					GPIO_PinWrite(LEDs[0].gpio, LEDs[0].pin, 1);
					GPIO_PinWrite(LEDs[0].gpio, LEDs[0].pin, 0);
					joystick_up = 0;
				} 
				else if (joystick_down == 1) 
				{
					GPIO_PinWrite(LEDs[4].gpio, LEDs[4].pin, 1);
					GPIO_PinWrite(LEDs[4].gpio, LEDs[4].pin, 0);
					joystick_down = 0;
				} 
				else if (joystick_left == 1) 
				{
					GPIO_PinWrite(LEDs[6].gpio, LEDs[6].pin, 1);
					GPIO_PinWrite(LEDs[6].gpio, LEDs[6].pin, 0);
					joystick_left = 0;
				} 
				else if (joystick_right == 1) 
				{
					GPIO_PinWrite(LEDs[2].gpio, LEDs[2].pin, 1);
					GPIO_PinWrite(LEDs[2].gpio, LEDs[2].pin, 0);
					joystick_right = 0;
				} 
				else if (joystick_button == 1) 
				{	
					// Do nothing
					clearAllShieldLEDs(LEDs, num_leds);
					joystick_button = 0;
				}
				break;
			}
    	}
	}
	return 0;
}
