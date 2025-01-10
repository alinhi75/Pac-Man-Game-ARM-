
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

/* Definitions ----------------------------------------------------------------*/
#define ROWS 26
#define COLS 18
#define CELL_SIZE 12  // Size of each maze cell on the LCD

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define JOYSTICK_UP     (1 << 29) // P1.29
#define JOYSTICK_DOWN   (1 << 26) // P1.26
#define JOYSTICK_LEFT   (1 << 27) // P1.27
#define JOYSTICK_RIGHT  (1 << 28) // P1.28

#define WALL_COLOR Blue
#define PILL_COLOR Yellow
#define EMPTY_COLOR Black
#define PACMAN_COLOR Green
#define POWER_PILL_COLOR Red
#define POWER_PILL 4

#define INITIAL_LIVES 1
#define SCORE_INCREMENT_PILL 10
#define SCORE_INCREMENT_POWER_PILL 50
#define WINNING_SCORE (240 * SCORE_INCREMENT_PILL)

#define MAX_POWER_PILLS 6       // Maximum number of Power Pills allowed
#define POWER_PILL_INTERVAL 500 // Interval (in milliseconds) for new Power Pill generation
#define MAX_ACTIVE_POWER_PILLS 6

typedef struct {
    int row;
    int col;
    int active; // 1 if active, 0 otherwise
} PowerPill;

PowerPill powerPills[MAX_ACTIVE_POWER_PILLS];



/* Maze Definition -----------------------------------------------------------*/

int maze[ROWS][COLS] = {
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 1, 3, 1, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 1, 3},
    {3, 1, 3, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3, 1, 3}, // Teleport points at column 0 and column 17
    {3, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 3, 1, 3, 3, 1, 3, 1, 1, 1, 1, 1, 3},
    {3, 3, 3, 1, 3, 3, 3, 1, 1, 1, 1, 3, 1, 3, 1, 3, 1, 3},
    {3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 3},
    {3, 3, 3, 1, 3, 1, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 3, 3},
    {0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 0},
    {3, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 3},
    {3, 1, 3, 1, 1, 1, 3, 1, 3, 3, 1, 3, 1, 1, 1, 3, 1, 3},
    {3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3},
    {3, 1, 3, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 1, 3, 1, 3, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 3},
    {3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3},
    {3, 3, 3, 1, 3, 1, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 3, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 1, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
};



/* Global Variables ----------------------------------------------------------*/
int pacmanRow = 5;
int pacmanCol = 7;
int direction = -1;
int score = 0;
int lives = INITIAL_LIVES;
int countdown = 60;  // Game countdown timer (in seconds)
int gamePaused = 1;  // Start in PAUSE mode
//int PowerPillCount = 0;
int getTouchInput(int *x, int *y) {
    // Read touch coordinates and store them in x and y
    // Return 1 if successful, 0 otherwise
    return 1;  // Example return value for successful touch
}
int powerPillsGenerated = 0;
int timerSeed = 0;  // Incremented dynamically elsewhere
volatile int globalTickCounter = 0; // Tracks time for countdown in main()
int powerPillsCount = 0;        // Current count of Power Pills
int powerPillTimer = 0;         // Timer to control random generation of Power Pills


/* Function Prototypes -------------------------------------------------------*/
void drawMaze(void);
void drawPacman(int row, int col);
void generatePowerPills(void);
void updateScore(int increment);
void checkWinCondition(void);
void pauseGame(void);
void resumeGame(void);
void countdownTimer(void);
void restartGame(void);
void updateDirectionAndMove(void);


/* Functions -----------------------------------------------------------------*/
void drawHeart(int x, int y) {
    // Draw the left part of the heart (circle)
    LCD_DrawFilledCircle(x - 4, y, 4, Red);

    // Draw the right part of the heart (circle)
    LCD_DrawFilledCircle(x + 4, y, 4, Red);

    // Draw the bottom triangle part of the heart
    LCD_DrawFilledTriangle(x - 8, y, x + 8, y, x, y + 10, Red);
}

