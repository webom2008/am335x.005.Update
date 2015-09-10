#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#define CONFIG_TARGET_FOR_ARM
//#define CONFIG_TARGET_FOR_PC

//#define CONFIG_RELEASE
#define CONFIG_DEBUG


#if defined(CONFIG_TARGET_FOR_ARM) && defined(CONFIG_TARGET_FOR_PC)
#ERROR: Only one of FOR_AM335X or FOR_X86 can define
#endif

#if defined(CONFIG_TARGET_FOR_ARM) && defined(CONFIG_TARGET_FOR_PC)
#ERROR: Only one of FOR_AM335X or FOR_X86 can define
#endif

/****************Module Address*******************************/
#define MPU_ADDR                    0xAA
#define AIO_ADDR                    0x55
#define KEY_ADDR                    0xEF
#define BCK_ADDR                    0xEE
#define RECORDER_ADDR               0XBB

/****************Module Serial********************************/
#ifdef CONFIG_TARGET_FOR_PC
#define AIO_PORT                    "/dev/ttyUSB0"
#define KEY_PORT                    "/dev/ttyUSB1"
#define BCK_PORT                    "/dev/ttyUSB20"
#define RECORDER_PORT               "/dev/ttyUSB30"
#endif
#ifdef CONFIG_TARGET_FOR_ARM
#define AIO_PORT                    "/dev/ttyO4"
#define KEY_PORT                    "/dev/ttyO5"
#define BCK_PORT                    "/dev/ttyO1"
#define RECORDER_PORT               "/dev/ttyO0"
#endif


/****************Time Length Of Command Response**************/
#define AIO_RESPONSE_TIMEOUT            1000
#define KEY_RESPONSE_TIMEOUT            1000
#define BCK_RESPONSE_TIMEOUT            1000
#define RECORDER_RESPONSE_TIMEOUT       1000

#define COMM_FAULT_INTERVAL             5000//5S

#define AM335X_MLO_LENGTH_MAX           0x00020000
#define AM335X_UBOOT_LENGTH_MAX         0x000c0000
#define AM335X_KERNEL_LENGTH_MAX        0x00500000
#define AM335X_ROOTFS_LENGTH_MAX        0x04000000
#define AM335X_USERFS_LENGTH_MAX        0x0BB00000

#endif // SYSCONFIG_H
