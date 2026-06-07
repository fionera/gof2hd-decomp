#include "class.h"

namespace AbyssEngine {

void PostBWShader::RenderEffect(FBOContainer *fbo, Engine *engine)
{
    AEMath::Matrix matrix;
    int width;
    int height;

    *(AEMath::Matrix *)(bytes(engine) + 0x3b4) = {};
    *(AEMath::Matrix *)(bytes(engine) + 0x3a4) = {};
    *(AEMath::Matrix *)(bytes(engine) + 0x394) = {};
    *(AEMath::Matrix *)(bytes(engine) + 0x384) = {};
    field_u32(engine, 0x3e4) = field_u32(this, 0x4);

    field_f32(engine, 0x384) = 2.0f / (float)engine->GetDisplayWidth();
    field_f32(engine, 0x398) = -(2.0f / (float)engine->GetDisplayHeight());
    field_u32(engine, 0x3ac) = 0xbf800000;
    field_u32(engine, 0x3b4) = 0xbf800000;
    field_f32(engine, 0x3b8) = 1.0f;
    field_f32(engine, 0x3c0) = 1.0f;

    matrix = {};
    matrix.m[0] = 1.0f;
    matrix.m[5] = 1.0f;
    matrix.m[14] = 1.0f;
    engine->SetWorldViewMatrix(matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(field_u32(this, 0x4));
    glActiveTexture(0x84c0);
    fbo->Activate();
    glBindFramebuffer(0x8d40, field_u32(engine, 0x40c));
    glClear(0x4100);

    if (field_i32(*(void **)field_ptr(engine, 0x30), 0x30) == 2) {
        width = engine->GetDisplayWidth();
        height = engine->GetDisplayHeight();
    } else {
        width = engine->GetDisplayHeight();
        height = engine->GetDisplayWidth();
    }
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(field_u32(this, 0x20));
    glEnableVertexAttribArray(field_u32(this, 0x28));
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, bytes(engine) + 0x104);
    glVertexAttribPointer(
        field_u32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(field_ptr(engine, 0x380), 0x4));
    glVertexAttribPointer(
        field_u32(this, 0x28), 2, 0x1406, 0, 0, field_ptr(field_ptr(engine, 0x380), 0x8));

    glClear(0x4000);
    width = engine->GetDisplayWidth();
    height = engine->GetDisplayHeight();
    engine->DrawQuad(0, 0, width, height);

    glDisableVertexAttribArray(field_u32(this, 0x20));
    glDisableVertexAttribArray(field_u32(this, 0x28));
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    field_u32(engine, 0x7c) = 0xffffffff;
}

} // namespace AbyssEngine
