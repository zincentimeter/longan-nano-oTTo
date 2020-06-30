#include "lcd/lcd.h"
#include "gd32vf103.h"
#include "pregame.h"
#include "utils.h"       // Button-related function
#include "live.h"

#define RGB_BLACK 0x000000
#define RGB_WHITE 0xFFFFFF
// The following palette comes from : https://colorhunt.co/palette/191957
#define SCHEME_1 0xf4f6ff // WHITE
#define SCHEME_2 0xffcb74 // SHALLOW YELLOW
#define SCHEME_3 0xea907a // SHALLOW RED
#define SCHEME_4 0x4f8a8b // GREY GREEN
// The follwing palette comes from : https://colorhunt.co/palette/181367
#define SCHEME_5 0x162447 // DPR DLU
#define SCHEME_6 0x1f4068 // DEP BLU
#define SCHEME_7 0x1b1b2f // DKR BLU
#define SCHEME_8 0xe43f5a // BRT RED
#define THIC     6

extern struct score_report score;
extern void live();
extern void congradulate();
void detectTitle()
{
    if ( Get_BOOT0() || Get_Key(0) || Get_Key(1))
    {
        Title();
    }
}
void displayAuthor()
{
    LCD_Clear(BLACK);
    uint32_t color = RGB_BLACK;
    uint32_t firstLine = 10;
    while ( color != RGB_WHITE )
    {
        LCD_ShowString(20, firstLine, "Luxcoldury &", rgb_to_565(color));
        LCD_ShowString(20, firstLine+20, "zincentimeter", rgb_to_565(color));
        color += 0x010101;
        detectTitle();
    }
    color = RGB_BLACK;
    LCD_DrawLine(20, 55, LCD_W-20, 55, rgb_to_565(RGB_WHITE));
    while ( color <= RGB_WHITE )
    {
        LCD_ShowString(20, firstLine+50, "P R E S E N T S", rgb_to_565(color));
        color += 0x010101;
        detectTitle();
    }
    delay_1ms(2000);
    color = RGB_BLACK;
    while ( color <= RGB_WHITE )
    {
        LCD_ShowString(20, firstLine, "Luxcoldury &", rgb_to_565(RGB_WHITE-color));
        LCD_ShowString(20, firstLine+20, "zincentimeter", rgb_to_565(RGB_WHITE-color));
        LCD_DrawLine(20, 55, LCD_W-20, 55, rgb_to_565(RGB_WHITE-color));
        LCD_ShowString(20, firstLine+50, "P R E S E N T S", rgb_to_565(RGB_WHITE-color));
        color += 0x010101;
        detectTitle();
    }
}

// GO TO RISC_V
// void Title()
// {
//     displayTitleBackground();
//     displayTitleText();
// }

// GO TO RISC_V
// void displayTitleBackground()
// {
//     LCD_Clear ( rgb_to_565(SCHEME_1) );
//     LCD_Fill ( 0, 40, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_2) );
//     LCD_Fill ( 0, 60, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_3) );
//     LCD_Fill ( 0, 70, LCD_W-1, LCD_H-1, rgb_to_565(SCHEME_4) );
// }

