/*
 * l3g4200d.h - header file for the l3g4200d gyro sensor.
 *
 * 
 * 
 */

#define __l3g4200d_H__

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
#define l3g4200d_GYRO_REG1 0x20
#define l3g4200d_GYRO_REG2 0x21
#define l3g4200d_GYRO_REG3 0x22
#define l3g4200d_GYRO_REG4 0x23
#define l3g4200d_GYRO_REG5 0x24
#define l3g4200d_STATUS_REG 0x27

#define l3g4200d_OUT_X_L 0x28
#define l3g4200d_OUT_X_H 0x29
#define l3g4200d_OUT_Y_L 0x2A
#define l3g4200d_OUT_Y_H 0x2B
#define l3g4200d_OUT_Z_L 0x2C
#define l3g4200d_OUT_Z_H 0x2D

char l3g4200d_read_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value);
char l3g4200d_write_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value);
char l3g4200d_init(unsigned char addr, double *ptr);
char l3g4200d_sleep(unsigned char addr);
char l3g4200d_readGyro(unsigned char addr, unsigned char *gyro);
char getGyro(unsigned char addr, double *gyro);
