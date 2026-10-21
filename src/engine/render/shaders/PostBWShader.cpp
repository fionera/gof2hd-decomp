#include "engine/render/shaders/PostBWShader.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/PaintCanvas.h"
#include <GLES2/gl2.h>
#include <string.h>


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
        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->sTextureBaseLoc = glGetUniformLocation(this->program, "s_texture_base");
        glUseProgram(this->program);
        glUniform1i(this->sTextureBaseLoc, 0);
    }

    void PostBWShader::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
    }

    void PostBWShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0)
            this->dirty = 0;

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
        } else {
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        }
    }

    void PostBWShader::RenderEffect(FBOContainer *fbo, Engine *engine) {
        engine->currentProgram = this->program;
        memset(engine->projMatrix, 0, sizeof(engine->projMatrix));

        engine->projMatrix[0] = 2.0f / (float) (int) engine->GetDisplayWidth();
        engine->projMatrix[5] = -(2.0f / (float) (int) engine->GetDisplayHeight());
        engine->projMatrix[10] = -0.05f;
        engine->projMatrix[15] = 1.0f;
        engine->projMatrix[12] = -1.0f;
        engine->projMatrix[13] = 1.0f;

        float matrix[15];
        matrix[0] = 1.0f;
        matrix[1] = 0.0f;
        matrix[2] = 0.0f;
        matrix[3] = 0.0f;
        matrix[4] = 0.0f;
        matrix[5] = 1.0f;
        matrix[6] = 0.0f;
        matrix[7] = 0.0f;
        matrix[8] = 0.0f;
        matrix[9] = 0.0f;
        matrix[10] = 1.0f;
        matrix[11] = 0.0f;
        matrix[12] = 1.0f;
        matrix[13] = 1.0f;
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

        if (engine->appManager->paintCanvas->gameOrientation == 2)
            glViewport(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
        else
            glViewport(0, 0, engine->GetDisplayHeight(), engine->GetDisplayWidth());

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0,
                              engine->quadMesh->positions);
        glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0,
                              engine->quadMesh->texCoords);

        glClear(0x4000);
        engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());

        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
        glEnable(0xbe2);
        glBlendFunc(0x302, 0x303);
        glActiveTexture(0x84c0);
        engine->boundTextures[0] = 0xffffffff;
    }
}
