#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char **argv )
{
	char str[50];

    int ofile = open( "/dev/mychdev", O_RDONLY, 777 );
	int num =  read( ofile, str, 50 );

	if( num < 0 )
		printf( "Error opening file: %s\n", (char *)strerror( errno ) );
	else
		printf( "str: %s", str );

    return 0;
}
