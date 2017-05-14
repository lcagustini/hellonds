#ifndef NDS
#define NDS
#include <nds.h>
#endif

enum Directions{DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT};

typedef struct{
    u8 id;
    u8 x, y;
    u16* gfx;
    u8 walking;
    u8 direction;
    u8 walked;
    u8 speed;
    OamState* screen;
    SpriteSize size;
    SpriteColorFormat color;
    uint *tiles;
    u32 tilesLen;
} Object;

void setObjectGfx(Object *s, uint* tiles, u32 tilesLen, uint *pal, u32 palLen);

void walk(Object *s, u16 input);

void updateObject(Object s);
    
Object newObject(int x, int y, u8 speed, OamState* screen, SpriteSize size, SpriteColorFormat format);
