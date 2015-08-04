/**
 *  Full-duplex Intrepid SPI peripheral test for use with the BMA250 Accelerometer.
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

// Main entry point 
int main(int argc, char **argv) {
	signed int Ave[3];
	signed int Accel[3];
	int c;

	FILE *log = fopen("AccelLog.txt", "w");
	while(1)
	{
		getAccel(Accel); //pass 3 element array for accelerometer data
		printf("Accelerometer Data: X = %dmg, Y = %dmg, Z = %dmg | ", Accel[0], Accel[1], Accel[2]);
		fprintf(stdout, "Timestamp: %u\n\r", (unsigned)time(NULL));
		fprintf(log, "X = %dmg, Y = %dmg, Z = %dmg | ", Accel[0], Accel[1], Accel[2]);
		fprintf(log, "Timestamp: %u\n\r", (unsigned)time(NULL));
		fflush(log);
		sleep(1);
	}
	fclose(log);
	return 0;
}
