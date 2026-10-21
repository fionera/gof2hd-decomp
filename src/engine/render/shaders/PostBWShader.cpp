#include "engine/render/shaders/PostBWShader.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/PaintCanvas.h"
#include <GLES2/gl2.h>
#include <arm_neon.h>


static const char *const kPostBWShaderVertexSource =
    "attribute highp vec4 a_position;\n"
    "attribute mediump vec2 a_texCoord;\n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform highp mat4 u_WorldMatrix;\n"
    "void main()\n"
    "{\n"
    "\tgl_Position = u_WorldMatrix * a_position;\n"
    "\tv_texCoord = a_texCoord;\n"
    "}\n";

static const char *const kPostBWShaderFragmentSource =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform sampler2D s_texture_base;\n"
    "void main()\n"
    "{\n"
    "\tvec4 textureColor = texture2D( s_texture_base, v_texCoord );\n"
    "   float value = (textureColor.r + textureColor.g + textureColor.b) / 3.0; \n"
    "   gl_FragColor.r = 0.0; \n"
    "   gl_FragColor.g = 0.0; \n"
    "   gl_FragColor.b = textureColor.b; \n"
    "\tgl_FragColor.a = textureColor.a;\n"
    "}\n";

namespace AbyssEngine {
    int PostBWShader::ShaderIndex;

    PostBWShader::PostBWShader() {
        PostBWShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("PostBWShader", false);
    }

    PostBWShader::~PostBWShader() {
    }

    void PostBWShader::Init(Engine *) {
        this->program = ES2LoadProgram(
            kPostBWShaderVertexSource,
            kPostBWShaderFragmentSource);
        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->uMvpMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->sTexture = glGetUniformLocation(this->program, "s_texture_base");
        glUseProgram(this->program);
        glUniform1i(this->sTexture, 0);
    }

    void PostBWShader::SetInActive() {
        glDisableVertexAttribArray(this->aPosition);
        glDisableVertexAttribArray(this->aTexCoord);
    }

    void PostBWShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
        this->dirty = 0;

        glEnableVertexAttribArray(this->aPosition);
        glEnableVertexAttribArray(this->aTexCoord);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        } else {
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
        }
    }

    void PostBWShader::RenderEffect(FBOContainer *fbo, Engine *engine) {
        uint32x4_t zero = vdupq_n_u32(0);
        vst1q_u32((uint32_t *) &engine->projMatrix[12], zero);
        vst1q_u32((uint32_t *) &engine->projMatrix[8], zero);
        vst1q_u32((uint32_t *) &engine->projMatrix[4], zero);
        vst1q_u32((uint32_t *) &engine->projMatrix[0], zero);
        engine->currentProgram = this->program;

        engine->projMatrix[0] = 2.0f / (float) engine->GetDisplayWidth();
        engine->projMatrix[5] = -(2.0f / (float) engine->GetDisplayHeight());
        engine->projMatrix[10] = -1.0f;
        engine->projMatrix[12] = -1.0f;
        engine->projMatrix[13] = 1.0f;
        engine->projMatrix[15] = 1.0f;

        float matrix[16] = {};
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[14] = 1.0f;
        engine->SetWorldViewMatrix(reinterpret_cast<const AEMath::Matrix &>(matrix));

        glDisable(0xb71);
        glDepthMask(0);
        glDisable(0xbe2);
        glUseProgram(this->program);
        glActiveTexture(0x84c0);
        fbo->Activate();
        glBindFramebuffer(0x8d40, engine->viewFramebuffer);
        glClear(0x4100);

        int width;
        int height;
        if (engine->appManager->paintCanvas->gameOrientation == 2) {
            width = engine->GetDisplayWidth();
            height = engine->GetDisplayHeight();
        } else {
            width = engine->GetDisplayHeight();
            height = engine->GetDisplayWidth();
        }
        glViewport(0, 0, width, height);

        glEnableVertexAttribArray(this->aPosition);
        glEnableVertexAttribArray(this->aTexCoord);
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0,
                              engine->quadMesh->positions);
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0,
                              engine->quadMesh->texCoords);

        glClear(0x4000);
        engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());

        glDisableVertexAttribArray(this->aPosition);
        glDisableVertexAttribArray(this->aTexCoord);
        glEnable(0xbe2);
        glBlendFunc(0x302, 0x303);
        glActiveTexture(0x84c0);
        engine->boundTextures[0] = 0xffffffff;
    }
}
