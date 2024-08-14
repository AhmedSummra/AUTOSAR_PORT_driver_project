 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Summra
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"
#include "Port_PBcfg.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif
   
STATIC const Port_PinConfigType * Port_PinPtr = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;


/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to initialize Port driver module
************************************************************************************/
void Port_Init(const Port_ConfigType * ConfigPtr )
{
    volatile uint32 * Port_Registers_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;                       /* variable to use in enabling the clock */
    uint8 i;                                         /* variable to use in the for loop for the pins initialization */
    Port_PinPtr=ConfigPtr->Pins;                     /* pointer to Port_PinConfigType structure 
    NOTE:- we pass the name of the configrations array (Pins) to this poiter so it points initially to the first member */
    Port_Status = PORT_INITIALIZED;                  /* set the module status to initialized */
    boolean error = FALSE;                           /* variable to hold the error status */
   
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    
    /* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
	Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_POINTER);
        error = TRUE;
	}
        else 
        {
          /* No action required */
        }
        
   /* Check if the Port number is incorrect */ 
    if(Port_PinPtr->Port >= PortNumberOfPorts)
    {
      Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
      error = TRUE;
    }
    else 
    {
      /* No action required */
    }
    
    /* Check if the Pin number is incorrect */
    if(Port_PinPtr->pin >= PortNumberOfPortPins)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
	PORT_E_PARAM_CONFIG);
        error = TRUE;
    }
    else 
    {
      /* No action required */
    }
    
    /* Check if the Pin direction is incorrect */
    if(Port_PinPtr->direction == PORT_PIN_IN || Port_PinPtr->direction == PORT_PIN_OUT)
    {
      /* Do nothing */
    }
    else 
    {
       Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
       PORT_E_PARAM_CONFIG);
       error = TRUE;
    }
    
    /* Check if the Pin possible mode number is incorrect */
    if(Port_PinPtr->mode > PortNumberOfPossibleModes)
    {
       Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
       PORT_E_PARAM_CONFIG);
       error = TRUE;
    }
    else 
    {
      /* No action required */
    } 
    
    /* Check if the Pin initial value is incorrect (in case of output pin)*/
    if(Port_PinPtr->initial_value == STD_HIGH || Port_PinPtr->initial_value == STD_LOW)
    {
      /* Do nothing */
    }
    else 
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
	PORT_E_PARAM_CONFIG);
        error = TRUE;
    }
    
    /* Check if the selection of the internal resistor is incorrect (in case of input pin)*/
    if(Port_PinPtr->resistor == PULL_UP || Port_PinPtr->resistor == PULL_DOWN || Port_PinPtr->resistor == OFF)
    {
      /* Do nothing */
    }
    else
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
	PORT_E_PARAM_CONFIG);
        error = TRUE; 
    }
    
#endif    
    if(error == FALSE)    /* In case there are no errors */
    {
    for(i=0;i<PortNumberOfPortPins;i++)
    {
    switch(Port_PinPtr->Port)
    {
    case 0: 
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
    case 1:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
    case 2:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
    case 3:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
    case 4:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
    case 5:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PinPtr->Port);
    delay = SYSCTL_REGCGC2_REG;
    
    if( ((Port_PinPtr->Port == 3) && (Port_PinPtr->pin == 31)) || ((Port_PinPtr->Port == 5) && (Port_PinPtr->pin == 38)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PinPtr->pin-8*Port_PinPtr->Port);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }                                                                                                                                         /* Note that (pin number - 8*port number) equals the pin's number in the port */
    else if( (Port_PinPtr->Port == 2) && ((Port_PinPtr->pin - 8*Port_PinPtr->Port) <= 3) ) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    
  if(Port_PinPtr->mode==PortConf_MODE0_DEG_ID||Port_PinPtr->mode==PortConf_MODE0_AN_ID)    /* In case of GPIO mode */
  {
    CLEAR_BIT(*((volatile uint32*)(volatile uint8*)Port_Registers_Ptr+PORT_ALT_FUNC_REG_OFFSET) , Port_PinPtr->pin-8*Port_PinPtr->Port);     /*clear the corresponding bit in the GPIOALSEL to enable GPIO mode */
    *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((Port_PinPtr->pin-8*Port_PinPtr->Port) * 4));  /* clear the corresponding 4 bits in the GPIOPCTL register to enable GPIO mode*/
  }
  else 
  {
   SET_BIT(*((volatile uint32*)(volatile uint8*)Port_Registers_Ptr+PORT_ALT_FUNC_REG_OFFSET) , Port_PinPtr->pin-8*Port_PinPtr->Port);  /* Set the corresponding bit in GPIOALSEL register to enable alternative functions */
   *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((Port_PinPtr->pin-8*Port_PinPtr->Port) * 4));    /* clear the corresponding 4 bits in the GPIOPCTL register */
   *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET)|= (Port_PinPtr->mode << ((Port_PinPtr->pin-8*Port_PinPtr->Port) * 4));   /* Insert the required mode number in the corresponding 4 bits in the GPIOPCTL register*/
  }
  
