#ifdef __cplusplus
    extern "C"
    {
#endif

    #include "lcd/lcd.h"     // Screen display functions
    #include "utils.h"       // Button-related function
    #include "init.h"        // device initializing function
    #include "pregame.h"
    #include "live.h"
    #include "liveS.h"
    // #include "hit_image.h"   // Image of hit text!


#ifdef __cplusplus
    }
#endif

#define FRAME_TIME (60000.0/110/4/5)  // 136/5 = 27, 5 frames per shortest note interval
// #define ONE_MS     (SystemCoreClock/4000.0)

#define LCD_MARGIN_OFFSET 15



// typedef uint16_t rgb565;
// static struct viewBuffer
// {
//     rgb565 buffer[LCD_H][LCD_W];
//     bool A[2];
// };

// uint64_t hitBias = 30; //existing bug : set to 0 , and green crashes

uint64_t lineLocation;
uint64_t currentTime, initTime;
uint64_t noteIndex = 0;
uint64_t frameCount = 0;
uint64_t drawIndex[2] = {0, 0}; // hitIndex[2] = {0, 0};
uint64_t hitLocation = 0;

uint64_t noteDisplayOffset = 30; // Not bigger than 160 otherwise bad
uint64_t noteDrawStartFrame, noteDrawStartTime, noteDrawEndFrame, noteDrawEndTime;
uint64_t noteHitStartFrame, noteHitStartTime, noteHitEndFrame, noteHitEndTime;
uint64_t noteLongDrawInit[2] = {0, 0}, 
         noteLongHitInit[2] = {0, 0},
         noteLongHitInitFrame[2] = {0, 0},
         noteLongFrameOffset = 0;
uint16_t noteLocation;
uint64_t key;

// global var for judge
uint16_t note_to_judge[2];
uint8_t longpressing[2];

struct score_report score;

uint64_t frameEndTime;


// void sineDisplay(void);
void live(void);
uint64_t getCurrentTime();
void judge(int8_t channelNo);
int judgePrecise(uint64_t targetTime);
void congradulate();

int main(void)
{
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
    gpio_bit_reset(GPIOA, GPIO_PIN_0);

    initHardware();
    // displayScore();
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
    gpio_bit_reset(GPIOA, GPIO_PIN_0);

    displayAuthor();
    Title();
    transitionVE();
    // BACK_COLOR = WHITE;
    // LCD_ShowNum(0,0,score.score,7,BLACK);
    // LCD_ShowNum(0,16,score.max_combo,4,BLACK);
    // LCD_ShowNum(0,32,score.perfect,4,BLACK);
    // LCD_ShowNum(0,48,score.normal,4,BLACK);
    // LCD_ShowNum(0,64,score.miss,4,BLACK);
    while(1){}
    
}

// void sineDisplay(void)
// {
//     LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
//     for ( int i = 0; i < LCD_W*LCD_H; ++i )
//     {
//         LCD_WR_DATA8(imageInput[i%(8192)]);
//     }
// }
// -------------- COLOR -------------
#define BACKGROUND         (BLACK)
#define BACKGROUND_LINE    (BLACK)
#define COMPLEMENTARY      (WHITE)
#define SKYBLUE     0x65DB
#define DEEPBLUE    0x5C94
#define SHALLOWBLUE 0x75DA
#define GOLD        0xEDE0
// const hitColor[] = { GREEN };
// const static colorScheme[][2] = 
// {
//     {SKYBLUE, DEEPBLUE},
//     {}
// }
// ------- DISPLAY COORDINATE -------
#define BUTTON_WIDTH       (4)
#define BUTTON_INNER_WIDTH (2)
#define KEY_Y_START        (15+36*key)
#define KEY_Y_END          (30+36*key)
#define CHAIN_Y_START      (KEY_Y_START+2)
#define CHAIN_Y_END        (KEY_Y_END-2)

#define LINE_LENGTH 13
#define FRESH_PIXEL 32

