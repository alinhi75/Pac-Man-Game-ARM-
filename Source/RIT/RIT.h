/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>

/* Global Variables */
extern int gamePaused;
extern int score;
extern int countdown;
extern int direction;
extern int pacmanRow;
extern int pacmanCol;
extern int timerSeed;
extern volatile int globalTickCounter;
extern int powerPillTimer;

/* Constants */
#define ROWS 26
#define COLS 18
#define CELL_SIZE 12
#define WINNING_SCORE (240 * SCORE_INCREMENT_PILL)
#define SCORE_INCREMENT_PILL 10
#define SCORE_INCREMENT_POWER_PILL 50
#define MAX_POWER_PILLS 6       // Maximum number of Power Pills allowed
#define POWER_PILL_INTERVAL 500 // Interval (in milliseconds) for new Power Pill generation

/* Function Prototypes */
void enable_RIT(void);
void disable_RIT(void);
void reset_RIT(void);
void RIT_IRQHandler(void);
//void init_RIT(uint32_t interval);
uint32_t init_RIT ( uint32_t RITInterval );

#endif // __RIT_H
/*****************************************************************************
**                            End Of File
******************************************************************************/