void handleTeleportation(int *col) {
    if (*col < 0) { // Exiting the left boundary
        *col = COLS - 1; // Reappear on the right
    } else if (*col >= COLS) { // Exiting the right boundary
        *col = 0; // Reappear on the left
    }
}


void updateDirectionAndMove(void) {
    static int pendingDirection = -1; // Store user input until it can be applied

    int joystickState = getJoystickState();

    // If the joystick state is valid, update the pending direction
    if (joystickState != -1) {
        pendingDirection = joystickState;
    }

    // Attempt to apply the pending direction
    int nextRow = pacmanRow;
    int nextCol = pacmanCol;

    switch (pendingDirection) {
        case UP:    nextRow--; break;
        case DOWN:  nextRow++; break;
        case LEFT:  nextCol--; break;
        case RIGHT: nextCol++; break;
        default:    break;
    }

    // Check if pending direction is valid
    if (nextRow >= 0 && nextRow < ROWS && nextCol >= 0 && nextCol < COLS && maze[nextRow][nextCol] != 3) {
        direction = pendingDirection; // Apply pending direction if valid
    }

    // Update position based on the current direction
    nextRow = pacmanRow;
    nextCol = pacmanCol;

    switch (direction) {
        case UP:    nextRow--; break;
        case DOWN:  nextRow++; break;
        case LEFT:  nextCol--; break;
        case RIGHT: nextCol++; break;
        default:    return; // No movement
    }

    handleTeleportation(&nextCol);

    // Check if movement is valid
    if (nextRow >= 0 && nextRow < ROWS && nextCol >= 0 && nextCol < COLS && maze[nextRow][nextCol] != 3) {
        // Clear current position
        LCD_DrawRectangle(pacmanCol * CELL_SIZE + 10, pacmanRow * CELL_SIZE + 40, CELL_SIZE, CELL_SIZE, EMPTY_COLOR);

        // Update Pac-Man position
        pacmanRow = nextRow;
        pacmanCol = nextCol;

        // Handle pills and power pills
        if (maze[pacmanRow][pacmanCol] == 1) {
            maze[pacmanRow][pacmanCol] = 0;  // Mark pill as eaten
            updateScore(SCORE_INCREMENT_PILL);
        } else if (maze[pacmanRow][pacmanCol] == POWER_PILL) {
						maze[pacmanRow][pacmanCol] = 0;  // Mark Power Pill as eaten
						updateScore(SCORE_INCREMENT_POWER_PILL);
						powerPillsCount--;               // Decrease Power Pill count
}
				checkWinCondition();
        // Redraw Pac-Man
        drawPacman(pacmanRow, pacmanCol);
    }
}




void drawPacman(int row, int col) {
    int x = col * CELL_SIZE + 10;  // X coordinate
    int y = row * CELL_SIZE + 40; // Y coordinate

    // Draw the main Pac-Man circle
    LCD_DrawFilledCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 2, PACMAN_COLOR);

    // Draw the Pac-Man mouth based on direction
    switch (direction) {
        case UP:
            LCD_DrawTriangle(x + CELL_SIZE / 2, y + CELL_SIZE / 2,
                             x + CELL_SIZE / 3, y,
                             x + 2 * CELL_SIZE / 3, y, EMPTY_COLOR);
            break;
        case DOWN:
            LCD_DrawTriangle(x + CELL_SIZE / 2, y + CELL_SIZE / 2,
                             x + CELL_SIZE / 3, y + CELL_SIZE,
                             x + 2 * CELL_SIZE / 3, y + CELL_SIZE, EMPTY_COLOR);
            break;
        case LEFT:
            LCD_DrawTriangle(x + CELL_SIZE / 2, y + CELL_SIZE / 2,
                             x, y + CELL_SIZE / 3,
                             x, y + 2 * CELL_SIZE / 3, EMPTY_COLOR);
            break;
        case RIGHT:
            LCD_DrawTriangle(x + CELL_SIZE / 2, y + CELL_SIZE / 2,
                             x + CELL_SIZE, y + CELL_SIZE / 3,
                             x + CELL_SIZE, y + 2 * CELL_SIZE / 3, EMPTY_COLOR);
            break;
        default:
            break; // No direction
    }
}


