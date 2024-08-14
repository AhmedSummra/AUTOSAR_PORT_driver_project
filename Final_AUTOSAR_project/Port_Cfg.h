/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Ahmed Summra
 ******************************************************************************/
 
 #ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)


/* The number of specified PortPins */ 
#define PortNumberOfPortPins                  (43)
   
/* Number of specified Ports */
#define PortNumberOfPorts                     (6)  
   
/* Number of possible modes */   
#define PortNumberOfPossibleModes             (15)
   
/* pre-compile option for development error detection */
#define PORT_DEV_ERROR_DETECT                 (STD_ON)

/* pre-compile option for presence of set pin direction API*/
#define PORT_SET_PIN_DIRECTION_API             (TRUE)

/* pre-compile option for presence of set pin mode API*/
#define PORT_SET_PIN_MODE_API                   (TRUE)

/* pre-compile option for presence of version info API*/
#define PORT_VERSION_INFO_API                    (STD_ON)

/* configured ports IDs */
#define PortConf_PORTA_ID                               (0)
#define PortConf_PORTB_ID                               (1)
#define PortConf_PORTC_ID                               (2)
#define PortConf_PORTD_ID                               (3)
#define PortConf_PORTE_ID                               (4)
#define PortConf_PORTF_ID                               (5)

/* configured Port pins IDs */
#define PortConf_PORTA_PIN0_ID                          (0)
#define PortConf_PORTA_PIN1_ID                          (1)
#define PortConf_PORTA_PIN2_ID                          (2)
#define PortConf_PORTA_PIN3_ID                          (3)
#define PortConf_PORTA_PIN4_ID                          (4)
#define PortConf_PORTA_PIN5_ID                          (5)
#define PortConf_PORTA_PIN6_ID                          (6)
#define PortConf_PORTA_PIN7_ID                          (7)
   
#define PortConf_PORTB_PIN0_ID                          (8)
#define PortConf_PORTB_PIN1_ID                          (9)
#define PortConf_PORTB_PIN2_ID                          (10)
#define PortConf_PORTB_PIN3_ID                          (11)
#define PortConf_PORTB_PIN4_ID                          (12)
#define PortConf_PORTB_PIN5_ID                          (13)
#define PortConf_PORTB_PIN6_ID                          (14)
#define PortConf_PORTB_PIN7_ID                          (15)
  
#define PortConf_PORTC_PIN0_ID                          (16)
#define PortConf_PORTC_PIN1_ID                          (17)
#define PortConf_PORTC_PIN2_ID                          (18)
#define PortConf_PORTC_PIN3_ID                          (19)
#define PortConf_PORTC_PIN4_ID                          (20)
#define PortConf_PORTC_PIN5_ID                          (21)
#define PortConf_PORTC_PIN6_ID                          (22)
#define PortConf_PORTC_PIN7_ID                          (23)
   
#define PortConf_PORTD_PIN0_ID                          (24)
#define PortConf_PORTD_PIN1_ID                          (25)
#define PortConf_PORTD_PIN2_ID                          (26)
#define PortConf_PORTD_PIN3_ID                          (27)
#define PortConf_PORTD_PIN4_ID                          (28)
#define PortConf_PORTD_PIN5_ID                          (29)
#define PortConf_PORTD_PIN6_ID                          (30)
#define PortConf_PORTD_PIN7_ID                          (31)
   
#define PortConf_PORTE_PIN0_ID                          (32)
#define PortConf_PORTE_PIN1_ID                          (33)
#define PortConf_PORTE_PIN2_ID                          (34)
#define PortConf_PORTE_PIN3_ID                          (35)
#define PortConf_PORTE_PIN4_ID                          (36)
#define PortConf_PORTE_PIN5_ID                          (37)
   
#define PortConf_PORTF_PIN0_ID                          (38)
#define PortConf_PORTF_PIN1_ID                          (39)
#define PortConf_PORTF_PIN2_ID                          (40)
#define PortConf_PORTF_PIN3_ID                          (41)
#define PortConf_PORTF_PIN4_ID                          (42)


/* different modes definitions */
#define PortConf_MODE0_DEG_ID                     (0)
#define PortConf_MODE0_AN_ID                      (15)
#define PortConf_MODE1_ID                         (1)
#define PortConf_MODE2_ID                         (2)
#define PortConf_MODE3_ID                         (3)
#define PortConf_MODE4_ID                         (4)
#define PortConf_MODE5_ID                         (5)
#define PortConf_MODE6_ID                         (6)
#define PortConf_MODE7_ID                         (7)
#define PortConf_MODE8_ID                         (8)
#define PortConf_MODE9_ID                         (9)
#define PortConf_MODE14_ID                        (14)


#endif
