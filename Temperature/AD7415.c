/**
 * ad7415.c - source file for ad7415 temperature sensor.
 *
 *  Tyvak Nano-Satellite Systems LLC
 *
 */

#include "AD7415.h"

/*
 * Lower layer function to read from the specified register address of the 
 * specified I2C slave device address.
 *
 * @param slaveAddr The I2C device slave address.
 * @param regAddr The device register address.
 * @param value A parameter return value for the register data.
 *
 * @return -1 on error and 0 on success.

 Transfer when master is recieving one byte of data from slave
 */
char ad7415_read_reg(unsigned char slaveAddr, unsigned char regAddr,
                     unsigned char *value){
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
   if (write(fd, &regAddr, AD7415_PNTR_REG) != AD7415_PNTR_REG){
      perror("write");
      close(fd);
      return -1;
   }

   // read from register
   if (regAddr == AD7415_CONFIG_REG){
      if (read(fd, value, AD7415_CONFIG_REG_SIZE) < 0){
         perror("read");
         close(fd);
         return -1;
      }
   }
   else{
      if (read(fd, value, AD7415_TEMP_REG_SIZE) < 0){
         perror("read");
         close(fd);
         return -1;
      }
   
   }

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

Transfer when master is writing one byte of data from slave
 */
char ad7415_write_reg(unsigned char slaveAddr, unsigned char regAddr,
                      unsigned char *value){
   int fd;
   unsigned char val[3];
   
   val[0] = regAddr;
   val[1] = value[0];
   if (regAddr == AD7415_TEMP_REG){
      val[2] = value[1];
   }

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
   if (regAddr == AD7415_CONFIG_REG){
      if (write(fd, val, AD7415_CONFIG_REG_SIZE + 1) != 
                         AD7415_CONFIG_REG_SIZE + 1){
         perror("write");
         close(fd);
         return -1;
      }
   }
   else if (regAddr == AD7415_TEMP_REG){
      if (write(fd, val, AD7415_TEMP_REG_SIZE + 1) != 
                         AD7415_TEMP_REG_SIZE + 1){
         perror("write");
         close(fd);
         return -1;
      }
   }

   return 0;
}

/*
 * This is the user-level call to initialize the AD7415 temperature.
 * Use the predefined values of AD7415_ONESHOT_ON or AD7415_ONESHOT_OFF
 * to set the oneshot value. AD7415_FILTER_ON/AD7415_FILTER_OFF for the
 * filter setting and AD7415_POWERDOWN_ON/OFF for the powerdown setting.
 *
 * @param addr The I2C slave address of the device to initialize.
 * @param oneshot The oneshot setting to use.
 * @param filter The filter setting to use.
 * @param powerdown The powerdown setting to use.
 *
 * @return -1 on error and 0 on success.
 */
char ad7415_init(unsigned char addr, unsigned char oneshot,
                 unsigned char filter, unsigned char powerdown){
   unsigned char val = AD7415_CONFIG_BASE;

   if (oneshot){
      val |= AD7415_ONESHOT_VAL;
   }
   if (filter){
      val |= AD7415_FILTER_VAL;
   }
   if (powerdown){
      val |= AD7415_POWERDOWN_VAL;
   }

   if (ad7415_write_reg(addr, AD7415_CONFIG_REG, &val)){
      return -1;
   }   

   return 0;
}

/*
 * This is the user-level call to read the temperature from the sensor.
 * The data in the temperature register is written into a 2-byte array.
 * This data needs to be bit-wise ORed into a 2 byte variable and
 * shifted 6 bits to the right. To convert to the actual temperature
 * value, see page 8 of the AD7415 datasheet.
 *
 * @param addr The I2C slave address of the device.
 * @param temp A return parameter for the data in the temperature register.
 *
 * @return -1 on error and 0 on success.
 */
char ad7415_readTemp(unsigned char addr, unsigned char *temp){
   unsigned char val = AD7415_ONESHOT_VAL;
   if (ad7415_write_reg(addr, AD7415_CONFIG_REG, &val)){
      return -1;
   }

   // note: the temp variable needs to be shifted 6 bits to the right
   if (ad7415_read_reg(addr, AD7415_TEMP_REG, temp)){
      return -1;
   }

   return 0;
}

/*
 * This is the user-level call to retrieve the temperature from the sensor,
 * and convert to the actual temperature value, see page 8 of the AD7415 datasheet.
 *
 * @param addr The I2C slave address of the device.
 * @param temp A return parameter for the data in the temperature register.
 *
 * @return -1 on error and 0 on success.
 */

double getTemp(unsigned char addr) {
   unsigned char buff[2];
   short int temperature;
   double t;

   // Initialize and read temperature register
   if (ad7415_init(addr, AD7415_ONESHOT_OFF, AD7415_FILTER_OFF, AD7415_POWERDOWN_VAL)) {
      fprintf(stderr, "failed to init ad7415\n");
      return EXIT_FAILURE;
   }

   if (ad7415_readTemp(addr, buff)) {
      fprintf(stderr, "failed to read temp\n");
      return EXIT_FAILURE;
   }

   // Convert AD7415 temperature data
   temperature = buff[0];
   if (buff[0] & 256){   
      temperature -= 256;
   }
   temperature <<= 8;
   temperature |= buff[1];
   temperature >>= 6;
   if (buff[0] & 256){  
      temperature -= 512;
   }
   t = (double)temperature * 0.25;

   //printf("Raw temperature register reads 0x%x\n", temperature);

   return t;
}
