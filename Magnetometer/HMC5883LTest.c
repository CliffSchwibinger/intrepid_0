/*
 * This is the user-level example to read the magnetometer data from the sensor.
 *
 */

#include "HMC5883L.h"
#include <math.h> //having difficulty implementing sqrt()
#include <time.h>

#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

// Main entry point
int main(int argc, char **argv) {
   short int MagData[3];
   int Magnitude;  //Magnitude of magnetic vectors = sqrt(x^2+y^2+z^2)
   int Var = 48;
   unsigned char addr = 0x1E;  // Address of one hmc5883l on Intrepid
   FILE *log = fopen("MagLog.txt", "w");
   while(1){
	getMag(addr, MagData);
	Magnitude = sqrt((MagData[0]*MagData[0])+(MagData[2]*MagData[2])+(MagData[1]*MagData[1]));
	printf("Magnetometer Data: X = %dmG, Y = %dmG, Z = %dmG, Magnitude = %dmG\n\r",
		MagData[0], MagData[2], MagData[1], Magnitude);
		//Note: the second value of the array is the z-axis and the third is the y-axis
		//This is due to the order in which the registers are placed on the HMC5883L
	fprintf(stdout, "Timestamp: %u\n\r", (unsigned)time(NULL));

	fprintf(log, "Magnetometer Data: X = %dmG, Y = %dmG, Z = %dmG, Magnitude = %dmG\n\r",
		MagData[0], MagData[2], MagData[1], Magnitude);
	fprintf(log, "Timestamp: %u\n\r", (unsigned)time(NULL));
	fflush(log);
   	sleep(1);
   }
   fclose(log);
}