if (Port_PinPtr->mode==PortConf_MODE0_DEG_ID||Port_PinPtr->mode==PortConf_MODE0_AN_ID)
{
if(Port_PinPtr->direction ==PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port );                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Port_PinPtr->initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DATA_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port );          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DATA_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Port_PinPtr->direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Port_PinPtr->resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_PULL_UP_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Port_PinPtr->resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_PULL_DOWN_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_PULL_UP_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_PULL_DOWN_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    }

if (Port_PinPtr->mode==PortConf_MODE0_AN_ID)
{
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);      /* sets the corresponding bit in the GPIO analog mode enable to enable analog functionality on this pin*/
}   
else  
{
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,Port_PinPtr->pin-8*Port_PinPtr->Port);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */  
}
    
    
Port_PinPtr++;
}
 
}

}

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
#if (PORT_SET_PIN_DIRECTION_API == TRUE)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
  
  volatile uint32 * Port_Registers_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  uint8 Port = 0;                         /* variable to hold port id*/
  boolean error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    
  /* Check for API service called prior to module initialization  */   
  if (PORT_NOT_INITIALIZED == Port_Status)
  {
    Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,
                    PORT_SET_PIN_DIRECTION_SID,PORT_E_UNINIT);
    error = TRUE;
  }
  else
  {
    /* No Action Required */
  }
   
  /* Check for incorrect Port pin id */
  if(Pin >= PortNumberOfPortPins)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
    PORT_E_PARAM_PIN);
    error = TRUE;
  }
  else 
  {
    /* No action required */
  }
  
  /* Check for Port Pin not configured as changeable */
  if (Port_PinPtr[Pin].changeable_direction == STD_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
    PORT_E_DIRECTION_UNCHANGEABLE);
    error = TRUE;
  }
  else 
  {
    /* No action required */
  }

#endif 
  
  if (error == FALSE)
  {
    
  switch(Port_PinPtr[Pin].Port)
    {
    case 0:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      Port = PortConf_PORTA_ID;
		 break;
    case 1:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      Port = PortConf_PORTB_ID;
		 break;
    case 2:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      Port = PortConf_PORTC_ID;
		 break;
    case 3:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      Port = PortConf_PORTD_ID;
		 break;
    case 4:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      Port = PortConf_PORTE_ID;
		 break;
    case 5:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      Port = PortConf_PORTF_ID;
		 break;
    }
  
if(Direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,Pin-8*Port );                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */    
    }
    else if(Direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,Pin-8*Port);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    }
    else
    {
        /* Do Nothing */
    }  
  
  } 
}
#endif

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
void Port_RefreshPortDirection(void)
{
  uint8 i;     /* Variable to use in the for loop */
  uint8 Port = 0;                         /* variable to hold port id*/
  volatile uint32 * Port_Registers_Ptr = NULL_PTR;    /* Pointer to access GPIO registers */
  boolean error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)  
  
  /* Check for API service called prior to module initialization */
  if (PORT_NOT_INITIALIZED == Port_Status)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, 
                    PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);

    error = TRUE;
  }
  else
  {
    /* No Action Required */
  }
   
