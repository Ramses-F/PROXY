#include <stdio.h>
#include <stdlib.h>
#include "proxy.h"
#include "config.h"
#include "logger.h"

int main()
{
    printf("Lancement de proxy........\n");
    start_proxy(PORT);

    return 0;
}