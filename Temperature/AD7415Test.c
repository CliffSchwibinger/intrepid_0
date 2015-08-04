/**
 *  Tyvak Nano-Satellite Systems LLC
 * 
 *  Sample test program to utilize I2C bus. Queries Intrepid temperature sensor for data.
 *  This example utilizes the exact AD7415 driver source provided to the developer with Intrepid 
 *  (see driver source repository).
 *
 */

#include "AD7415.h"

unsigned char TempSensors[3] = {0x48, 0x49, 0x4A};  // Address of one AD7415 on Intrepid

// Main entry point
int main(int argc, char **argv) {
int i = 0;
	FILE *log = fopen("TempLog.txt", "w");
   // Print
   while(1){
	for(i = 0; i <= 2; i++)
	{
	   double t = getTemp(TempSensors[i]);
	   printf("Sensor %d Temperature: %f | ", i+1, t);
	   fprintf(stdout, "Timestamp: %u\n\r", (unsigned)time(NULL));
	   fprintf(log, "Sensor %d Temperature: %f | ", i+1, t);
	   fprintf(log, "Timestamp: %u\n\r", (unsigned)time(NULL));
        }
	   fflush(log);
	   sleep(1);
   }
	fclose(log);
}
