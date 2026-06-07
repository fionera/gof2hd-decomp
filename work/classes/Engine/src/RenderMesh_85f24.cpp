#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" uint8_t g_Engine_shaderDrew;
extern "C" int g_Engine_defaultShader;
extern "C" void Engine_AEClientState(Engine *self, unsigned int state, bool enable);
extern "C" void Engine_ShaderSetActive(Engine *self, int shader, Mesh *mesh);
extern "C" void Engine_ShaderSetInActive(Engine *self);
extern "C" void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glNormalPointer(unsigned int type, int stride, const void *ptr);
extern "C" void glColorPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
extern "C" void glDrawArrays(unsigned int mode, int first, int count);
extern "C" void glGetIntegerv(unsigned int name, void *out);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);

extern "C" void Engine_RenderMesh(Engine *self, Mesh *mesh)
{
    void * volatile cookie = __stack_chk_guard;
    if (mesh == 0 || *(uint16_t *)((char *)mesh + 0x28) == 0) {
        goto done;
    }

    if (g_Engine_useShaders == 0) {
        glVertexPointer(3, 0x1406, 0, *(void **)((char *)mesh + 4));
        Engine_AEClientState(self, 0x8074, true);
        bool tex = ((uint32_t)*(uint8_t *)mesh << 30) < 0;
        if (tex && (*(void **)((char *)mesh + 0x30) == 0 ||
                    *(int *)(*(char **)((char *)mesh + 0x30) + 4) == -1)) {
            glTexCoordPointer(2, 0x1406, 0, *(void **)((char *)mesh + 8));
        }
        Engine_AEClientState(self, 0x8078, tex);
        bool normals = ((uint32_t)*(uint8_t *)mesh << 29) < 0;
        if (normals) {
            glNormalPointer(0x1406, 0, *(void **)((char *)mesh + 0x10));
        }
        Engine_AEClientState(self, 0x8075, normals);
        bool colors = ((uint32_t)*(uint8_t *)mesh << 28) < 0;
        if (colors) {
            glColorPointer(4, 0x1406, 0, *(void **)((char *)mesh + 0x0c));
        }
        Engine_AEClientState(self, 0x8076, colors);
        if (((uint32_t)*(uint8_t *)mesh << 27) < 0) {
            glDrawElements(4, *(uint16_t *)((char *)mesh + 0x28), 0x1403,
                           *(void **)((char *)mesh + 0x2c));
        } else {
            glDrawArrays(4, 0, *(uint16_t *)((char *)mesh + 2));
        }
        if (tex && *(void **)((char *)mesh + 0x30) != 0 &&
            *(int *)(*(char **)((char *)mesh + 0x30) + 4) != -1) {
            Engine_AEClientState(self, 0x8078, false);
        }
    } else {
        g_Engine_shaderDrew = 0;
        Engine_ShaderSetActive(self, g_Engine_defaultShader, mesh);
        if (g_Engine_shaderDrew != 0) {
            int oldBuffer = 0;
            glGetIntegerv(0x8ca6, &oldBuffer);
            if (((uint32_t)*(uint8_t *)mesh << 27) < 0) {
                if (*(uint8_t *)((char *)mesh + 0x5c) == 0) {
                    glDrawElements(4, *(uint16_t *)((char *)mesh + 0x28), 0x1403,
                                   *(void **)((char *)mesh + 0x2c));
                } else {
                    glBindBuffer(0x8893, *(uint32_t *)((char *)mesh + 0x64));
                    glDrawElements(4, *(uint16_t *)((char *)mesh + 0x28), 0x1403, 0);
                    glBindBuffer(0x8892, 0);
                    glBindBuffer(0x8893, 0);
                }
            } else {
                glDrawArrays(4, 0, *(uint16_t *)((char *)mesh + 2));
            }
            Engine_ShaderSetInActive(self);
        }
    }

done:
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
