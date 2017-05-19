#ifndef NDS
#define NDS
#include <nds.h>
#endif

#include "gfx.h"

typedef struct{
    int id;
    gfx_t *data;
    u8 layer;
} Background;

Background newBackground(int layer, gfx_t *data, BgType type, BgSize size, int mapBase, int tileBase);

void setBackgroundGfx(Background b, int offset);