uint32_t customRand(uint32_t seed) {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed & 0x7FFFFFFF; // Keep it within the positive range
}



void resetPowerPillsFlag(void) {
    // Reset the flag to allow new power pills to be generated
    powerPillsGenerated = 0;
}
void generatePowerPill(void) {
    if (powerPillsCount >= MAX_POWER_PILLS) {
        return; // Do not generate more Power Pills than allowed
    }

    uint32_t seed = timerSeed ^ (uint32_t)LPC_TIM0->TC ^ rand();
    srand(seed); // Initialize random seed

    int row, col;
    int attempts = 0; // Limit the number of retries to avoid infinite loops
		int i;

    do {
        row = rand() % ROWS;         // Random row
        col = rand() % COLS;         // Random column

        // Ensure the position is valid: not a wall and not already a power pill
        attempts++;
    } while ((maze[row][col] != 1 || maze[row][col] == POWER_PILL) && attempts < 100);

    if (attempts >= 100) {
        return; // Exit if unable to find a valid position after multiple attempts
    }

    // Update maze
    maze[row][col] = POWER_PILL;

    // Add to powerPills array
    for (i = 0; i < MAX_ACTIVE_POWER_PILLS; i++) {
        if (!powerPills[i].active) {
            powerPills[i].row = row;
            powerPills[i].col = col;
            powerPills[i].active = 1;
            break;
        }
    }

    powerPillsCount++;              // Increment Power Pill count

    // Draw the Power Pill on the LCD
    int x = col * CELL_SIZE + 10;   // X coordinate
    int y = row * CELL_SIZE + 40;  // Y coordinate
    LCD_DrawFilledCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, 5, POWER_PILL_COLOR);
}

void redrawPowerPills(void) {
		int i;
    for (i = 0; i < MAX_ACTIVE_POWER_PILLS; i++) {
        if (powerPills[i].active) {
            int row = powerPills[i].row;
            int col = powerPills[i].col;

            int x = col * CELL_SIZE + 10;   // X coordinate
            int y = row * CELL_SIZE + 40;  // Y coordinate

            LCD_DrawFilledCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, 5, POWER_PILL_COLOR);
        }
    }
}




void drawMaze(void) {
    int row, col;

    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            int x = col * CELL_SIZE + 10;
            int y = row * CELL_SIZE + 40;

            if (maze[row][col] == 3) {
                LCD_DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, WALL_COLOR);
            } else if (maze[row][col] == 1) {
                LCD_DrawCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, 2, PILL_COLOR);
            } else {
                LCD_DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, EMPTY_COLOR);
            }
        }
    }
}

void restartGame(void) {
    // Reset game variables
    pacmanRow = 1;
    pacmanCol = 1;
    direction = -1;
    score = 0;
    lives = INITIAL_LIVES;
    countdown = 60;

    // Combine multiple dynamic sources for a robust seed
    uint32_t seed = (timerSeed ^ (uint32_t)LPC_TIM0->TC ^ (uint32_t)LPC_GPIO0->FIOPIN ^ rand());

    

    srand(seed); // Initialize random seed

    // Reset the power pills flag
    powerPillsGenerated = 0;

    // Redraw the maze and Pac-Man
    drawMaze();
    drawPacman(pacmanRow, pacmanCol);
}

void updateScoreDisplay(void) {
    char buffer[20];
    sprintf(buffer, "SCORE: %d", score);
    GUI_Text(5, 2, (uint8_t *)buffer, Green, Black);
}

