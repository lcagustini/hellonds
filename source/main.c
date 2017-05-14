#include <stdio.h>
#include <stdlib.h>
#include <nds.h>
#include <square.h>
#include <backTop.h>
#include <assert.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define TRUE 1
#define FALSE 0

enum Directions{DIR_NONE = 0, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};

typedef struct{
    u8 id;
    u8 x, y;
    u16* gfx;
    u8 walking;
    u8 direction;
    float walked;
    float xf, yf;
    float speed;
} Square;

u16 round(float n){
    return (u16)(n+0.5);
}

void createSquare(Square s, OamState* screen, int priority){
    oamSet(screen, // which display
            s.id, // the oam entry to set
            s.x, s.y, // x & y location
            priority, // priority
            0, // palette for 16 color sprite or alpha for bmp sprite
            SpriteSize_16x16, // size
            SpriteColorFormat_16Color, // color type
            s.gfx, // the oam gfx
            -1, //affine index
            true, //double the size of rotated sprites
            false, //don't hide the sprite
            false, false, //vflip, hflip
            false //apply mosaic
          );
}

int main(void){
    u16 Pressed;
    u16 Held;

    consoleDemoInit();
    consoleClear();

    //Top Screen Sprite Setup
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    oamInit(&oamMain, SpriteMapping_1D_128, false);
    int bgID = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);

    //Bottom Screen Sprite Setup
    /*videoSetModeSub(MODE_0_2D);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      oamInit(&oamSub, SpriteMapping_1D_32, false);
      */

    Square s = {.id = 0, .x = 50, .y = 50};
    s.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    s.walking = FALSE;
    s.direction = DIR_NONE;
    s.walked = 0;
    s.speed = 1.5;
    s.xf = s.x;
    s.yf = s.y;

    timerStart(0, ClockDivider_1024, 0, NULL);
    touchPosition* t = (touchPosition*) malloc(sizeof(touchPosition*));

    DC_FlushRange(squarePal, squarePalLen);
    dmaCopy(squarePal, SPRITE_PALETTE, squarePalLen);

    DC_FlushRange(backTopTiles, backTopTilesLen);
    dmaCopy(backTopTiles, bgGetGfxPtr(bgID), backTopTilesLen);
    DC_FlushRange(backTopMap, backTopMapLen);
    dmaCopy(backTopMap, bgGetMapPtr(bgID), backTopMapLen);
    DC_FlushRange(backTopPal, backTopPalLen);
    dmaCopy(backTopPal, BG_PALETTE, backTopPalLen);

    u16 dt;
    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();

        touchRead(t);

        dt = timerElapsed(0);

        DC_FlushRange(squareTiles, squareTilesLen);
        dmaCopy(squareTiles, s.gfx, squareTilesLen);
        createSquare(s, &oamMain, 0);

        if(!s.walking){
            if(KEY_DOWN & Held){
                s.walking = TRUE;
                s.direction = DIR_DOWN;
            }
            else if(KEY_UP & Held){
                s.walking = TRUE;
                s.direction = DIR_UP;
            }
            else if(KEY_LEFT & Held){
                s.walking = TRUE;
                s.direction = DIR_LEFT;
            }
            else if(KEY_RIGHT & Held){
                s.walking = TRUE;
                s.direction = DIR_RIGHT;
            }
        }

        if(s.walking){
            s.walked += s.speed;
            switch(s.direction){
                case DIR_UP:
                    s.yf -= s.speed;
                    s.y = round(s.yf);
                    break;
                case DIR_DOWN:
                    s.yf += s.speed;
                    s.y = round(s.yf);
                    break;
                case DIR_LEFT:
                    s.xf -= s.speed;
                    s.x = round(s.xf);
                    break;
                case DIR_RIGHT:
                    s.xf += s.speed;
                    s.x = round(s.xf);
                    break;
                default:
                    printf("Deu ruim\n");
                    break;
            }
            if(s.walked >= 16){
                s.walked = 0;
                s.walking = FALSE;
                s.direction = DIR_NONE;
            }
        }

        if(KEY_TOUCH & Pressed){
            iprintf("Tap! -- ");
            u16 index = 0b1000000000000000;
            for(u8 bit = 16; bit >= 1; bit--){
                if (Held & index) iprintf("1");
                else iprintf("0");
                index >>= 1;
            }
            iprintf("\n");
        }

        if(KEY_A & Pressed){
            consoleClear();
        }

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
