#include "bsp.h"

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

/* ============================================================
 *  Static (private) variables
 * ============================================================*/
#define NUMBER_OF_OUTPUTS 1
#define NUMBER_OF_INPUTS 1

typedef enum {
    eCONFIG_GPIO_OK,
    eCONFIG_GPIOS_CHIP_ERROR,
    eCONFIG_GPIOS_LINE_ERROR,  
} configGpios_t;

typedef struct {
    unsigned int gpioOutput;
    unsigned int valueOutput;
} outputs_t;

typedef struct {
    unsigned int gpioInput;
    int valueInput;
} inputs_t;

const char *chip_path = "/dev/gpiochip0";
struct gpiod_chip *chip;
struct gpiod_line_request *reqLedYellow;

outputs_t outputs[NUMBER_OF_OUTPUTS];

struct gpiod_line_request *reqButton1;
inputs_t inputs[NUMBER_OF_INPUTS];

/* ============================================================
 *  Static (private) functions
 * ============================================================*/
static configGpios_t bsp_initOutputs(void);
static configGpios_t bsp_initInputs(void);

/* ============================================================
 *  Public functions
 * ============================================================*/
void bsp_init(void) {

    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        perror("Failed to open gpiochip");
        return;
    }

    //OUTPUTS
    if (bsp_initOutputs() != eCONFIG_GPIO_OK) {
        printf("Error on outputs config\n");
    }
    else {
        printf("Success on outputs config\n");
        for(int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
            printf("GPIO: %d, VALUE: %d\n",outputs[i].gpioOutput, outputs[i].valueOutput);
        }  
    }

    //INPUTS
    if (bsp_initInputs() != eCONFIG_GPIO_OK) {
        printf("Error on inputs config\n");
    }
    else {
        printf("Success on inputs config\n");
    }
}

void bsp_closeGPIOs() {
    gpiod_line_request_release(reqLedYellow);
    gpiod_chip_close(chip);
}

void bsp_OutputsHandler(void) {
    for(int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
        gpiod_line_request_set_value(reqLedYellow, outputs[i].gpioOutput, outputs[i].valueOutput);
        printf("OUT GPIO: %d, VALUE: %d\n",outputs[i].gpioOutput, outputs[i].valueOutput);
    }  
    for(int i = 0; i < NUMBER_OF_INPUTS; i++) {
        printf("IN GPIO: %d, VALUE: %d\n",inputs[i].gpioInput, inputs[i].valueInput);
    } 
}

void bsp_SetOutputValue(outputsName_t output, bool value) {
    if(output >= NUMBER_OF_OUTPUTS) {
        return;
    }
    outputs[output].valueOutput = value; 
}

int bsp_GetInputValue(){
    inputs[eBUTTON_1].valueInput = gpiod_line_request_get_value(reqButton1, inputs[eBUTTON_1].gpioInput);
    return inputs[eBUTTON_1].valueInput;
}

/* ============================================================
 *  Private functions (static)
 * ============================================================*/
static configGpios_t bsp_initOutputs(void) {

    /* Assign GPIO number for the output */
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

    reqLedYellow = gpiod_chip_request_lines(chip, NULL, line_cfg);
    if (!reqLedYellow) {
        perror("Failed to request line");
        return eCONFIG_GPIOS_LINE_ERROR;
    }

    return eCONFIG_GPIO_OK;
}

static configGpios_t bsp_initInputs(void) {

    /* Assign GPIO number for the input */
    inputs[eBUTTON_1].gpioInput = GPIO_BUTTON_1;

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    // Enable pull-up (important for buttons!)
    gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(
        line_cfg,
        &inputs[eBUTTON_1].gpioInput,
        1,
        settings
    );

    reqButton1 = gpiod_chip_request_lines(chip, NULL, line_cfg);
    if (!reqButton1) {
        perror("Failed to request input line");
        return eCONFIG_GPIOS_LINE_ERROR;
    }

    return eCONFIG_GPIO_OK;
}
