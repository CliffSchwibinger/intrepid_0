/**
 * Test program for the gyroscope.
 * Initializes the gyroscope, giving the initial offset values,
 * then samples the gyroscope endlessly, adjusting for the offset.
 * Sleep function is included in the example, though it is never executed.
 */

#include "L3G4200D.h"

// Main entry point
int main(int argc, char **argv) {
   
   double GyroData[3], Offset[3], Noise[3]; int a, b;
   unsigned char addr = 0x69;  // Address of one l3g4200d on Intrepid
   FILE *log = fopen("GyroLog.txt", "w");

	if (l3g4200d_init(addr, Offset)) //pass address + 3 element for the offset
	{
		  fprintf(stderr, "failed to init hmc5883l\n");
		  return EXIT_FAILURE;
	}
	printf("Initial Offset: X = %fdps, Y = %fdps, Z = %fdps\n", Offset[0], Offset[1], Offset[2]);


	while(1){
		if (getGyro(addr, GyroData)) //pass address + 3 element array for data
	{
		  fprintf(stderr, "failed to init hmc5883l\n");
		  return EXIT_FAILURE;
	}
		printf("Gyro: X = %fdps, Y = %fdps, Z = %fdps | ", (GyroData[0]-Offset[0]), (GyroData[1]-Offset[1]), (GyroData[2]-Offset[2])); //subtract offset from readings
		fprintf(stdout, "Timestamp: %u\n\r", (unsigned)time(NULL));

		fprintf(log, "Val: X = %fdps, Y = %fdps, Z = %fdps | ", (GyroData[0]-Offset[0]), (GyroData[1]-Offset[1]), (GyroData[2]-Offset[2])); //subtract offset from readings
		fprintf(log, "Timestamp: %u\n\r", (unsigned)time(NULL));
		fflush(log);
		sleep(1);
  	} //end while
	fclose(log);
	l3g4200d_sleep(addr);
	
   return 0;
}
