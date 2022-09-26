#include <stdio.h>

#include "String.h"

struct CP_String {
    char* string;
    int len;
};

struct CP_StringIF {
    CP_String *String;

    CP_String *(*setString)(char* stringBuffer);
    
};