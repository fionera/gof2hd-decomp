#include "gof2/engine/render/shaders/VertexColorShader.h"
#include "gof2/platform/gl.h"

// VertexColorShader's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" char VertexColorShader_vtable[];

namespace AbyssEngine {

VertexColorShader::VertexColorShader()
{
    this->vtable = VertexColorShader_vtable + 8;
    this->name.s = u"VertexColorShader";
}

// Compiles the vertex-colour program and caches its attribute/uniform locations.
void VertexColorShader::Init()
{
    this->program = this->ES2LoadProgram("vtx_color_vs", "vtx_color_fs");

    this->aColor = glGetAttribLocation(this->program, "color");
    this->aPosition = glGetAttribLocation(this->program, "position");
    this->aTexCoord = glGetAttribLocation(this->program, "texcoord");
    this->aNormal = glGetAttribLocation(this->program, "normal");
    this->aTangent = glGetAttribLocation(this->program, "tangent");
    this->aBiNormal = glGetAttribLocation(this->program, "binormal");

    this->uWorldViewProj = glGetUniformLocation(this->program, "worldViewProj");
    this->uNormalMatrix = glGetUniformLocation(this->program, "normalMatrix");
    this->uLightDir = glGetUniformLocation(this->program, "lightDir");
    this->uLightColor = glGetUniformLocation(this->program, "lightColor");
    this->uAmbientColor = glGetUniformLocation(this->program, "ambientColor");
    this->uMaterialDiffuse = glGetUniformLocation(this->program, "materialDiffuse");
    this->uMaterialAmbient = glGetUniformLocation(this->program, "materialAmbient");
    this->uMaterialSpecular = glGetUniformLocation(this->program, "materialSpecular");
    this->uMaterialShininess = glGetUniformLocation(this->program, "materialShininess");

    glUseProgram(this->program);
    glUniform1i(this->uAmbientColor, 0);
}

// Binds the program's matrices, lighting/material uniforms (when dirty) and the mesh's
// vertex attribute arrays. Engine/Mesh fields are reached by raw offset (deferred to a
// later Engine/Mesh-modelling pass).
void VertexColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uWorldViewProj >= 0) {
        glUniformMatrix4fv(this->uWorldViewProj, 1, false, (float *)((char *)engine + 0x104));
    }
    if (this->uNormalMatrix >= 0) {
        glUniformMatrix3fv(this->uNormalMatrix, 1, false, (float *)((char *)engine + 0x204));
    }

    if (this->dirty != 0) {
        if (this->uLightDir >= 0) {
            float *lightDir = (float *)((char *)engine + 0x330);
            glUniform3f(this->uLightDir, lightDir[0], lightDir[1], lightDir[2]);
        }
        if (this->uLightColor >= 0) {
            float *lightColor = (float *)((char *)engine + 0x34c);
            glUniform3f(this->uLightColor, lightColor[0], lightColor[1], lightColor[2]);
        }
        if (this->uMaterialDiffuse >= 0) {
            glUniform4fv(this->uMaterialDiffuse, 1, (float *)((char *)engine + 0xd0));
        }
        if (this->uMaterialAmbient >= 0) {
            glUniform4fv(this->uMaterialAmbient, 1, (float *)((char *)engine + 0x2a8));
        }
        if (this->uMaterialSpecular >= 0) {
            glUniform4fv(this->uMaterialSpecular, 1, (float *)((char *)engine + 0x298));
        }
        if (this->uMaterialShininess >= 0) {
            glUniform4fv(this->uMaterialShininess, 1, (float *)((char *)engine + 0x2b8));
        }
        this->dirty = 0;
    }

    if (this->aPosition >= 0) {
        glEnableVertexAttribArray(this->aPosition);
    }
    if (this->aTexCoord >= 0) {
        glEnableVertexAttribArray(this->aTexCoord);
    }
    if (this->aNormal >= 0) {
        glEnableVertexAttribArray(this->aNormal);
    }
    if (this->aTangent >= 0) {
        glEnableVertexAttribArray(this->aTangent);
    }
    if (this->aBiNormal >= 0) {
        glEnableVertexAttribArray(this->aBiNormal);
    }
    if (this->aColor >= 0) {
        glEnableVertexAttribArray(this->aColor);
    }

    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, false, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, false, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x6c));
        glVertexAttribPointer(this->aNormal, 3, 0x1406, false, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x70));
        glVertexAttribPointer(this->aTangent, 3, 0x1406, false, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x74));
        glVertexAttribPointer(this->aBiNormal, 3, 0x1406, false, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x78));
        glVertexAttribPointer(this->aColor, 4, 0x1406, false, 0, 0);
        return;
    }

    if (this->aPosition >= 0) {
        glVertexAttribPointer(this->aPosition, 3, 0x1406, false, 0, *(void **)((char *)mesh + 4));
    }
    if (this->aTexCoord >= 0) {
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, false, 0, *(void **)((char *)mesh + 8));
    }
    if (this->aNormal >= 0) {
        glVertexAttribPointer(this->aNormal, 3, 0x1406, false, 0, *(void **)((char *)mesh + 0x10));
    }
    if (this->aTangent >= 0) {
        glVertexAttribPointer(this->aTangent, 3, 0x1406, false, 0, *(void **)((char *)mesh + 0x14));
    }
    if (this->aBiNormal >= 0) {
        glVertexAttribPointer(this->aBiNormal, 3, 0x1406, false, 0, *(void **)((char *)mesh + 0x18));
    }
    if (this->aColor >= 0) {
        glVertexAttribPointer(this->aColor, 4, 0x1406, false, 0, *(void **)((char *)mesh + 0x0c));
    }
}

// Disables all of this shader's vertex attribute arrays.
void VertexColorShader::SetInActive()
{
    if (this->aPosition >= 0) {
        glDisableVertexAttribArray(this->aPosition);
    }
    if (this->aTexCoord >= 0) {
        glDisableVertexAttribArray(this->aTexCoord);
    }
    if (this->aNormal >= 0) {
        glDisableVertexAttribArray(this->aNormal);
    }
    if (this->aTangent >= 0) {
        glDisableVertexAttribArray(this->aTangent);
    }
    if (this->aBiNormal >= 0) {
        glDisableVertexAttribArray(this->aBiNormal);
    }
    if (this->aColor >= 0) {
        glDisableVertexAttribArray(this->aColor);
    }
}

} // namespace AbyssEngine
