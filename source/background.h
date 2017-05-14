#ifndef NDS
#define NDS
#include <nds.h>
#endif

#include "gfx.h"

typedef struct{
    int id;
    uint *tiles, *map, *pal;
    u32 tilesLen, mapLen, palLen;
} Background;

Background newBackground(int layer, BgType type, BgSize size, int mapBase, int tileBase);

void setBackgroundGfx(Background b, gfx_t *data);
