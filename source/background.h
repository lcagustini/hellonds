#ifndef NDS
#define NDS
#include <stdio.h>
#include <nds.h>
#endif

typedef struct{
    int id;
    uint *tiles, *map, *pal;
    u32 tilesLen, mapLen, palLen;
} Background;

Background newBackground(int layer, BgType type, BgSize size, int mapBase, int tileBase);

void setBackgroundGfx(Background* b, uint* tiles, u32 tilesLen, uint* map, u32 mapLen, uint* pal, u32 palLen);