void updateLivesDisplay(void) {
    char buffer[20];
    int labelX = 5;    // X position for the "LIVES:" label
    int labelY = 25;   // Y position for the "LIVES:" label (moved down to avoid overlapping time)
    int heartX = labelX + 55; // Starting X position for the first heart
    int heartY = labelY + 5;  // Y position for the hearts
    int heartSpacing = 15;    // Spacing between hearts
    int heartSize = 5;        // Size of the hearts
    int i;

    // Clear the area where the "LIVES" text and hearts are displayed
    LCD_FillRect(labelX - 5, labelY - 2, 150, 20, Black);

    // Display "LIVES:" text
    sprintf(buffer, "LIVES:");
    GUI_Text(labelX, labelY, (uint8_t *)buffer, White, Black);

    // Display hearts for remaining lives
    for (i = 0; i < lives; i++) {
        LCD_DrawFilledHeart(heartX, heartY, heartSize, Red); // Draw heart symbol
        heartX += heartSpacing; // Move to the next position for the next heart
    }
}




void updateTimeDisplay(void) {
    char buffer[20];
    sprintf(buffer, "TIME: %02d", countdown); // Format as two-digit number
    GUI_Text(150, 2, (uint8_t *)buffer, Red, Black);
}



void updateScore(int increment) {
    static int previousLifeMilestone = 0; // Track when the last life was granted

    score += increment;

    // Check if the score reached the next milestone for an extra life
    if (score / 1000 > previousLifeMilestone) {
        lives++;
        previousLifeMilestone = score / 1000;
        updateLivesDisplay();
    }

    // Update the score display
    updateScoreDisplay();

    // Check for a win condition
    checkWinCondition();
}


void countdownTimer(void) {
    static int delayCounter = 0; // Track elapsed time in milliseconds

    // Increment delay counter (assuming RIT fires every 10ms)
    delayCounter += 10;

    // Check if 1 second (1000ms) has passed
    if (delayCounter >= 1000) {
        delayCounter = 0; // Reset the counter

        if (countdown > 0) {
            countdown--;           // Decrease countdown
            updateTimeDisplay();   // Refresh the displayed number
        } else {
            // Handle "Game Over" condition
            LCD_Clear(Black);      // Clear the screen
            GUI_Text(80, 120, (uint8_t *)"Game Over!", Red, Black);
            GUI_Text(50, 140, (uint8_t *)"Press Reset Button", White, Black);
            gamePaused = 1;        // Pause the game
        }
    }
}


void checkWinCondition(void) {
    if (score >= WINNING_SCORE) {
        // Clear the screen
        LCD_Clear(Black);

        // Display the victory message
        GUI_Text(80, 120, (uint8_t *)"Victory!", Green, Black);
        GUI_Text(50, 140, (uint8_t *)"Press Reset Button", White, Black);

        // Stop the game
        gamePaused = 1;
        disable_RIT();
    }
}

void init_timer0(uint32_t interval) {
    LPC_SC->PCONP |= (1 << 1);       // Enable Timer0 power
    LPC_TIM0->TCR = 0x02;            // Reset Timer0
    LPC_TIM0->PR = 0;                // No prescaler
    LPC_TIM0->MR0 = interval;        // Match value (interval in clock cycles)
    LPC_TIM0->MCR = 0x03;            // Interrupt and reset on MR0
    LPC_TIM0->TCR = 0x01;            // Enable Timer0
    NVIC_EnableIRQ(TIMER0_IRQn);     // Enable Timer0 interrupt
}

void togglePause(void) {
    int overlayX = 70;   // X-coordinate for the overlay
    int overlayY = 100;  // Y-coordinate for the overlay
    int overlayWidth = 100; // Width of the overlay
    int overlayHeight = 50; // Height of the overlay

    if (gamePaused) {
        // Resume the game
        gamePaused = 0;

        // Fully clear the overlay box area
        LCD_FillRect(overlayX, overlayY, overlayWidth, overlayHeight, EMPTY_COLOR);

        // Redraw critical game elements
        drawMaze();                  // Redraw the maze to ensure no overlay residuals
        redrawPowerPills();          // Redraw active power pills
        drawPacman(pacmanRow, pacmanCol); // Redraw Pac-Man
        updateScoreDisplay();        // Refresh the score
        updateTimeDisplay();         // Refresh the timer
        updateLivesDisplay();        // Refresh the lives
			
				

        // Enable game updates
        init_RIT(500000);            // Reinitialize RIT for consistent updates
        enable_RIT();
				//updateDirectionAndMove();
    } else {
        // Pause the game
        gamePaused = 1;

        // Draw a dark semi-transparent overlay
        LCD_FillRect(overlayX, overlayY, overlayWidth, overlayHeight, Black);

        // Draw a border around the overlay
       //LCD_DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, White);

        // Display the "PAUSE" message in smaller font
        GUI_Text(overlayX + 30, overlayY + 20, (uint8_t *)"PAUSE", Yellow, Black);

        // Stop game updates
        disable_RIT();
    }
}

