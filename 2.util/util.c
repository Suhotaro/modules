#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char **argv )
{

    int fd = open( "/dev/mychdev", O_RDWR, 777 );
    printf("%d\n", fd);

    char str[20] = {0,};
    int num =  read( fd, str, 20 );


    if( num < 0 )
    	printf( "Error opening file: %s\n", (char *)strerror( errno ) );
    else
    	printf( "STR: %s\n", str );

    close(fd);
    fd = open( "/dev/mychdev", O_RDWR, 777 );

    int n = 0;
    scanf( "%d", &n );

    char str2[20] = "aaa\n";
    num = write( fd, str2, 10 );

/*
    sleep(3);

    close(fd);
    fd = open( "/dev/mychdev", O_RDWR, 777 );

    char arr[20] = {0,};

    num =  read( fd, arr, 20 );


    if( num < 0 )
    	printf( "Error opening file: %s\n", (char *)strerror( errno ) );
    else
    	printf( "STR: %s\n", arr );
*/

    return 0;
}

