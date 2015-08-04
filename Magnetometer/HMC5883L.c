/**
 *
 *
 * 
 *
 * 
 * 
 */

#include "HMC5883L.h"

/*
 * Lower layer function to read from the specified register address of the 
 * specified I2C slave device address.
 *
 * @param slaveAddr The I2C device slave address.
 * @param regAddr The device register address.
 * @param value A parameter return value for the register data.
 *
 * @return -1 on error and 0 on success.
 */
char hmc5883l_read_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value){
   int fd;

   // open i2c
   if ((fd = open(I2C_PATH, O_RDWR)) < 0){
      perror("open()");
      return -1;
   }

   // set control
   if (ioctl(fd, I2C_SLAVE, slaveAddr) < 0){
      perror("Address");
      close(fd);
      return -1;
   }

   // set pointer register
   if (write(fd, &regAddr, 1) != 1){
      perror("write");
      close(fd);
      return -1;
   }

   // read from register
   if (read(fd, value, 1) < 0){
      perror("read");
      close(fd);
      return -1;
   }

	close(fd);

   return 0;
}

/*
 * Lower layer function to write the specified value to the specified
 * register address for the specified I2C slave device.
 *
 * @param slaveAddr The I2C device slave address.
 * @param regAddr The device register address.
 * @param value The value to write.
 *
 * @return -1 on error and 0 on success.
 */
char hmc5883l_write_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value){
    int fd;
    unsigned char val[2];
	val[0] = regAddr;
	val[1] = value[0];

   // open i2c
   if ((fd = open(I2C_PATH, O_RDWR)) < 0){
      perror("open()");
      return -1;
   }

   // set control
   if (ioctl(fd, I2C_SLAVE, slaveAddr) < 0){
      perror("Address");
      close(fd);
      return -1;
   }

   // write
   if (write(fd, val, 2) != 2){
      perror("write");
      close(fd);
      return -1;
   }

	close(fd);

   return 0;
}

/*
 * This is the user-level call to initialize the hmc5883l magnetometer.
 *
 * @return -1 on error and 0 on success.
 */
char hmc5883l_init(unsigned char addr){
   unsigned char REG_A = 0x10; //hmc5883l_CONFIG_REG_A BASE
   unsigned char REG_B = 0x20; //hmc5883l_CONFIG_REG_B BASE

   unsigned char CONFIG_A = REG_A | 0x70; //8-average, 15 Hz default
   unsigned char CONFIG_B = REG_B | 0xA0; //Gain=5

   if (hmc5883l_write_reg(addr, hmc5883l_CONFIG_REG_A, &CONFIG_A)){
      return -1;
   }
   if (hmc5883l_write_reg(addr, hmc5883l_CONFIG_REG_B, &CONFIG_B)){
      return -1;
   }

   return 0;
}

/*
 * This is the user-level call to read the magnetometer data from the sensor.
 *
 * @param addr The I2C slave address of the device.
 * @param mag A return parameter for the data in the magnetometer register.
 *
 * @return -1 on error and 0 on success.
 */
char hmc5883l_readMag(unsigned char addr, unsigned char *mag){
	char x = 0;
	for(x = 0; x <= 5; x++){
	   unsigned char Mode = 0x01; //Mode register default
	   if (hmc5883l_write_reg(addr, Mode_Register, &Mode)){ //Single-measurement mode
		  perror("Error write reg");
		  return -1;
	   }

	   usleep(6000); //wait 6ms

	   if (hmc5883l_read_reg(addr, x+3, mag+x)){
		  perror("Error read reg");
		  return -1;
	   }
	}
	return 0;

}

/*
 * This is the user-level call to call the magnetometer data from the sensor and converts it to milliTeslas.
 *
 * @param addr The I2C slave address of the device.
 * @param magnetometer A pointer to the 3 element array where the data will be written.
 *
 * @return -1 on error and 0 on success.
 */

char getMag(unsigned char addr, short int *magnetometer) {
   unsigned char buff[6];

   // Initialize and read magnetometer register
   if (hmc5883l_init(addr)) {
      fprintf(stderr, "failed to init hmc5883l\n");
      return EXIT_FAILURE;
   }

   if (hmc5883l_readMag(addr, buff)) {
      fprintf(stderr, "failed to read magnetometer\n");
      return EXIT_FAILURE;
   }

   // Convert hmc5883l magnetometer data
   char x = 0;
   for(x = 0; x <= 2; x++){
	   *magnetometer = buff[2*x];
	   *magnetometer <<= 8;
	   *magnetometer |= buff[2*x+1];
	   *magnetometer = *magnetometer * 1000 / 390;
	   magnetometer++;
   }

   return EXIT_SUCCESS;
}
