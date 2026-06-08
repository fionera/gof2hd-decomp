#include "class.h"

extern "C" int g_Engine_lineShader;
extern "C" void Engine_ShaderSetActive(Engine *self, int shader, Mesh *mesh);
extern "C" void glDrawArrays(unsigned int mode, int first, int count);

extern "C" void Engine_DrawLine2D(Engine *self, int vertexCount, int count, bool strip)
{
    *(int *)((char *)self + 0x348) = vertexCount;
    Engine_ShaderSetActive(self, g_Engine_lineShader, 0);
    unsigned int mode = strip != 0 ? 2 : 1;
    return glDrawArrays(mode, 0, count);
}
