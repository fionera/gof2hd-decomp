#include "class.h"

extern "C" {
long long ApplicationManager_GetCurrentTimeMillis();
float __aeabi_l2f(long long v);
float sinf(float x);
extern float PulseShader_timeScale;
}

namespace AbyssEngine {

void PulseShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->f_34 >= 0)
        glUniformMatrix4fv(this->f_34, 1, 0, (float *)((char *)engine + 0x104));
    if (this->f_38 >= 0)
        glUniformMatrix3fv(this->f_38, 1, 0, (float *)((char *)engine + 0x204));

    if (this->f_9 != 0) {
        if (this->f_3c >= 0)
            glUniform3f(this->f_3c, engine->f_330, engine->f_334,
                        engine->f_338);
        if (this->f_40 >= 0)
            glUniform3f(this->f_40, engine->f_34c, engine->f_350,
                        engine->f_354);
        if (this->f_48 >= 0)
            glUniform4fv(this->f_48, 1, (float *)((char *)engine + 0xd0));
        if (this->f_4c >= 0)
            glUniform4fv(this->f_4c, 1, (float *)((char *)engine + 0x2a8));
        if (this->f_50 >= 0)
            glUniform4fv(this->f_50, 1, (float *)((char *)engine + 0x298));
        if (this->f_54 >= 0)
            glUniform4fv(this->f_54, 1, (float *)((char *)engine + 0x2b8));

        long long t = ApplicationManager_GetCurrentTimeMillis();
        float v = sinf(__aeabi_l2f(t) / PulseShader_timeScale);
        glUniform1f(this->f_58, v + 2.0f);
        this->f_9 = 0;
    }

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    if (this->f_20 >= 0)
        enableVertexAttribArray(this->f_20);
    if (this->f_24 >= 0)
        enableVertexAttribArray(this->f_24);
    if (this->f_28 >= 0)
        enableVertexAttribArray(this->f_28);
    if (this->f_2c >= 0)
        enableVertexAttribArray(this->f_2c);
    if (this->f_30 >= 0)
        enableVertexAttribArray(this->f_30);

    if (mesh->f_5c == 0) {
        if (this->f_20 >= 0)
            glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x4));
        if (this->f_24 >= 0)
            glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x8));
        if (this->f_28 >= 0)
            glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x10));
        if (this->f_2c >= 0)
            glVertexAttribPointer(this->f_2c, 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x14));
        if (this->f_30 >= 0)
            glVertexAttribPointer(this->f_30, 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x18));
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, mesh->f_60);
        vertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->f_68);
        vertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->f_6c);
        vertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->f_70);
        vertexAttribPointer(this->f_2c, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, mesh->f_74);
        vertexAttribPointer(this->f_30, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
