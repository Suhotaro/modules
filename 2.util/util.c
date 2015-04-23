#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char **argv )
{

    int fd = open( "/dev/mychdev", O_RDONLY, 777 );
    printf("%d\n", fd);

    char str[20];
    int num =  read( fd, str, 20 );

    if( num < 0 )
    	printf( "Error opening file: %s\n", (char *)strerror( errno ) );
    else
    	printf( "str: %s", str );

    char *str2 = "yarrr!!!";
    num = write( fd, str2, 20 );

    num =  read( fd, str, 20 );

    if( num < 0 )
    	printf( "Error opening file: %s\n", (char *)strerror( errno ) );
    else
    	printf( "str: %s", str );


    return 0;
}
