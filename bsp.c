#include "bsp.h"

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

/* ============================================================
 *  Static (private) variables
 * ============================================================*/
#define NUMBER_OF_OUTPUTS 1

typedef enum {
    eCONFIG_OUTPUTS_OK,
    eCONFIG_OUTPUTS_CHIP_ERROR,
    eCONFIG_OUTPUTS_LINE_ERROR,  
} configOutputs_t;

typedef struct {
    unsigned int gpioOutput;
    unsigned int valueOutput;
} outputs_t;

const char *chip_path = "/dev/gpiochip0";
struct gpiod_chip *chip;
struct gpiod_line_request *req;

outputs_t outputs[NUMBER_OF_OUTPUTS];

/* ============================================================
 *  Static (private) functions
 * ============================================================*/
static configOutputs_t bsp_initOutputs(void);

/* ============================================================
 *  Public functions
 * ============================================================*/
void bsp_init(void) {

    //OUTPUTS
    if (bsp_initOutputs() != eCONFIG_OUTPUTS_OK) {
        printf("Error on outputs config\n");
    }
    else {
        printf("Success on outputs config\n");
    }

    //INPUTS
}

void bsp_closeGPIOs(void) {
    gpiod_line_request_release(req);
    gpiod_chip_close(chip);
}

void bsp_OutputsHandler(void) {
    for(int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
        gpiod_line_request_set_value(req, outputs[i].gpioOutput, outputs[i].valueOutput);
    }  
}

void bsp_SetOutputValue(outputs_t output, bool value) {
    if(output >= NUMBER_OF_OUTPUTS) {
        return;
    }
    outputs[output].valueOutput = value; 
}

/* ============================================================
 *  Private functions (static)
 * ============================================================*/
static configOutputs_t bsp_initOutputs(void) {

    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        perror("Failed to open gpiochip");
        return eCONFIG_OUTPUTS_CHIP_ERROR;
    }

    /* Assign GPIO number for the only output */
    outputs[eLED_YELLOW].gpioOutput = GPIO_YELLOW_LED;

    /* Configure the line as output */
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(
        line_cfg,
        &outputs[eLED_YELLOW].gpioOutput,
        1,
        settings
    );

    req = gpiod_chip_request_lines(chip, NULL, line_cfg);
    if (!req) {
        perror("Failed to request line");
        return eCONFIG_OUTPUTS_LINE_ERROR;
    }

    return eCONFIG_OUTPUTS_OK;
}
