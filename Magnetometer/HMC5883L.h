#define __hmc5883l_H__

#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_PATH "/dev/i2c-1"

// register addresses
#define hmc5883l_CONFIG_REG_A		0x0
#define hmc5883l_CONFIG_REG_B		0x1
#define Mode_Register				0x2
#define Data_Output_X_MSB_Reg		0x3
#define Data_Output_X_LSB_Reg		0x4
#define Data_Output_Z_MSB_Reg		0x5
#define Data_Output_Z_LSB_Reg		0x6
#define Data_Output_Y_MSB_Reg		0x7
#define Data_Output_Y_LSB_Reg		0x8
#define Status_Register				0x9
#define Identification_Register_A	0xA
#define Identification_Register_B	0xB
#define Identification_Register_C	0xC

// write/read vales
#define hmc5883l_write_val				0x3C
#define hmc5883l_read_val				0x3D

char hmc5883l_read_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value);
char hmc5883l_write_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value);
char hmc5883l_init(unsigned char addr);
char hmc5883l_readMag(unsigned char addr, unsigned char *mag);
char getMag(unsigned char addr, short int *magnetometer);
