#include "main.h"
#include "app.h"
#include "bsp.h"

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    bsp_init();
    while (1) {
        app_Handler();
        bsp_OutputsHandler();
        sleep(1);
    }

    bsp_closeGPIOs();
    return 0;
}
