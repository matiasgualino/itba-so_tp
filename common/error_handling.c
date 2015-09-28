#include "error_handling.h"
#include <stdio.h> 

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}