#endif 
  
  if(error == FALSE)
  {
  for(i=0;i<PortNumberOfPortPins;i++)
  {
      switch(i)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      Port = PortConf_PORTA_ID;
		 break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      Port = PortConf_PORTB_ID;
		 break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      Port = PortConf_PORTC_ID;
		 break;
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      Port = PortConf_PORTD_ID;
		 break;
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      Port = PortConf_PORTE_ID;
		 break;
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      Port = PortConf_PORTF_ID;
		 break;
    }
    
if (Port_PinPtr[i].changeable_direction == STD_OFF)
{
  if(Port_PinPtr[i].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,i-8*Port );                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */    
    }
    else if(Port_PinPtr[i].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_DIR_REG_OFFSET) ,i-8*Port);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    }
    else
    {
        /* Do Nothing */
    }  
  
}
else 
{
  /* No action required */
}
  
  }
  }
}

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
#if (PORT_SET_PIN_MODE_API == TRUE)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
   volatile uint32 * Port_Registers_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  uint8 Port = 0;                         /* variable to hold port id*/
  boolean error = FALSE;
  
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    
  /* Check for API service called prior to module initialization  */    
  if (PORT_NOT_INITIALIZED == Port_Status)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
    PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
    error = TRUE;
  }
  else
  {
    /* No Action Required */
  }
  
  /* Check for incorrect port pin id passed */
   if(Pin >= PortNumberOfPortPins)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
    PORT_E_PARAM_PIN);
    error = TRUE;
  }
  else 
  {
    /* No action required */
  }
  
  /* Check for Port pin mode passed not valid */
  if(Mode > PortNumberOfPossibleModes)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
    PORT_E_PARAM_INVALID_MODE);
    error = TRUE;
  }
  else 
  {
    /* No action required */
  }
  
  /* Check for the API service called when the mode is unchangeable */
  if (Port_PinPtr[Pin].changeable_mode == STD_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
    PORT_E_MODE_UNCHANGEABLE);
    error = TRUE;
  }
  else 
  {
    /* No action required */
  }

#endif  
    
   switch(Pin)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      Port = PortConf_PORTA_ID;
		 break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      Port = PortConf_PORTB_ID;
		 break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      Port = PortConf_PORTC_ID;
		 break;
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      Port = PortConf_PORTD_ID;
		 break;
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      Port = PortConf_PORTE_ID;
		 break;
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
      Port_Registers_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      Port = PortConf_PORTF_ID;
		 break;
    }
  
   if(Mode==PortConf_MODE0_DEG_ID||Mode==PortConf_MODE0_AN_ID)    /* In case of GPIO mode */
  {
    CLEAR_BIT(*((volatile uint32*)(volatile uint8*)Port_Registers_Ptr+PORT_ALT_FUNC_REG_OFFSET) , Pin-8*Port);     /*clear the corresponding bit in the GPIOALSEL to enable GPIO mode */
    *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((Pin-8*Port) * 4));  /* clear the corresponding 4 bits in the GPIOPCTL register to enable GPIO mode*/
  }
  else 
  {
   SET_BIT(*((volatile uint32*)(volatile uint8*)Port_Registers_Ptr+PORT_ALT_FUNC_REG_OFFSET) , Pin-8*Port);  /* Set the corresponding bit in GPIOALSEL register to enable alternative functions */
   *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((Pin-8*Port) * 4));    /* clear the corresponding 4 bits in the GPIOPCTL register */
   *(volatile uint32 *)((volatile uint8 *)Port_Registers_Ptr + PORT_CTL_REG_OFFSET)|= (Mode << ((Pin-8*Port) * 4));   /* Insert the required mode number in the corresponding 4 bits in the GPIOPCTL register*/
  } 
  
}
#endif

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
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
 #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
	}
	else
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
        
}
#endif
