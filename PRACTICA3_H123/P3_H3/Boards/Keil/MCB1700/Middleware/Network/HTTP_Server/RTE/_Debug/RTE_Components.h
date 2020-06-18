
/*
 * Auto generated Run-Time-Environment Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'HTTP_Server' 
 * Target:  'Debug' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "LPC17xx.h"

/*  ARM::CMSIS:RTOS2:Keil RTX5:Source:5.5.1 */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_RTX5            /* CMSIS-RTOS2 Keil RTX5 */
        #define RTE_CMSIS_RTOS2_RTX5_SOURCE     /* CMSIS-RTOS2 Keil RTX5 Source */
/*  Keil.ARM Compiler::Compiler:Event Recorder:DAP:1.4.0 */
#define RTE_Compiler_EventRecorder
          #define RTE_Compiler_EventRecorder_DAP
/*  Keil.MDK-Pro::Network:CORE:IPv4/IPv6 Debug:7.12.0 */
#define RTE_Network_Core                /* Network Core */
          #define RTE_Network_IPv4                /* Network IPv4 Stack */
          #define RTE_Network_IPv6                /* Network IPv6 Stack */
          #define RTE_Network_Debug               /* Network Debug Version */
/*  Keil.MDK-Pro::Network:Interface:ETH:7.12.0 */
#define RTE_Network_Interface_ETH_0     /* Network Interface ETH 0 */

/*  Keil.MDK-Pro::Network:Service:Web Server Compact:HTTP:7.12.0 */
#define RTE_Network_Web_Server_RO       /* Network Web Server with Read-only Web Resources */
/*  Keil.MDK-Pro::Network:Socket:TCP:7.12.0 */
#define RTE_Network_Socket_TCP          /* Network Socket TCP */
/*  Keil.MDK-Pro::Network:Socket:UDP:7.12.0 */
#define RTE_Network_Socket_UDP          /* Network Socket UDP */
/*  Keil::CMSIS Driver:Ethernet MAC:2.12.0 */
#define RTE_Drivers_ETH_MAC0            /* Driver ETH_MAC0 */
/*  Keil::CMSIS Driver:Ethernet PHY:DP83848C:6.2.0 */
#define RTE_Drivers_PHY_DP83848C        /* Driver PHY DP83848C */
/*  Keil::CMSIS Driver:SPI:SSP:2.7.0 */
#define RTE_Drivers_SPI0                /* Driver SPI0 */
        #define RTE_Drivers_SPI1                /* Driver SPI1 */
/*  Keil::Device:Startup:1.0.0 */
#define RTE_DEVICE_STARTUP_LPC17XX      /* Device Startup for NXP17XX */


#endif /* RTE_COMPONENTS_H */
