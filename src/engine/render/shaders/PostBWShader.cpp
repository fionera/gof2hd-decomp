#include "gof2/PostBWShader.h"

namespace AbyssEngine {

// AbyssEngine::PostBWShader::SetInActive()
void PostBWShader::SetInActive()
{
    glDisableVertexAttribArray(field_0x20);
    return glDisableVertexAttribArray(field_0x28);
}

// AbyssEngine::PostBWShader::Init(AbyssEngine::Engine*)
void PostBWShader::Init(Engine *)
{
    uint32_t program = ES2LoadProgram(
        "attribute vec4 a_Position;attribute vec2 a_TexCoord;varying vec2 v_TexCoord;uniform mat4 u_MVPMatrix;void main(){gl_Position=u_MVPMatrix*a_Position;v_TexCoord=a_TexCoord;}",
        "precision mediump float;varying vec2 v_TexCoord;uniform sampler2D s_Texture;void main(){vec4 c=texture2D(s_Texture,v_TexCoord);float g=(c.r+c.g+c.b)/3.0;gl_FragColor=vec4(g,g,g,c.a);}");
    field_0x4 = (int)program;
    field_0x20 = glGetAttribLocation(field_0x4, "a_Position");
    field_0x28 = glGetAttribLocation(field_0x4, "a_TexCoord");
    field_0x24 = glGetUniformLocation(field_0x4, "u_MVPMatrix");
    field_0x2c = glGetUniformLocation(field_0x4, "s_Texture");
    glUseProgram(field_0x4);
    return glUniform1i(field_0x2c, 0);
}

// AbyssEngine::PostBWShader::~PostBWShader()
PostBWShader::~PostBWShader()
{
    field_0x0 = (char *)PostBWShader_vtable + 8;
}

// AbyssEngine::PostBWShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void PostBWShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;
    glUniformMatrix4fv(field_0x24, 1, 0, e + 0x104);
    if (field_0x9 != 0) {
        field_0x9 = 0;
    }

    glEnableVertexAttribArray(field_0x20);
    glEnableVertexAttribArray(field_0x28);

    if (*(uint8_t *)(m + 0x5c) != 0) {
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
        glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x68));
        glVertexAttribPointer(field_0x28, 2, 0x1406, 0, 0, 0);
    } else {
        glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        glVertexAttribPointer(field_0x28, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
    }
}

// AbyssEngine::PostBWShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*)
void PostBWShader::RenderEffect(FBOContainer *fbo, Engine *engine)
{
    char *e = (char *)engine;
    AEMath::Matrix matrix;
    int width;
    int height;

    *(AEMath::Matrix *)(e + 0x3b4) = {};
    *(AEMath::Matrix *)(e + 0x3a4) = {};
    *(AEMath::Matrix *)(e + 0x394) = {};
    *(AEMath::Matrix *)(e + 0x384) = {};
    *(uint32_t *)(e + 0x3e4) = (uint32_t)field_0x4;

    *(float *)(e + 0x384) = 2.0f / (float)engine->GetDisplayWidth();
    *(float *)(e + 0x398) = -(2.0f / (float)engine->GetDisplayHeight());
    *(uint32_t *)(e + 0x3ac) = 0xbf800000;
    *(uint32_t *)(e + 0x3b4) = 0xbf800000;
    *(float *)(e + 0x3b8) = 1.0f;
    *(float *)(e + 0x3c0) = 1.0f;

    matrix = {};
    matrix.m[0] = 1.0f;
    matrix.m[5] = 1.0f;
    matrix.m[14] = 1.0f;
    engine->SetWorldViewMatrix(matrix);

    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    glUseProgram(field_0x4);
    glActiveTexture(0x84c0);
    fbo->Activate();
    glBindFramebuffer(0x8d40, *(uint32_t *)(e + 0x40c));
    glClear(0x4100);

    if (*(int *)((char *)*(void **)(*(void **)(e + 0x30)) + 0x30) == 2) {
        width = engine->GetDisplayWidth();
        height = engine->GetDisplayHeight();
    } else {
        width = engine->GetDisplayHeight();
        height = engine->GetDisplayWidth();
    }
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(field_0x20);
    glEnableVertexAttribArray(field_0x28);
    glUniformMatrix4fv(field_0x24, 1, 0, e + 0x104);
    glVertexAttribPointer(
        field_0x20, 3, 0x1406, 0, 0, *(void **)((char *)*(void **)(e + 0x380) + 0x4));
    glVertexAttribPointer(
        field_0x28, 2, 0x1406, 0, 0, *(void **)((char *)*(void **)(e + 0x380) + 0x8));

    glClear(0x4000);
    width = engine->GetDisplayWidth();
    height = engine->GetDisplayHeight();
    engine->DrawQuad(0, 0, width, height);

    glDisableVertexAttribArray(field_0x20);
    glDisableVertexAttribArray(field_0x28);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    glActiveTexture(0x84c0);
    *(uint32_t *)(e + 0x7c) = 0xffffffff;
}

// AbyssEngine::PostBWShader::PostBWShader()
PostBWShader::PostBWShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();

    uint32_t copied = PostBWShader_ctor_copy_src;
    PostBWShader_ctor_copy_dst = copied;

    field_0x0 = (char *)PostBWShader_vtable + 8;

    // field_0xc = String("PostBWShader", false)
    field_0xc.s = u"PostBWShader";
}

} // namespace AbyssEngine
