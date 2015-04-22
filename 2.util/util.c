#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

int main( int argc, char **argv )
{
    
    int ofile = open( "/dev/mycdev", "rw" );



    while( 1 )
    {
    	char str[20];
    	int =  read(ofile, str, 20 );
    }

    return 0;
}