// Initialize EINT0 (Pause/Resume button)
void initEINT0(void) {
    LPC_PINCON->PINSEL4 |= (1 << 20);   // Configure P2.10 as EINT0
    LPC_SC->EXTMODE |= (1 << 0);        // Set EINT0 to edge-sensitive mode
    LPC_SC->EXTPOLAR &= ~(1 << 0);      // Set EINT0 to falling edge trigger
    NVIC_EnableIRQ(EINT0_IRQn);         // Enable EINT0 interrupt
}


// Initialize EINT1 (Speed boost)
void initEINT1(void) {
    LPC_PINCON->PINSEL4 |= (1 << 22);   // Configure P2.11 as EINT1
    LPC_SC->EXTMODE |= (1 << 1);       // Set EINT1 to edge-sensitive mode
    LPC_SC->EXTPOLAR &= ~(1 << 1);     // Set EINT1 to falling edge
    NVIC_EnableIRQ(EINT1_IRQn);        // Enable EINT1 interrupt
}

// Initialize EINT2 (Pause/Resume functionality)
void initEINT2(void) {
    LPC_PINCON->PINSEL4 |= (1 << 24);  // Configure P2.12 as EINT2
    LPC_SC->EXTMODE |= (1 << 2);       // Set EINT2 to edge-sensitive mode
    LPC_SC->EXTPOLAR &= ~(1 << 2);     // Set EINT2 to falling edge
    NVIC_EnableIRQ(EINT2_IRQn);        // Enable EINT2 interrupt
}


void EINT0_IRQHandler(void) {
    togglePause();                 // Call the function to toggle pause/resume
    LPC_SC->EXTINT |= (1 << 0);    // Clear the interrupt flag for EINT0
}


/* Main Function -------------------------------------------------------------*/
		int main(void) {
		int overlayX = 70;   // X-coordinate for the overlay
    int overlayY = 100;  // Y-coordinate for the overlay
    int overlayWidth = 100; // Width of the overlay
    int overlayHeight = 50; // Height of the overlay
    // Step 1: System Initialization
    SystemInit();                // Initialize the system
    LCD_Initialization();        // Initialize the LCD
    LCD_Clear(Black);            // Clear the screen with a black background

    // Step 2: Draw Initial Game State
    updateScoreDisplay();        // Display the initial score
    updateTimeDisplay();         // Display the initial countdown timer
    updateLivesDisplay();        // Display the initial lives
    drawMaze();                  // Draw the game maze
    drawPacman(pacmanRow, pacmanCol);  // Draw Pac-Man in the starting position
	
    // Step 4: Initialize Peripherals

    init_RIT(500000);        // Initialize the periodic timer for game updates
	//0x004C4B40
    enable_RIT();                // Enable the RIT interrupt
	  joystick_init();             // Initialize the joystick
		initEINT0();
	
		// Step 4: Set Initial Game State to PAUSE
    gamePaused = 1;              // Start in PAUSE mode
    // Draw a semi-transparent overlay for the pause message
        LCD_FillRect(overlayX, overlayY, overlayWidth, overlayHeight, Black);

        // Display the "PAUSE" message in yellow
        GUI_Text(overlayX + 30, overlayY + 20, (uint8_t *)"PAUSE", Yellow, Black);
		
    while (1) {
        __ASM("wfi"); // Wait for interrupt
    }
    return 0; // Optional, though this line is never reached
}