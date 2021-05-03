#include <stdio.h>
#include "vector.h"
#include "stream.h"

int main() {

    // -----
    //  Vector2 example: construct vector with two parameters, set X and Y, calculate magnitude.

    Vector2 (v1)->init(3, 4);

    ref (v1)->setX(6);
    ref (v1)->setY(8);

    double magnitude = ref (v1)->magnitude();
    
    printf("Magnitude is: %f\n", magnitude);

    // -----
    //  Stream example: construct streams (init function but this time without parameters), set file to stdout/stderr, write to stream.

    Stream (stream)->init();
    ref (stream)->setFile(stdout);
    ref (stream)->write("Writing from stream object!\n");

    Stream (errStream)->init();
    ref (errStream)->setFile(stderr);
    ref (errStream)->write("Error message goes here.\n");

    return 0;
}