// GO TO RISC_V
// void displayTitleText()
// {
//     int x_1, y_1, x_2, y_2;
//     int size;
//     uint32_t color = RGB_BLACK;
//     while (1)
//     {
//         delay_1ms(20);
//         color = RGB_BLACK;
//         detectBoot0();
//         while ( color <= RGB_WHITE )
//         {
//             // Capital `O`
//             x_1 = y_1 = 10;
//             size = 30;
//             LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , rgb_to_565(color) );
//             LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
//             // Under `T`
//             x_2 = 45; y_2 = 20;
//             LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , RGB_WHITE - rgb_to_565(color) );
//             LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, RGB_WHITE - rgb_to_565(color));
//             // Under `T`
//             x_2 = 75; y_2 = 20;
//             LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , RGB_WHITE - rgb_to_565(color) );
//             LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, RGB_WHITE - rgb_to_565(color));
//             // Xiaoxie `o`
//             x_1 = 105; y_1 = 20;
//             size = 20;
//             LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , rgb_to_565(color) );
//             LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
//             color += 0x010101;
//             detectBoot0();
//         }
//         delay_1ms(20);
//         detectBoot0();
//         color = RGB_BLACK;
//         while ( color <= RGB_WHITE )
//         {
//             // Capital `O`
//             x_1 = y_1 = 10;
//             size = 30;
//             LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , RGB_WHITE - rgb_to_565(color) );
//             LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
//             // Under `T`
//             x_2 = 45; y_2 = 20;
//             LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , rgb_to_565(color) );
//             LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, rgb_to_565(color));
//             // Under `T`
//             x_2 = 75; y_2 = 20;
//             LCD_Fill( x_2, y_2, x_2 + 25, y_2 + THIC , rgb_to_565(color) );
//             LCD_Fill( x_2 + 13 - THIC/2, y_2 , x_2 + 13 + THIC/2, y_2 + 40, rgb_to_565(color));
//             // Xiaoxie `o`
//             x_1 = 105; y_1 = 20;
//             size = 20;
//             LCD_Fill( x_1, y_1, x_1 + size  , y_1 + size , RGB_WHITE - rgb_to_565(color) );
//             LCD_Fill( x_1+THIC, y_1+THIC, x_1 + size -THIC, y_1 + size -THIC , WHITE);
//             color += 0x010101;
//             detectBoot0();
//         }
//     }
// }

void detectBoot0()
{
    int color = RGB_BLACK;
    if (Get_BOOT0() || Get_Key(0) || Get_Key(1))
    {
        while (color <= RGB_WHITE)
        {
            LCD_Clear(rgb_to_565(RGB_WHITE - color));
            color += 0x030303;
        }
        color = RGB_BLACK;
        while (color <= RGB_WHITE)
        {
            LCD_Clear(rgb_to_565(color));
            color += 0x030303;
        }
        displayMenu();
    }
}

