#ifndef PREGAME_H
#define PREGAME_H

void displayAuthor();
void Title();
void displayMenu();
void displayScore();
void proceed_to_scoreboard();
void displayTitleBackground();
void displayTitleText();
// Auxiliary function
uint16_t rgb_to_565 ( uint32_t rgb );

#endif