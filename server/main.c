#include "translatlib.h"

#ifndef LISTEN_HOST
#define LISTEN_HOST "0.0.0.0"
#endif

#ifndef LISTEN_PORT
#define LISTEN_PORT 2333
#endif

int main(int argc, const char **argv)
{
    int fileno;

    fileno = configure(NULL, LISTEN_PORT);

    return run(fileno);
}


