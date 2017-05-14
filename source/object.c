#include "object.h"

u8 objectNumber = 0;

void setObjectGfx(Object *s, uint* tiles, u32 tilesLen, uint *pal, u32 palLen){
    DC_FlushAll();
    dmaCopy(pal, SPRITE_PALETTE, palLen);
    dmaCopy(tiles + s->direction*16*16, s->gfx, tilesLen);
    s->tiles = tiles;
    s->tilesLen = tilesLen;
}

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
                sassert(0, "Error walking");
                break;
        }
        if(s->walked >= 16){
            s->walked = 0;
            s->walking = FALSE;
        }
    }
}

void updateObject(Object s){
    oamSet(s.screen, // which display
           s.id, // the oam entry to set
           s.x, s.y, // x & y location
           0, // priority
           0, // palette for 16 color sprite or alpha for bmp sprite
           s.size, // size
           s.color, // color type
           s.gfx, // the oam gfx
           -1, //affine index
           true, //double the size of rotated sprites
           false, //don't hide the sprite
           false, false, //vflip, hflip
           false //apply mosaic
          );
    dmaCopy(s.tiles + s.direction*16*16, s.gfx, s.tilesLen);
}

Object newObject(int x, int y, u8 speed, OamState* screen, SpriteSize size, SpriteColorFormat format){
    Object s;
    if(objectNumber <= SPRITE_COUNT){
        objectNumber++;
        s.id = objectNumber;
        s.x = x;
        s.y = y;
        s.gfx = oamAllocateGfx(screen, size, format);
        s.screen = screen;
        s.size = size;
        s.color = format;
        s.walking = FALSE;
        s.direction = DIR_DOWN;
        s.walked = 0;
        s.speed = speed;
    }
    return s;
}

