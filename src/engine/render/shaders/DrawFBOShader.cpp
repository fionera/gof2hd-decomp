#include "engine/render/shaders/DrawFBOShader.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/PaintCanvas.h"
#include <GLES2/gl2.h>
#include <arm_neon.h>


namespace AbyssEngine {
    int DrawFBOShader::ShaderIndex;

    DrawFBOShader::DrawFBOShader() {
        this->name = u"DrawFBOShader";
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    }

    DrawFBOShader::~DrawFBOShader() {
    }

    void DrawFBOShader::Init(Engine *) {
        this->program = this->ES2LoadProgram("DrawFBOShader.vsh", "DrawFBOShader.fsh");

        this->positionLoc = glGetAttribLocation(this->program, "position");
        this->texCoordLoc = glGetAttribLocation(this->program, "texCoord");
        this->worldViewMatrixLoc = glGetUniformLocation(this->program, "worldViewMatrix");
        this->textureLoc = glGetUniformLocation(this->program, "texture");

        glUseProgram(this->program);
        glUniform1i(this->textureLoc, 0);
    }

    void DrawFBOShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->worldViewMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            this->dirty = 0;
        }

        glEnableVertexAttribArray(this->positionLoc);
        glEnableVertexAttribArray(this->texCoordLoc);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, 0);
        }
    }

    void DrawFBOShader::RenderEffect(FBOContainer *fbo, Engine *engine) {
        float *projMatrix = engine->projMatrix;

        uint32x4_t zero = vdupq_n_u32(0);
        vst1q_u32((uint32_t *) &projMatrix[12], zero);
        vst1q_u32((uint32_t *) &projMatrix[8], zero);
        vst1q_u32((uint32_t *) &projMatrix[4], zero);
        vst1q_u32((uint32_t *) &projMatrix[0], zero);

        float two = 2.0f;
        projMatrix[0] = two / (float) (int) engine->GetDisplayWidth();
        int height = (int) engine->GetDisplayHeight();

        projMatrix[10] = -0.05f;
        projMatrix[15] = 1.0f;
        projMatrix[12] = -1.0f;
        projMatrix[13] = 1.0f;
        projMatrix[5] = -(two / (float) height);

        uint32_t one = 0x3f800000;
        uint32_t matrix[15];
        matrix[0] = one;
        matrix[1] = 0;
        matrix[2] = 0;
        matrix[3] = 0;
        matrix[4] = 0;
        matrix[5] = one;
        matrix[6] = 0;
        matrix[7] = 0;
        matrix[8] = 0;
        matrix[9] = 0;
        matrix[10] = one;
        matrix[11] = 0;
        matrix[12] = one;
        matrix[13] = one;
        matrix[14] = one;

        engine->SetWorldViewMatrix(reinterpret_cast<const AEMath::Matrix &>(matrix));
        glDisable(0xb71);
        glDepthMask(0);
        glDisable(0xbe2);
        glUseProgram(this->program);
        glActiveTexture(0x84c0);
        fbo->Activate();

        if (engine->IsPostEffectActivated() == 0) {
            glBindFramebuffer(0x8d40, engine->viewFramebuffer);
            unsigned int width;
            unsigned int viewportHeight;
            if (engine->appManager->paintCanvas->gameOrientation == 2) {
                width = engine->GetDisplayWidth();
                viewportHeight = engine->GetDisplayHeight();
            } else {
                width = engine->GetDisplayHeight();
                viewportHeight = engine->GetDisplayWidth();
            }
            glViewport(0, 0, width, viewportHeight);
        } else {
            engine->ActivateRender2FracFBO();
        }

        glEnableVertexAttribArray(this->positionLoc);
        glEnableVertexAttribArray(this->texCoordLoc);
        glUniformMatrix4fv(this->worldViewMatrixLoc, 1, 0, engine->worldViewProjMatrix);

        Mesh *quadMesh = engine->quadMesh;
        glVertexAttribPointer(this->positionLoc, 3, 0x1406, 0, 0, quadMesh->positions);
        quadMesh = engine->quadMesh;
        glVertexAttribPointer(this->texCoordLoc, 2, 0x1406, 0, 0, quadMesh->texCoords);

        glClear(0x4000);
        glClear(0x100);

        int drawWidth = engine->GetDisplayWidth();
        int drawHeight = engine->GetDisplayHeight();
        engine->DrawQuad(0, 0, drawWidth, drawHeight);

        glDisableVertexAttribArray(this->positionLoc);
        glDisableVertexAttribArray(this->texCoordLoc);
        glEnable(0xb71);
        glClear(0x100);

        if (engine->IsPostEffectActivated() != 0) {
            engine->DeactivateRender2FracFBO();
        }
        glActiveTexture(0x84c0);
    }

    void DrawFBOShader::SetInActive() {
        glDisableVertexAttribArray(this->positionLoc);
        glDisableVertexAttribArray(this->texCoordLoc);
    }
}
