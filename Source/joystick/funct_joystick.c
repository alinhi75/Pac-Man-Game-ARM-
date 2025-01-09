/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLCD/GLCD.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


//// Joystick pin definitions
#define JOYSTICK_UP     (1 << 29) // P1.29
#define JOYSTICK_DOWN   (1 << 26) // P1.26
#define JOYSTICK_LEFT   (1 << 27) // P1.27
#define JOYSTICK_RIGHT  (1 << 28) // P1.28

/**
 * @brief  Get the state of the joystick.
 * @retval An integer representing the joystick state:
 *         - 0 for UP
 *         - 1 for DOWN
 *         - 2 for LEFT
 *         - 3 for RIGHT
 *         - -1 if no direction is pressed
 */

/**
 * @brief Initialize the joystick by setting up the GPIO pins.
 */
void joystick_init(void) {
    // Configure P1.29, P1.26, P1.27, and P1.28 as GPIO
    LPC_PINCON->PINSEL3 &= ~(3 << 26); // P1.29 (UP)
    LPC_PINCON->PINSEL3 &= ~(3 << 20); // P1.26 (DOWN)
    LPC_PINCON->PINSEL3 &= ~(3 << 22); // P1.27 (LEFT)
    LPC_PINCON->PINSEL3 &= ~(3 << 24); // P1.28 (RIGHT)

    // Set P1.29, P1.26, P1.27, and P1.28 as input
    LPC_GPIO1->FIODIR &= ~JOYSTICK_UP;
    LPC_GPIO1->FIODIR &= ~JOYSTICK_DOWN;
    LPC_GPIO1->FIODIR &= ~JOYSTICK_LEFT;
    LPC_GPIO1->FIODIR &= ~JOYSTICK_RIGHT;

    // Enable pull-ups for P1.29, P1.26, P1.27, and P1.28
    LPC_PINCON->PINMODE3 &= ~(3 << 26); // P1.29 pull-up
    LPC_PINCON->PINMODE3 &= ~(3 << 20); // P1.26 pull-up
    LPC_PINCON->PINMODE3 &= ~(3 << 22); // P1.27 pull-up
    LPC_PINCON->PINMODE3 &= ~(3 << 24); // P1.28 pull-up
}


volatile uint32_t lastJoystickReadTime = 0;

int getJoystickState(void) {
    uint32_t joystickState = LPC_GPIO1->FIOPIN;

//    char buffer[30];
//    sprintf(buffer, "Joystick GPIO: %X", joystickState);
//    GUI_Text(5, 300, (uint8_t *)buffer, White, Black); // Display GPIO state

    if (!(joystickState & JOYSTICK_UP)) {
        //GUI_Text(5, 320, (uint8_t *)"Joystick UP", Green, Black);
        return UP;
    } else if (!(joystickState & JOYSTICK_DOWN)) {
        //GUI_Text(5, 320, (uint8_t *)"Joystick DOWN", Green, Black);
        return DOWN;
    } else if (!(joystickState & JOYSTICK_LEFT)) {
        //GUI_Text(5, 320, (uint8_t *)"Joystick LEFT", Green, Black);
        return LEFT;
    } else if (!(joystickState & JOYSTICK_RIGHT)) {
        //GUI_Text(5, 320, (uint8_t *)"Joystick RIGHT", Green, Black);
        return RIGHT;
    }

    //GUI_Text(5, 320, (uint8_t *)"Joystick NONE", Red, Black);
    return -1; // No valid input
}



