/**
 *  Full-duplex Intrepid SPI peripheral use example. A majority of the full-duplex support
 *  is handled at the kernel driver level via ioctl.  Hence, no read/write system calls are needed
 *  outside of an open() to create and bind a file descriptor to the device.
 *  
 *  Additional SPI kernel driver documentation available at linux-2.6.30.2/Documentation/spidev
 *  Additional documentation regarding spi_ioc_transfer struct parameters is available under linux-2.6.30.2/include/linux/spi/spidev.h 
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <syslog.h>

/*
 * This is the low level call to call the accelerometer data.
 *
 * @param addr The register address of the accelerometer to be read.
 *
 * @return dataBytes[1] The second byte from the spi transfer that contains acceleration data.
 */

uint8_t readAccel(unsigned short addr) {

   int ret, fd, i;
   struct sensorPing *currSens;
   uint8_t dataBytes[128];
   struct spi_ioc_transfer xf;
   double Data = 0;
   memset(dataBytes, 0, sizeof(dataBytes));

struct sensorPing {
   char *dev;
   uint16_t cmd;  // Define command array if necessary
} sensors[] = {
   { "/dev/spidev1.0", addr }, //address to be read
   { 0, 0 }
};

   // Ping sensor given command(s) above
   	for (currSens = sensors; currSens->dev; currSens++) {

	      // Setup unique spi_ioc_transfer for ioctl - tx_buf, rx_buf may be the same if desired
	      memset(&xf, 0, sizeof(xf));
	      xf.tx_buf = (unsigned long)(&currSens->cmd);
	      xf.rx_buf = (unsigned long)dataBytes;
	      xf.len = 2; // Length of buffer sizes in bytes (i.e., only expecting 16-bits in return in this example)

	      // Create device node file descriptor
	      if ((fd = open(currSens->dev, O_RDWR)) < 0) {
		 fprintf(stderr, "Error opening device: %s\n\r", currSens->dev);
		 continue;
	      }
	      
	      // Attempt to command and read device
	      if ((ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xf)) < 0) {
		 perror("Error transferring (R/W) bytes\n\r");
		 continue;
	      }
	      close(fd);

	} //end for loop
   return dataBytes[1];
}

/*
 * This is the user-level call to call the accelerometer data and convert it to mili-Gs.
 *
 * @param *ptr A pointer to a 3 element array to which the readings will be written to.
 *
 * @return 0 on success.
 */

int getAccel(int *ptr) {

   int AccelData[] = {0x0082,  //x-axis LSB
			0x0083,  //x-axis MSB
			0x0084,  //y-axis LSB
			0x0085,  //y-axis MSB
			0x0086,  //z-axis LSB
			0x0087}; //z-axis MSB
   int i, AccelXLSB, AccelXMSB, AccelYLSB, AccelYMSB, AccelZLSB, AccelZMSB;
   signed int Data[3];

	AccelXLSB = readAccel(AccelData[0]);
	AccelXMSB = readAccel(AccelData[1]);
	AccelYLSB = readAccel(AccelData[2]);
	AccelYMSB = readAccel(AccelData[3]);
	AccelZLSB = readAccel(AccelData[4]);
	AccelZMSB = readAccel(AccelData[5]);
	Data[0] = ((AccelXMSB << 8) | AccelXLSB) >> 6;
	Data[1] = ((AccelYMSB << 8) | AccelYLSB) >> 6;
	Data[2] = ((AccelZMSB << 8) | AccelZLSB) >> 6;

	for(i = 0; i <= 1; i++)
	{
		if(Data[i] > 900)
			{
			Data[i] = Data[i] - 1000;
			}
	}

	*ptr = (Data[0]*2000/511); ptr++;
	*ptr = (Data[1]*2000/511); ptr++;
	*ptr = (Data[2])*2000/511;

	return 0;
}