const char * hitText[]  = {" \\PERFECT!/ ", " F I N E ", " M I S S "};
const int    hitColor[] = {GOLD, BLUE,     RED};
void drawNote ( uint64_t noteLocation, uint64_t key )
{
    
    LCD_Fill (      noteLocation - BUTTON_WIDTH,       KEY_Y_START,                      noteLocation + BUTTON_WIDTH,       KEY_Y_END,                      SKYBLUE  );
    LCD_Fill (      noteLocation - BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, noteLocation + BUTTON_INNER_WIDTH, KEY_Y_END - BUTTON_INNER_WIDTH, DEEPBLUE );

    LCD_DrawPoint ( noteLocation - BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, SKYBLUE );
    LCD_DrawPoint ( noteLocation - BUTTON_INNER_WIDTH, KEY_Y_END   - BUTTON_INNER_WIDTH, SKYBLUE );
    LCD_DrawPoint ( noteLocation + BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, SKYBLUE );
    LCD_DrawPoint ( noteLocation + BUTTON_INNER_WIDTH, KEY_Y_END   - BUTTON_INNER_WIDTH, SKYBLUE );

    LCD_DrawPoint ( noteLocation - BUTTON_WIDTH, KEY_Y_START, BACKGROUND );
    LCD_DrawPoint ( noteLocation - BUTTON_WIDTH, KEY_Y_END,   BACKGROUND );
    LCD_DrawPoint ( noteLocation + BUTTON_WIDTH, KEY_Y_START, BACKGROUND );
    LCD_DrawPoint ( noteLocation + BUTTON_WIDTH, KEY_Y_END,   BACKGROUND );
}

void drawChain ( uint64_t noteLocation, uint64_t key )
{
    LCD_Fill (      noteLocation,   CHAIN_Y_START,   noteLocation ,  CHAIN_Y_END,   DEEPBLUE    );
    LCD_Fill (      noteLocation+2, CHAIN_Y_START+2, noteLocation+2, CHAIN_Y_END-2, SHALLOWBLUE );
    LCD_DrawPoint ( noteLocation+2, CHAIN_Y_START,   DEEPBLUE );
    LCD_DrawPoint ( noteLocation+2, CHAIN_Y_END,     DEEPBLUE );
}

// #define HALO_WIDTH 1
void drawFeedback ( uint64_t noteLocation, uint64_t key, bool isMiddle, int accuracy )
{
    BACK_COLOR = COMPLEMENTARY;
    LCD_ShowNum(0 , FRESH_PIXEL+2, score.combo, 5, BACKGROUND); // LEAVE FOR HIGHSCORE
    // LCD_Fill( LCD_W/2-2, FRESH_PIXEL+2, LCD_W/2+17, FRESH_PIXEL+2+15, BACKGROUND);
    LCD_ShowString( LCD_W/2 - 24 , FRESH_PIXEL+2, hitText[accuracy], hitColor[accuracy]);
    // LCD_ShowString();
    if (accuracy != 2) //Not miss
    {
        if ( !isMiddle )
        {
            LCD_Fill (      noteLocation - BUTTON_WIDTH,       KEY_Y_START,                      noteLocation + BUTTON_WIDTH,       KEY_Y_END,                      GREEN  );
            LCD_Fill (      noteLocation - BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, noteLocation + BUTTON_INNER_WIDTH, KEY_Y_END - BUTTON_INNER_WIDTH, GREEN );

            LCD_DrawPoint ( noteLocation - BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, GREEN );
            LCD_DrawPoint ( noteLocation - BUTTON_INNER_WIDTH, KEY_Y_END   - BUTTON_INNER_WIDTH, GREEN );
            LCD_DrawPoint ( noteLocation + BUTTON_INNER_WIDTH, KEY_Y_START + BUTTON_INNER_WIDTH, GREEN );
            LCD_DrawPoint ( noteLocation + BUTTON_INNER_WIDTH, KEY_Y_END   - BUTTON_INNER_WIDTH, GREEN );

            LCD_DrawPoint ( noteLocation - BUTTON_WIDTH, KEY_Y_START, BACKGROUND );
            LCD_DrawPoint ( noteLocation - BUTTON_WIDTH, KEY_Y_END,   BACKGROUND );
            LCD_DrawPoint ( noteLocation + BUTTON_WIDTH, KEY_Y_START, BACKGROUND );
            LCD_DrawPoint ( noteLocation + BUTTON_WIDTH, KEY_Y_END,   BACKGROUND );
        }
        else
        {
            LCD_Fill (      noteLocation,   CHAIN_Y_START,   noteLocation ,  CHAIN_Y_END,   GREEN );
            LCD_Fill (      noteLocation+2, CHAIN_Y_START+2, noteLocation+2, CHAIN_Y_END-2, GREEN );
            LCD_DrawPoint ( noteLocation+2, CHAIN_Y_START,   GREEN );
            LCD_DrawPoint ( noteLocation+2, CHAIN_Y_END,     GREEN );
        }
    }
}

