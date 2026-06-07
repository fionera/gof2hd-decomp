#include "class.h"

void Trail::changeType(int type)
{
    switch (type) {
    case 1:
    case 7:
    case 11:
        return Trail_transformSetColor(*gTrailCanvasType1, u32(this, 0x14), 0xff0000ffu);
    case 2:
    case 9:
        return Trail_transformSetColor(*gTrailCanvasType2, u32(this, 0x14), 0x00ff00ffu);
    case 3:
    case 6:
    case 10:
        return Trail_transformSetColor(*gTrailCanvasType3, u32(this, 0x14), 0xffff00ffu);
    case 5:
        return Trail_transformSetColor(*gTrailCanvasType5, u32(this, 0x14), 0x00ff0000u);
    case 8:
        return Trail_transformSetColor(*gTrailCanvasType8, u32(this, 0x14), 0xff4000ffu);
    default:
        return Trail_transformSetColor(*gTrailCanvasTypeDefault, u32(this, 0x14), 0xffffffffu);
    }
}
