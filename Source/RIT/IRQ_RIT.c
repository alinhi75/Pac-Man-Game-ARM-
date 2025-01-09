	/*********************************************************************************************************
	**--------------File Info---------------------------------------------------------------------------------
	** File name:           IRQ_RIT.c
	** Last modified Date:  2014-09-25
	** Last Version:        V1.00
	** Descriptions:        functions to manage T0 and T1 interrupts
	** Correlated files:    RIT.h
	**--------------------------------------------------------------------------------------------------------
	*********************************************************************************************************/
	#include "LPC17xx.h"
	#include "RIT.h"
	//#include "../led/led.h"
	#include "GLCD/GLCD.h"
	#include <stdio.h>


	/******************************************************************************
	** Function name:		RIT_IRQHandler
	**
	** Descriptions:		REPETITIVE INTERRUPT TIMER handler
	**
	** parameters:			None
	** Returned value:		None
	**
	******************************************************************************/

void handleCountdown(void) {
    if (countdown > 0) {
        countdown--;           // Decrease countdown
        updateTimeDisplay();   // Refresh the displayed number
    } else if (countdown == 0) {
        // Game Over condition
        LCD_Clear(Black);      // Clear the screen

        // Display "Game Over!" centered on the screen
        GUI_Text(80, 120, (uint8_t *)"Game Over!", Red, Black);

        // Display "Press Reset Button" below "Game Over!"
        GUI_Text(50, 140, (uint8_t *)"Press Reset Button", White, Black);

        // Pause the game
        gamePaused = 1;
    }
}

void RIT_IRQHandler(void) {
    static int movementCounter = 0;   // Counter for Pac-Man's movement
    static int countdownCounter = 0; // Counter for countdown updates
    static int powerPillTimer = 0;   // Timer for Power Pill generation

    if (!gamePaused) {
        // Increment counters
        movementCounter++;
        countdownCounter++;
        powerPillTimer++;

        // Move Pac-Man every 200ms
        if (movementCounter >= 25) {
            updateDirectionAndMove();
            movementCounter = 0;
        }

        // Update countdown every 1 second
        if (countdownCounter >= 100) {
            handleCountdown();
            countdownCounter = 0;
        }

        // Randomly generate Power Pills
        if (powerPillTimer >= (500 + rand() % 100)) { // Randomize interval
            generatePowerPill();
            powerPillTimer = 0; // Reset timer
        }
    }

    // Clear the RIT interrupt flag
    LPC_RIT->RICTRL |= 0x1;
}






	/******************************************************************************
	**                            End Of File
	******************************************************************************/
