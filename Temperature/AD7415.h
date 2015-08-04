/*
 * AD7415.h - header file for the ad7415 temperature sensor.
 *
 */

#define __AD7415_H__

#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_PATH "/dev/i2c-0"

// register addresses
#define AD7415_TEMP_REG 0x0
#define AD7415_CONFIG_REG 0x1

#define AD7415_PNTR_REG 1
#define AD7415_CONFIG_REG_SIZE 1
#define AD7415_TEMP_REG_SIZE 2

#define AD7415_CONFIG_BASE 0x0
#define AD7415_ONESHOT_VAL 0x4
#define AD7415_FILTER_VAL 0x40
#define AD7415_POWERDOWN_VAL 0x80

#define AD7415_ONESHOT_ON 1
#define AD7415_ONESHOT_OFF 0
#define AD7415_FILTER_ON 1
#define AD7415_FILTER_OFF 0
#define AD7415_POWERDOWN_ON 1
#define AD7415_POWERDOWN_OFF 0

char ad7415_read_reg(unsigned char slaveAddr, unsigned char regAddr, 
                     unsigned char *value);
char ad7415_write_reg(unsigned char slaveAddr, unsigned char regAddr, 
                      unsigned char *value);
char ad7415_init(unsigned char addr, unsigned char oneshot,
                 unsigned char filter, unsigned char powerdown);
char ad7415_readTemp(unsigned char addr, unsigned char *temp);
double getTemp(unsigned char addr);