// #define TRIANGLE_HIGH   (10-TRIANGLE_SHADOW)
// #define TRIANGLE_SHADOW (3)

void lineUpdate ( uint64_t * lineLocation )
{
    LCD_Fill ( (*lineLocation - 15), 0, (*lineLocation - 10), FRESH_PIXEL-1, BACKGROUND_LINE );
    LCD_Fill ( (*lineLocation),      0, (*lineLocation),      LINE_LENGTH-1, BACKGROUND_LINE);
 
    LCD_Fill ( (*lineLocation - 15), LCD_H - FRESH_PIXEL + 3,     (*lineLocation - 10), LCD_H-1, BACKGROUND_LINE );
    LCD_Fill ( (*lineLocation - 5),  LCD_H - LINE_LENGTH, (*lineLocation),      LCD_H-1, BACKGROUND_LINE );
    * lineLocation = ( frameCount*4 ) % ( LCD_W ) + LCD_MARGIN_OFFSET;
    
    // for (int i = 0; i < TRIANGLE_HIGH; ++i)
    // {
    //     LCD_DrawLine( *lineLocation+i, 0, *lineLocation+i, TRIANGLE_HIGH-i+TRIANGLE_SHADOW*2, COMPLEMENTARY);
    //     LCD_DrawLine( *lineLocation+i, TRIANGLE_HIGH-i, *lineLocation+i, TRIANGLE_HIGH-i+TRIANGLE_SHADOW, RED);
    //     LCD_DrawLine( *lineLocation+i, LCD_H-(TRIANGLE_HIGH-i+TRIANGLE_SHADOW*2), *lineLocation+i, LCD_H, COMPLEMENTARY);
    //     LCD_DrawLine( *lineLocation+i, LCD_H-(TRIANGLE_HIGH-i+TRIANGLE_SHADOW), *lineLocation+i, LCD_H-(TRIANGLE_HIGH-i) , BLUE);
    // }
    // LCD_Fill( *lineLocation-1, 0, *lineLocation+1, TRIANGLE_HIGH-2, COMPLEMENTARY );
    LCD_Fill( *lineLocation, 0,                    *lineLocation, LINE_LENGTH-1, BLUE );
    LCD_Fill( *lineLocation, LCD_H -  LINE_LENGTH, *lineLocation, LCD_H-1,       RED );
}