#define BRT_RED 0xF02929
#define DPR_RED 0xA31B1B
#define PINK    0xF3969A
#define DPR_BRN 0x732529
#define BRT_PNK 0xF44E57
#define SLW_PNK 0xFFBABA
#define GOLD    0xEDE0 //RGB(565)
#define BRT_BLU 0x84a9ac
void displayMenu()
{
    // SONG SELECT
    LCD_Fill(0, 0, 100, 18, rgb_to_565(BRT_RED));
    LCD_Fill(1, 1, 99, 17, rgb_to_565(DPR_RED));
    BACK_COLOR = rgb_to_565(DPR_RED);
    LCD_ShowString(6, 1, "SONG SELECT", WHITE);

    // SONG FRAME
    LCD_Fill(3, 22, LCD_W-1-22, LCD_H-1-3, WHITE);
    LCD_Fill(3+2, 22+2, LCD_W-1-22-2, LCD_H-1-3-2, rgb_to_565(PINK));
    
    // SONG TITLE
    BACK_COLOR = rgb_to_565(BRT_PNK);
    LCD_Fill(7, 28, LCD_W - 1 - 27, 28 + 20, rgb_to_565(DPR_BRN));
    LCD_Fill(7, 28, LCD_W - 1 - 27, 28 + 19, rgb_to_565(BRT_PNK));
    LCD_ShowString(8, 28, "ANONE GANBARE", WHITE);
    BACK_COLOR = rgb_to_565(PINK);
    LCD_ShowString(8, 28+25, "Lily White", WHITE);
    BACK_COLOR = rgb_to_565(BRT_PNK);
    LCD_ShowString(8+100, 28+25, "110", WHITE);

    delay_1ms(3000);

    // Button A - HIGHSCORE
    LCD_Fill(7, LCD_H-1-15, 7 + 86, LCD_H-1, rgb_to_565(BRT_RED));
    LCD_Fill(8, LCD_H-1-14, 8 + 84, LCD_H-1, rgb_to_565(DPR_RED));
    BACK_COLOR = rgb_to_565(DPR_RED);
    LCD_ShowString(15, LCD_H-1-15, "HIGHSCORE", WHITE);

    // Button B - PLAY
    LCD_Fill(108, LCD_H-1-15, 108 + 42, LCD_H-1, rgb_to_565(BRT_RED));
    LCD_Fill(109, LCD_H-1-14, 109 + 40, LCD_H-1, rgb_to_565(DPR_RED));
    BACK_COLOR = rgb_to_565(DPR_RED);
    LCD_ShowString(109 + 7, LCD_H-1-15, "PLAY", WHITE);

    delay_1ms(500);

    bool buttonB_onActivity = TRUE;
    // Default to arouse Button B
    LCD_Fill(109, LCD_H-1-14, 109 + 40, LCD_H-1, rgb_to_565(SLW_PNK));
    BACK_COLOR = rgb_to_565(SLW_PNK);
    LCD_ShowString(109 + 7, LCD_H-1-15, "PLAY", WHITE);
    uint64_t timer = 0;
    while (1)
    {
        if (Get_Key(0))
        {
            if (buttonB_onActivity){
                live();
                congradulate();
                displayScore();
            }else{
                displayScore();
            }
        }
        if (Get_Key(1))
        {
            if (buttonB_onActivity)
            {
                // Button B skipped and A aroused
                LCD_Fill(109, LCD_H-1-14, 109 + 40, LCD_H-1, rgb_to_565(DPR_RED));
                BACK_COLOR = rgb_to_565(DPR_RED);
                LCD_ShowString(109 + 7, LCD_H-1-15, "PLAY", WHITE);

                buttonB_onActivity = FALSE;

                LCD_Fill(8, LCD_H-1-14, 8 + 84, LCD_H-1, rgb_to_565(SLW_PNK));
                BACK_COLOR = rgb_to_565(SLW_PNK);
                LCD_ShowString(15, LCD_H-1-15, "HIGHSCORE", WHITE);
            }
            else
            {
                // Button B aroused and A skipped
                LCD_Fill(109, LCD_H-1-14, 109 + 40, LCD_H-1, rgb_to_565(SLW_PNK));
                BACK_COLOR = rgb_to_565(SLW_PNK);
                LCD_ShowString(109 + 7, LCD_H-1-15, "PLAY", WHITE);
                
                buttonB_onActivity = TRUE;

                LCD_Fill(8, LCD_H-1-14, 8 + 84, LCD_H-1, rgb_to_565(DPR_RED));
                BACK_COLOR = rgb_to_565(DPR_RED);
                LCD_ShowString(15, LCD_H-1-15, "HIGHSCORE", WHITE);
            }
        }
        delay_1ms(200);
        timer += 200;
    }
    

}

