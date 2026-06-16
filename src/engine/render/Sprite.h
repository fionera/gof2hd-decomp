#ifndef GOF2_SPRITE_H
#define GOF2_SPRITE_H
#include <cstdint>

/// 2D sprite: either a single image, or an array of frame image handles laid
/// out as a grid (columns x rows) that can be animated frame by frame.
class Sprite {
public:
    uint32_t *frames;       ///< frame image handle array (null => single image)
    uint32_t  image;        ///< single image handle (-1 when using frame array)
    int32_t   refPixelX;    ///< reference pixel X
    int32_t   refPixelY;    ///< reference pixel Y
    int32_t   posX;         ///< position X
    int32_t   posY;         ///< position Y
    int32_t   frameWidth;   ///< frame width
    int32_t   frameHeight;  ///< frame height
    int32_t   imageWidth;   ///< source image width
    int32_t   imageHeight;  ///< source image height
    int32_t   frameSrcX;    ///< current frame source X
    int32_t   frameSrcY;    ///< current frame source Y
    int32_t   columns;      ///< grid columns
    int32_t   rows;         ///< grid rows
    int32_t   currentFrame; ///< current frame index
    int32_t   frameCount;   ///< total frame count

    Sprite(uint32_t image, int frameWidth, int frameHeight);
    Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight);
    ~Sprite();

    void setPosition(int x, int y);
    void setRefPixelPosition(int x, int y);
    void defineReferencePixel(int x, int y);
    void setFrame(int frame);
    void nextFrame();
    void prevFrame();
    void draw(float scaleX, float scaleY);
    void drawRegion(int srcX, int srcY, int w, int h);
    int getFrameWidth();
    int getFrameHeight();
};
#endif
