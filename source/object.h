#ifndef NDS
#define NDS
#include <nds.h>
#endif

#include "gfx.h"
#define GRID_UNIT_SIZE 16

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
    gfx_t *gfxData;
    u8 priority;
} Object;

typedef struct{
    Object *objects[20];
    u8 objectNumber;
    u8 grid[256/GRID_UNIT_SIZE][256/GRID_UNIT_SIZE];
} World;

void walk(World *w, Object *s, u16 input);

void updateScreens();
    
Object *newObject(World *w, int x, int y, u8 speed, OamState* screen, SpriteSize size, SpriteColorFormat format, gfx_t *data);
