#include <stdio.h>
#include <stdlib.h>
#include <nds.h>
#include <assert.h>
#include <math.h>

#include <zapdos.h>
#include <backTop.h>
#include <grass.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define TRUE 1
#define FALSE 0

enum Directions{DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT};

typedef struct{
    u8 id;
    u8 x, y;
    u16* gfx;
    u8 walking;
    u8 direction;
    u8 walked;
    u8 speed;
} Object;

// input is a bitfield of keys
void walk(Object *s, u16 input) {
    if(!s->walking){
        if(KEY_DOWN & input){
            s->walking = TRUE;
            s->direction = DIR_DOWN;
        }
        else if(KEY_UP & input){
            s->walking = TRUE;
            s->direction = DIR_UP;
        }
        else if(KEY_LEFT & input){
            s->walking = TRUE;
            s->direction = DIR_LEFT;
        }
        else if(KEY_RIGHT & input){
            s->walking = TRUE;
            s->direction = DIR_RIGHT;
        }
    }

    if(s->walking){
        s->walked += s->speed;
        switch(s->direction){
            case DIR_UP:
                s->y -= s->speed;
                if (s->walked > 16) {
                    s->y += s->walked - 16;
                }
                break;
            case DIR_DOWN:
                s->y += s->speed;
                if (s->walked > 16) {
                    s->y -= s->walked - 16;
                }
                break;
            case DIR_LEFT:
                s->x -= s->speed;
                if (s->walked > 16) {
                    s->x += s->walked - 16;
                }
                break;
            case DIR_RIGHT:
                s->x += s->speed;
                if (s->walked > 16) {
                    s->x -= s->walked - 16;
                }
                break;
            default:
                assert(0);
                break;
        }
        if(s->walked >= 16){
            s->walked = 0;
            s->walking = FALSE;
        }
    }
}

void updateObject(Object s, OamState* screen){
    oamSet(screen, // which display
            s.id, // the oam entry to set
            s.x, s.y, // x & y location
            0, // priority
            0, // palette for 16 color sprite or alpha for bmp sprite
            SpriteSize_32x32, // size
            SpriteColorFormat_256Color, // color type
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
    int bgID = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);

    //Bottom Screen Sprite Setup
    /*videoSetModeSub(MODE_0_2D);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      oamInit(&oamSub, SpriteMapping_1D_32, false);
      */

    Object s = {.id = 0, .x = 0, .y = 0};
    s.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    s.walking = FALSE;
    s.direction = DIR_DOWN;
    s.walked = 0;
    s.speed = 1;

    timerStart(0, ClockDivider_1024, 0, NULL);
    touchPosition* t = (touchPosition*) malloc(sizeof(touchPosition*));

    DC_FlushRange(zapdosPal, zapdosPalLen);
    dmaCopy(zapdosPal, SPRITE_PALETTE, zapdosPalLen);

    DC_FlushRange(grassTiles, grassTilesLen);
    dmaCopy(grassTiles, bgGetGfxPtr(bgID), grassTilesLen);
    DC_FlushRange(grassMap, grassMapLen);
    dmaCopy(grassMap, bgGetMapPtr(bgID), grassMapLen);
    DC_FlushRange(grassPal, grassPalLen);
    dmaCopy(grassPal, BG_PALETTE, grassPalLen);

    u16 dt;
    while(1){
        scanKeys();
        Pressed = keysDown();
        Held = keysHeld();

        touchRead(t);

        dt = timerElapsed(0);

        DC_FlushRange(zapdosTiles, 32*32);
        dmaCopy(zapdosTiles + s.direction*16*16, s.gfx, 32*32);
        updateObject(s, &oamMain);

        // handle walking
        // TODO: handle walking by scrolling the background instead of moving the player
        //       (depending on where the player is)
        walk(&s, Held);

        // this is how to scroll the background
        //bgSetScroll(bgID, 16, 16);
        //bgUpdate();

        // debug
        {
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
        }

        swiWaitForVBlank();
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);
    }

    return 0;
}