void displayScore()
{
    while (1)
    {
        highscore:
        // HIGHSCORE
        LCD_Clear(rgb_to_565(SCHEME_5));
        LCD_Fill(0, 0, 100, 18, rgb_to_565(SCHEME_6));
        LCD_Fill(1, 1, 99, 17, rgb_to_565(SCHEME_7));
        BACK_COLOR = rgb_to_565(SCHEME_7);
        LCD_ShowString(6, 1, "HIGHSCORE", WHITE);

        // SONG FRAME
        LCD_Fill(3, 22, LCD_W-1-22, LCD_H-1-3, WHITE);
        LCD_Fill(3+2, 22+2, LCD_W-1-22-2, LCD_H-1-3-2, rgb_to_565(SCHEME_6));

        // PERFECT
        BACK_COLOR = GOLD;
        LCD_ShowString(8,28 + 15*0, " PF ", WHITE);
        BACK_COLOR = rgb_to_565(SCHEME_7);
        LCD_ShowNum(8+28, 28 + 15*0, score.perfect, 4,  WHITE);
        // NORMAL 
        BACK_COLOR = rgb_to_565(BRT_BLU);
        LCD_ShowString(8,28 + 15*1, " NM ", WHITE);
        BACK_COLOR = rgb_to_565(SCHEME_7);
        LCD_ShowNum(8+28, 28 + 15*1, score.normal, 4,  WHITE);
        // MISS 
        BACK_COLOR = rgb_to_565(BRT_RED);
        LCD_ShowString(8,28 + 15*2, " MS ", WHITE);
        BACK_COLOR = rgb_to_565(SCHEME_7);
        LCD_ShowNum(8+28, 28 + 15*2, score.miss, 4,  WHITE);

        // MAX COMBO
        BACK_COLOR = WHITE;
        LCD_ShowString(70,28 + 15*0, " MAX CB ", rgb_to_565(SCHEME_7));
        BACK_COLOR = rgb_to_565(SCHEME_6);
        LCD_ShowNum(70+10, 28+23, score.max_combo, 4, WHITE);

        delay_1ms(1000);
        BACK_COLOR = rgb_to_565(SCHEME_5);
        LCD_ShowString( LCD_W-22+5, 40-7, ">", WHITE);
        while(1)
        {
            if (Get_BOOT0() || Get_Key(0) || Get_Key(1))
            {
                // HIGHSCORE
                LCD_Clear(rgb_to_565(SCHEME_5));
                LCD_Fill(0, 0, 100, 18, rgb_to_565(SCHEME_6));
                LCD_Fill(1, 1, 99, 17, rgb_to_565(SCHEME_7));
                BACK_COLOR = rgb_to_565(SCHEME_7);
                LCD_ShowString(6, 1, "HIGHSCORE", WHITE);

                // SONG FRAME
                LCD_Fill(3, 22, LCD_W-1-22, LCD_H-1-3, WHITE);
                LCD_Fill(3+2, 22+2, LCD_W-1-22-2, LCD_H-1-3-2, rgb_to_565(SCHEME_6));

                // MAX COMBO
                BACK_COLOR = WHITE;
                LCD_ShowString(8,28 + 15*0, " - S C O R E - ", rgb_to_565(SCHEME_7));
                BACK_COLOR = rgb_to_565(SCHEME_6);
                LCD_ShowNum(35, 28+23, score.score, 8, WHITE);

                delay_1ms(1000);
                BACK_COLOR = rgb_to_565(SCHEME_5);
                LCD_ShowString( LCD_W-22+5, 40-7, "<", WHITE);
                while (1)
                {
                    if (Get_BOOT0())
                    {
                        displayMenu();
                    }
                    else
                    {
                        if (Get_Key(0) || Get_Key(1))
                        {
                            goto highscore;
                        }
                    }
                    delay_1ms(200);
                }
            }
            delay_1ms(200);
        }
    }

}

// transitionVE.S
void transitionVisual(){
    // 视效及配色极土_(:з」∠)_

    int colors_1[10] = {
        0xbc4d9b,
        0xc24795,
        0xc54291,
        0xcf3887,
        0xd62f7f,
        0xdd2777,
        0xe41f70,
        0xeb1567,
        0xf20e60,
        0xf7085a,
    };

    for(int i=0;i<10;i++){
        LCD_Fill(80-i*8-8, 40-i*4-4, 80+i*8+8-1, 40-i*4-1, rgb_to_565(colors_1[i]));
        LCD_Fill(80-i*8-8, 40+i*4, 80+i*8+8-1, 40+i*4+3, rgb_to_565(colors_1[i]));
        LCD_Fill(80-i*8-8, 40-i*4-4, 80-i*8-1, 40+i*4+3, rgb_to_565(colors_1[i]));
        LCD_Fill(80+i*8-8, 40-i*4-4, 80+i*8+8-1, 40+i*4+3, rgb_to_565(colors_1[i]));
        delay_1ms(100);
    }

    int colors_2[10] = {
        0x01b19b,
        0x0db293,
        0x1fb588,
        0x34b97b,
        0x41bb72,
        0x50bd69,
        0x5abf62,
        0x66c15a,
        0x78c450,
        0x87c747
    };

    for(int i=0;i<10;i++){
        LCD_Fill(i*8, i*4, 160-i*8-1, 80-i*4-1, rgb_to_565(colors_2[i]));
        delay_1ms(100);
    }
    
}

// Auxiliary function

uint16_t rgb_to_565 ( uint32_t rgb )
{
    return (((rgb & 0xf80000) >> 8) + ((rgb & 0xfc00) >> 5) + ((rgb & 0xf8) >> 3));
}