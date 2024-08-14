 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Summra
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H


/* Id for the company in the AUTOSAR
 * for example Mohamed Tarek's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (120U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)
   
/*not autosar file*/
#include "Common_Macros.h"

/* Standard AUTOSAR types */
#include "Std_Types.h"
   
/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

 /* Dio Pre-Compile Configuration Header file */
#include "Port_Cfg.h"
   
 /* AUTOSAR Version checking between Port_Cfg.h and Dio.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Dio_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Dio.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif
  
  

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/


 /* Description: variable to hold the symbolic name of a port pin*/   
   typedef uint8 Port_PinType;

/* Description: variable to hold the symbolic name of port id */
typedef uint8 Port_PortType;

/* Description: variable to hold pin's initial value */
typedef uint8 Port_PinInitialValueType;

/* Description: variable to hold changeable pin direction option */
typedef uint8 Port_PinDirectionChangeableType;

/* Description: variable to hold changeable pin mode option*/
typedef uint8 Port_PinModeChangeableType;

/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: variable to hold pin mode */
typedef uint8 Port_PinModeType; 

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistorType;

/* Description: Structure to configure each individual PIN:
 *	1. the Pin number (in the form PortConf_PORTnumber_PINnumber_ID)
 *	2. the direction of the pin
 *      3. the mode of the pin (from MODE0 to MODE8)
 *      4. the initial value of the pin (in case of output)
 *      5. the internal resistor option (in case of input)
 *      6. the changeable pin direction option 
 *      7. the changeable pin mode option 
 */
typedef struct 
{
    Port_PortType Port;  
    Port_PinType pin;  
    Port_PinDirectionType direction;
    Port_PinModeType mode;
    Port_PinInitialValueType initial_value;
    Port_InternalResistorType resistor;
    Port_PinDirectionChangeableType changeable_direction;
    Port_PinModeChangeableType changeable_mode;
}Port_PinConfigType;

/* Description: Type of the external data structure containing the initialization data for this module 
*        1. array of individual pin configration structures 
*        (its size equals to the number of configured port pins )
*/
typedef struct
{
  Port_PinConfigType Pins[PortNumberOfPortPins];
}Port_ConfigType;  

/********************************************************************************
                      API SERVICE ID MACROS 
********************************************************************************/
/* Service id for init function */
#define PORT_INIT_SID                               (uint8)0x00

/* Service id for set pin direction function */
#define PORT_SET_PIN_DIRECTION_SID                  (uint8)0x01

/* service id for refresh port direction function*/
#define PORT_REFRESH_PORT_DIRECTION_SID             (uint8)0x02

/* Service id for get version info function */
#define PORT_GET_VERSION_INFO_SID                   (uint8)0x03

/* service id for set pin mode function */
#define PORT_SET_PIN_MODE_SID                       (uint8)0x04


/**************************************************************************************
                            DET ERROR CODES 
**************************************************************************************/
/* DET code to report invalid port pin id reguested */
#define PORT_E_PARAM_PIN                              (uint8)0x0A

/* DET error to report port pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE                 (uint8)0x0B

/* DET error to report API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                           (uint8)0x0C

/* DET error to report API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_PARAM_INVALID_MODE                      (uint8)0x0D
#define PORT_E_MODE_UNCHANGEABLE                       (uint8)0x0E

/* DET error to report API service called without module initialization */
#define PORT_E_UNINIT                                  (uint8)0x0F

/* DET error to report API is called with a NULL pointer */
#define PORT_E_PARAM_POINTER                            (uint8)0x10

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin configuration:
*              - Setup the pin mode 
*              - Setup the direction of the GPIO pin
*              - Setup the internal resistor for i/p pin
*              - setup the pin initial value 
************************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr );


/***********************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port pin ID
*                : Direction - Port Pin Direction 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction 
************************************************************************************/
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction);


/***********************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes Port direction 
************************************************************************************/
void Port_RefreshPortDirection(void);


/***********************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): Versioninfo - Pointer to where to store the version information of this module
* Return value: None
* Description: Returns the version infornation of this module 
***********************************************************************************/
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);


/**************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port pin id number
*                : Mode - New port pin mode to be set on port pin 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode 
***************************************************************************************/
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode);


#endif /* PORT_H */
