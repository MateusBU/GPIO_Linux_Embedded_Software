#include "main.h"
#include "bsp.h"

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    bsp_init();
    while (1) {
        bsp_OutputsHandler(void);
    }

    bsp_closeGPIOs(void);
    return 0;
}
