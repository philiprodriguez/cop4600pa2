#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char stringHolder[256];
	strcpy(stringHolder, "test message!");
	
	int device = open("/dev/testdev", O_RDWR);
	if (device < 0)
	{
		printf("Failed to open device! %d\n", device);
		return -1;
	}

	int returned  = write(device, stringHolder, strlen(stringHolder));

	if (returned < 0)
	{
		printf("Shit went south writing!\n");
	}
	printf("Wrote stuff to device!\n");

	returned  = read(device, stringHolder, strlen(stringHolder));

	if (returned < 0)
	{
		printf("Shit went south reading!\n");
	}
	printf("Read stuff from device: \"%s\"!\n", stringHolder);

	return 0;
}

