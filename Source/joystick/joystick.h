																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

void joystick_init(void);
int getJoystickState(void);

extern int pacmanRow;
extern int pacmanCol;
extern int direction;
extern int gamePaused;

#endif /* __JOYSTICK_H */
