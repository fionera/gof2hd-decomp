#include "engine/render/shaders/SpecCubeMapping.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "platform/gl.h"

namespace AbyssEngine {

// AbyssEngine::SpecCubeMapping::SpecCubeMapping()
//   Installs the SpecCubeMapping vtable, publishes its shader index, and stores
//   the shader resource name.
SpecCubeMapping::SpecCubeMapping()
{
    this->vtable = (void *)(_ZTVN11AbyssEngine15SpecCubeMappingE + 8);
    SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    this->name.s = u"SpecCubeMapping";
}

void SpecCubeMapping::Init(Engine *)
{
    int program = this->LoadBindShader("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
    this->program = program;
    if (program == 0) {
        program = this->ES2LoadProgram("SpecCubeMapping.vsh", "SpecCubeMapping.fsh");
        this->program = program;
    }

    this->attribPosition = glGetAttribLocation(program, "a0");
    this->attribNormal = glGetAttribLocation(this->program, "a1");
    this->attribTexCoord = glGetAttribLocation(this->program, "a2");

    this->mvpMatrixLoc = glGetUniformLocation(this->program, "u0");
    this->normalMatrixLoc = glGetUniformLocation(this->program, "u1");
    this->uCameraPosition = glGetUniformLocation(this->program, "u2");
    this->field_0x38 = glGetUniformLocation(this->program, "u3");
    this->samplerLoc0 = glGetUniformLocation(this->program, "u4");
    this->samplerLoc1 = glGetUniformLocation(this->program, "u5");
    this->field_0x44 = glGetUniformLocation(this->program, "u6");
    this->field_0x48 = glGetUniformLocation(this->program, "u7");
    this->field_0x4c = glGetUniformLocation(this->program, "u8");
    this->field_0x50 = glGetUniformLocation(this->program, "u9");
    this->uShininess = glGetUniformLocation(this->program, "u10");
    this->field_0x58 = glGetUniformLocation(this->program, "u11");

    glUseProgram(this->program);
    glUniform1i(this->samplerLoc0, 0);
    glUniform1i(this->samplerLoc1, 1);
}

// AbyssEngine::SpecCubeMapping::UpdateMeshData(Mesh*, Engine*)
//   Streams the per-frame uniforms (lighting cube data only once, then the
//   matrices/material) and binds the mesh's vertex attribute arrays, handling
//   both client-array and VBO meshes.
void SpecCubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->dirty != 0) {
        glUniform4fv(this->field_0x58, 1, engine->glColor);
        glUniform3fv(this->field_0x48, 1, (const float *)&engine->lightAmbientShaded);
        glUniform3fv(this->field_0x4c, 1, (const float *)&engine->field_0x2fc);
        glUniform3fv(this->field_0x50, 1, (const float *)&engine->lightDiffuseShaded);
        glUniform1f(this->uShininess, engine->materialShininess);
        this->dirty = 0;
    }

    glUniform1f(this->field_0x44, engine->field_0xcc);
    glUniformMatrix4fv(this->mvpMatrixLoc, 1, 0, engine->worldViewProjMatrix);
    glUniformMatrix3fv(this->normalMatrixLoc, 1, 0, engine->normalMatrix);
    glUniform4f(this->field_0x38,
                engine->lightDir.x, engine->lightDir.y,
                engine->lightDir.z, engine->lightDirty[0]);
    glUniform3f(this->uCameraPosition,
                engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);

    glEnableVertexAttribArray(this->attribPosition);
    glEnableVertexAttribArray(this->attribTexCoord);
    glEnableVertexAttribArray(this->attribNormal);

    if (mesh->uploaded == 0) {
        // Client-side vertex arrays.
        glVertexAttribPointer(this->attribPosition, 3, 0x1406, 0, 0, mesh->positions);
        uint8_t flags = mesh->vertexFormat;
        if (flags & 2) {
            glVertexAttribPointer(this->attribTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
            flags = mesh->vertexFormat;
        }
        if (((uint32_t)flags << 0x1d) & 0x80000000u) {
            glVertexAttribPointer(this->attribNormal, 3, 0x1406, 0, 0, mesh->normals);
        }
    } else {
        // Buffer-object meshes: bind each VBO before wiring up the attribute.
        glBindBuffer(0x8892, mesh->positionVBO);
        glVertexAttribPointer(this->attribPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->texCoordVBO);
        glVertexAttribPointer(this->attribTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->normalVBO);
        glVertexAttribPointer(this->attribNormal, 3, 0x1406, 0, 0, 0);
    }
}

void SpecCubeMapping::SetInActive()
{
    glDisableVertexAttribArray(this->attribPosition);
    glDisableVertexAttribArray(this->attribNormal);
    glDisableVertexAttribArray(this->attribTexCoord);
}

} // namespace AbyssEngine
