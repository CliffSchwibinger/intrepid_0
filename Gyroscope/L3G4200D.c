/**
 *
 *
 *
 *
 * 
 * 
 */

#include "L3G4200D.h"

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
char l3g4200d_read_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value){
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
char l3g4200d_write_reg(unsigned char slaveAddr, unsigned char regAddr, unsigned char *value){
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
 * This is the user-level call to initialize the l3g4200d GYRO, and return an offset value based on
 * the average of a specified number of samples.
 * @return -1 on error and 0 on success.
 */
char l3g4200d_init(unsigned char addr, double *ptr){
   unsigned char REG_A = 0x07; //l3g4200d_CONFIG_REG1 BASE
   unsigned char CONFIG_A = REG_A | 0x0F; //Power on
   double Ave[3], Data[3];
   int samplesize = 500;
   int x, y;

   if (l3g4200d_write_reg(addr, l3g4200d_GYRO_REG1, &CONFIG_A)){
	   perror("Error write reg");
	   return -1;
   }

	sleep(1); //wait 1s

   	for(x = 0;x < samplesize; x++){
		getGyro(addr, Data);
        	for(y = 0;y <= 2;y++){
			Ave[y] += Data[y];
		}
	}
/*
	//------Calculate Noise----------------------
	Noise[0]=0; Noise[1]=0; Noise[2]=0;
	for (a=0;a<500;a++){
		getGyro(addr, GyroData);
		for(b=0;b<=2;b++){
			if(GyroData[b]-Offset[b]>Noise[b]){
				Noise[b]=GyroData[b]-Offset[b];
			}else if(GyroData[b]-Offset[b]<-Noise[b]){
				Noise[b]=- GyroData[b]-Offset[b];
			}
		}
	}//end for
	Noise[0]=Noise[0]/100; Noise[1]=Noise[1]/100; Noise[2]=Noise[2]/100; //gyro returns 1/100th of dps
	printf("Noise: X = %f, Y = %f, Z = %f\n", Noise[0], Noise[1], Noise[2]);
*/
	Ave[0]=Ave[0]/samplesize;
	Ave[1]=Ave[1]/samplesize;
	Ave[2]=Ave[2]/samplesize; //initial offsets

	*ptr = Ave[0]; ptr++;
	*ptr = Ave[1]; ptr++;
	*ptr = Ave[2];

   return 0;
}

/*
 * This is the user-level call to sleep the l3g4200d GYRO, reducing power usage.
 * @return -1 on error and 0 on success.
 */
char l3g4200d_sleep(unsigned char addr){
   unsigned char REG_A = 0x07; //l3g4200d_CONFIG_REG1 BASE
   unsigned char CONFIG_A = REG_A & 0x08; //Axis measurement off

   if (l3g4200d_write_reg(addr, l3g4200d_GYRO_REG1, &CONFIG_A)){
	   perror("Error write reg");
	   return -1;
   }

   return 0;
}

/*
 * This is the user-level call to read the gyro from the sensor.
 * The data in the gyro register is written into a 2-byte array.
 * This data needs to be bit-wise ORed into a 2 byte variable and
 * shifted 6 bits to the right. To convert to the actual gyro
 * value, see the l3g4200d datasheet.
 *
 * @param addr The I2C slave address of the device.
 * @param gyro A return parameter for the data in the gyro register.
 *
 * @return -1 on error and 0 on success.
 */
char l3g4200d_readGyro(unsigned char addr, unsigned char *gyro){
   char x = 0;
   for(x = 0; x <= 5; x++){
	   if (l3g4200d_read_reg(addr, x+0x28, gyro+x)){
		   perror("Error read reg");
		   return -1;
	   }
   }

   return 0;
}

/*
 * This is the user-level call to call the gyro data and convert it to degrees per second.
 *
 * @param addr The I2C slave address of the device.
 * @param gyro A pointer to a 3 element array to which the readings will be written to.
 *
 * @return -1 on error and 0 on success.
 */

char getGyro(unsigned char addr, double *gyro) {

   unsigned char buff[6];
   short int temp;
   short int Tmp[3];
   char x = 0;

	   if (l3g4200d_readGyro(addr, buff)) {
		  fprintf(stderr, "failed to read Gyro\n");
		  return EXIT_FAILURE;
	   }

	   // Convert l3g4200d gyro data
	   for(x = 0; x <= 2; x++){
		   Tmp[x] = buff[2*x+1];
		   Tmp[x] <<= 8;
		   Tmp[x] |= buff[2*x];
		   *gyro = (double)(Tmp[x] * 0.00875);
		   gyro++;
	   }

   return EXIT_SUCCESS;
}
