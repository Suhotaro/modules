#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "../1.cdev/chdev_ioctl.h"

int main( int argc, char **argv )
{
	int res = 0, num = 0;

    int fd = open( "/dev/mychdev", O_RDWR, 777 );
  
    res = ioctl( fd, CHDEV_IOGET, &num );
	printf( "1: res=%d num=%d\n", res, num );

    num = 12;
    res = ioctl( fd, CHDEV_IOSET, &num );
	printf( "1: res=%d num=%d\n", res, num );

    return 0;
}

