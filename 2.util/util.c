#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char **argv )
{

    int ofile = open( "/dev/mychdev", O_RDONLY, 777 );
    printf("%d\n", ofile);

    while( 1 )
    {
    	char str[20];
    	int num =  read( ofile, str, 20 );

    	if( num < 0 )
    		printf( "Error opening file: %s\n", (char *)strerror( errno ) );
    	else
    		printf( "str: %s", str );
    }

    return 0;
}