void live(void)
{
    // noteGen();

    // flush lcd
    LCD_Clear(BLACK);

    // gpio_bit_write(GPIOB, GPIO_PIN_7,SET);

    noteIndex = 0;
    frameCount = 0;
    drawIndex[0] = 0;
    drawIndex[1] = 0;
    note_to_judge[0] = 0;
    note_to_judge[1] = 0;
    hitLocation = 0;

    score.combo=0;
    score.combo_score=0;
    score.max_combo=0;
    score.miss=0;
    score.normal=0;
    score.perfect=0;
    score.score=0;

    noteDisplayOffset = 30; // Not bigger than 160 otherwise bad
    uint16_t noteLocation;
    uint64_t key;


    lineLocation = 0;
    // Move position per frame : move by the distance LCD_W per sec.
    // lineSpeed = (LCD_W)/(FRAME_TIME); 
    LCD_Fill(0, FRESH_PIXEL+1, LCD_W-1, LCD_H-FRESH_PIXEL+2, RED);
    LCD_Fill(0, FRESH_PIXEL+2, LCD_W-1, LCD_H-FRESH_PIXEL+1, COMPLEMENTARY);

    gpio_bit_set(GPIOA, GPIO_PIN_0);
    
    initTime = getCurrentTime();
    do
    {

        // Graphics
        // Draw fresh line
        lineUpdate(&lineLocation);

        if(frameCount>50) gpio_bit_reset(GPIOA, GPIO_PIN_0);

        // Iterate for each key
        for (key = 0; key < NOTE_KEY; ++key)
        {
            // record current note info FOR convenience
            noteDrawStartFrame = note[key][drawIndex[key]].frame_start;
            noteDrawEndFrame = note[key][drawIndex[key]].frame_end;
            if(noteDrawStartFrame==0) continue;
            // calculate the place going to be drawn
            noteLocation = ((frameCount + noteDisplayOffset) * 4) % (LCD_W) + LCD_MARGIN_OFFSET;

            if ( noteDrawStartFrame <= (frameCount + noteDisplayOffset) )
            {  
                if (noteDrawStartFrame == noteDrawEndFrame) // single note
                {
                    drawNote(noteLocation, key);
                    drawIndex[key]++;
                }
                else // LongNote
                {
                    if (noteLongDrawInit[key] == 0) // Begin to draw
                    {
                        drawNote(noteLocation, key);
                        noteLongDrawInit[key] = 1;
                    }
                    else // On the way or the end
                    {
                        if (noteDrawEndFrame <= (frameCount + noteDisplayOffset)) // already done
                        {
                            drawNote(noteLocation, key);
                            noteLongDrawInit[key] = 0;
                            drawIndex[key]++;
                        }
                        else // the long note is not done yet
                        {
                            // Middle note, just print shorter one
                            drawChain( noteLocation, key );
                        }
                    }
                }
            }
        }

        // Monitoring buttons until next frame comes
        frameEndTime = initTime + (frameCount+1)*FRAME_TIME;
        do
        {   
            judge(0);
            judge(1);
        }
        while (getCurrentTime() < frameEndTime);
        // update score & max_combo
        score.combo_score += score.combo;
        score.score = score.perfect*13 + score.normal*7 + score.combo_score;
        score.max_combo = score.max_combo>score.combo ? score.max_combo : score.combo;
        frameCount++;
    }while(frameCount<MAX_FRAME);
}



uint64_t getCurrentTime()
{
    return (get_timer_value()*4000)/SystemCoreClock;
}

