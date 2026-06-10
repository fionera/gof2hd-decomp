#include "PostBWShader.h"


extern "C" void *PostBWShader_vtable[];
extern "C" uint32_t PostBWShader_ctor_copy_src;
extern "C" uint32_t PostBWShader_ctor_copy_dst;

// ---- SetInActive_8f018.cpp ----
namespace AbyssEngine {

void PostBWShader::SetInActive()
{
    glDisableVertexAttribArray(field_u32(this, 0x20));
    return glDisableVertexAttribArray(field_u32(this, 0x28));
}

} // namespace AbyssEngine

// ---- Init_8efa4.cpp ----
namespace AbyssEngine {

void PostBWShader::Init(Engine *)
{
    uint32_t program = ES2LoadProgram(
        "attribute vec4 a_Position;attribute vec2 a_TexCoord;varying vec2 v_TexCoord;uniform mat4 u_MVPMatrix;void main(){gl_Position=u_MVPMatrix*a_Position;v_TexCoord=a_TexCoord;}",
        "precision mediump float;varying vec2 v_TexCoord;uniform sampler2D s_Texture;void main(){vec4 c=texture2D(s_Texture,v_TexCoord);float g=(c.r+c.g+c.b)/3.0;gl_FragColor=vec4(g,g,g,c.a);}");
    field_u32(this, 0x4) = program;
    field_i32(this, 0x20) = glGetAttribLocation(program, "a_Position");
    field_i32(this, 0x28) = glGetAttribLocation(field_u32(this, 0x4), "a_TexCoord");
    field_i32(this, 0x24) = glGetUniformLocation(field_u32(this, 0x4), "u_MVPMatrix");
    field_i32(this, 0x2c) = glGetUniformLocation(field_u32(this, 0x4), "s_Texture");
    glUseProgram(field_u32(this, 0x4));
    return glUniform1i(field_i32(this, 0x2c), 0);
}

} // namespace AbyssEngine

// ---- _PostBWShader_8ef80.cpp ----
#define POSTBW_NONVIRTUAL_DTOR


namespace AbyssEngine {

PostBWShader::~PostBWShader()
{
    field_ptr(this, 0x0) = (char *)PostBWShader_vtable + 8;
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8f23c.cpp ----
namespace AbyssEngine {

void PostBWShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x24), 1, 0, bytes(engine) + 0x104);
    if (field_u8(this, 0x9) != 0) {
        field_u8(this, 0x9) = 0;
    }

    glEnableVertexAttribArray(field_u32(this, 0x20));
    glEnableVertexAttribArray(field_u32(this, 0x28));

    if (field_u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, field_u32(mesh, 0x60));
        glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, field_u32(mesh, 0x68));
        glVertexAttribPointer(field_u32(this, 0x28), 2, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(field_u32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        glVertexAttribPointer(field_u32(this, 0x28), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
    }
}

} // namespace AbyssEngine

// ---- RenderEffect_8f030.cpp ----
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

// ---- _PostBWShader_8ef94.cpp ----
namespace AbyssEngine {

PostBWShader::~PostBWShader() {}

} // namespace AbyssEngine

// ---- PostBWShader_8eef4.cpp ----
namespace AbyssEngine {

PostBWShader::PostBWShader()
{
    uint32_t copied = PostBWShader_ctor_copy_src;
    PostBWShader_ctor_copy_dst = copied;

    uint32_t storage[4];
    String *name = new (storage) String("PostBWShader", false);
    *reinterpret_cast<String *>(bytes(this) + 0xc) = *name;
    name->~String();
}

} // namespace AbyssEngine