void judge(int8_t channelNo){
    uint16_t note_start_frame = note[channelNo][note_to_judge[channelNo]].frame_start;
    uint16_t note_end_frame = note[channelNo][note_to_judge[channelNo]].frame_end;
    if(!note_start_frame) return; // skip last note
    int isLongNote = note_start_frame != note_end_frame;
    
    // prepare argu for feedback draw
    int hitLocationHEAD   = (note_start_frame*4) % (LCD_W) + LCD_MARGIN_OFFSET;
    int hitLocationMIDDLE;
    int hitLocationEND;
    if(isLongNote){
        hitLocationMIDDLE = ((frameCount+2)*4) % (LCD_W) + LCD_MARGIN_OFFSET;
        hitLocationEND    = (note_end_frame*4) % (LCD_W) + LCD_MARGIN_OFFSET;
    }
    // judge!
    uint64_t targetTime = !(isLongNote && longpressing[channelNo]) ? (note_start_frame*FRAME_TIME + initTime) : (note_end_frame*FRAME_TIME + initTime); // 单押目标时间，长押头目标时间，长押尾目标时间
    int judgePreciseResult = judgePrecise(targetTime);

    // deal with judge result
    if(Get_Key(channelNo)){
        if(!isLongNote){
            // 单押判定
            if(judgePreciseResult){
                switch (judgePreciseResult){
                    case 1:
                    case 4:
                        score.miss++;
                        score.combo=0;
                        drawFeedback(hitLocationEND, channelNo, FALSE, 2);
                        break;
                    case 2:
                        score.normal++;
                        score.combo++;
                        drawFeedback(hitLocationHEAD, channelNo, FALSE, 1);
                        break;
                    case 3:
                        score.perfect++;
                        score.combo++;
                        drawFeedback(hitLocationHEAD, channelNo, FALSE, 0);
                        break;
                }
                note_to_judge[channelNo]++;
                longpressing[channelNo]=0;
            }
        }else{
            if(!longpressing[channelNo]){
                // 长押头判
                switch (judgePreciseResult){
                    case 0:
                        break;
                    case 1:
                    case 4:
                        score.miss++;
                        score.combo=0;
                        note_to_judge[channelNo]++;
                        longpressing[channelNo]=0;
                        drawFeedback(hitLocationEND, channelNo, FALSE, 2);
                        break;
                    case 2:
                        score.normal++;
                        score.combo++;
                        drawFeedback(hitLocationHEAD, channelNo, FALSE, 1);
                        longpressing[channelNo]=1;
                        break;
                    case 3:
                        score.perfect++;
                        score.combo++;
                        drawFeedback(hitLocationHEAD, channelNo, FALSE, 0);
                        longpressing[channelNo]=1;
                        break;
                }
            }else{
                // 长押中按
                if(judgePreciseResult==4){
                    // 中按超时miss
                    score.miss++;
                    score.combo=0;
                    note_to_judge[channelNo]++;
                    longpressing[channelNo]=0;
                    drawFeedback(hitLocationEND, channelNo, TRUE, 2);
                }else{
                    drawFeedback(hitLocationMIDDLE, channelNo, TRUE, 0);
                }
            }
        }
    }else{
        if(!isLongNote){
            // 单押的后miss
            if(judgePreciseResult==4){
                score.miss++;
                score.combo=0;
                note_to_judge[channelNo]++;
                longpressing[channelNo]=0;
                drawFeedback(hitLocationEND, channelNo, FALSE, 2);
            }
        }else{
            if(longpressing[channelNo]){
                // 长押尾判
                // prepare 尾判 argu for feedback draw
                hitLocation = ( ( (note_end_frame ) *4 ) % (LCD_W) + LCD_MARGIN_OFFSET );
                switch (judgePreciseResult){
                    case 0:
                    case 1:
                    case 4:
                        score.miss++;
                        score.combo=0;
                        drawFeedback(hitLocationEND, channelNo, FALSE, 2);
                        break;
                    case 2:
                        score.normal++;
                        score.combo++;
                        drawFeedback(hitLocationEND, channelNo, FALSE, 1);
                        break;
                    case 3:
                        score.perfect++;
                        score.combo++;
                        drawFeedback(hitLocationEND, channelNo, FALSE, 0);
                        break;
                }
                note_to_judge[channelNo]++;
                longpressing[channelNo]=0;
            }else{
                // 长押头的后miss
                if(judgePreciseResult==4){
                    score.miss++;
                    score.combo=0;
                    note_to_judge[channelNo]++;
                    longpressing[channelNo]=0;
                }
            }
        }
    }
}

#define MISS_DIFF 82
#define NORMAL_DIFF 81
#define PERFECT_DIFF 80

// 0 先于前miss
// 1 前miss
// 2 normal
// 3 perfect
// 4 后miss
int judgePrecise(uint64_t targetTime){
    uint64_t currentTime = getCurrentTime();
    int16_t diffTime = currentTime - targetTime;
    if(diffTime<-MISS_DIFF){return 0;}
    if(diffTime<-NORMAL_DIFF){return 1;}
    if(diffTime<-PERFECT_DIFF){return 2;}
    if(diffTime<PERFECT_DIFF){return 3;}
    if(diffTime<NORMAL_DIFF){return 2;}else{return 4;}
}

void congradulate(){
    int i;

	LCD_Address_Set(0,0,159,79);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(liveSs2[i]);
	}		

    delay_1ms(150);

	LCD_Address_Set(0,0,159,79);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(liveSs1[i]);
	}		

    delay_1ms(150);

	LCD_Address_Set(0,0,159,79);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(liveS[i]);
	}		

    delay_1ms(150);

	LCD_Address_Set(0,0,159,79);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(liveSl1[i]);
	}		

    delay_1ms(150);

	LCD_Address_Set(0,0,159,79);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(liveS[i]);
	}		

    delay_1ms(1250);
}