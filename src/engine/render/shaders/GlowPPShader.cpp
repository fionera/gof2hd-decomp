#include "engine/render/shaders/GlowPPShader.h"
#include "engine/core/AbyssEngine.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include <GLES2/gl2.h>
#include <cstring>



namespace AbyssEngine {
    int GlowPPShader::ShaderIndex = -1;

    void GlowPPShader::SetInActive() {
        glDisableVertexAttribArray(this->meshAttribPosition);
        glDisableVertexAttribArray(this->meshAttribTexCoord);
    }

    void GlowPPShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->combineUniformWorldView, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            this->dirty = 0;
        }

        glEnableVertexAttribArray(this->meshAttribPosition);
        glEnableVertexAttribArray(this->meshAttribTexCoord);
        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->meshAttribPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->meshAttribTexCoord, 2, 0x1406, 0, 0, 0);
        } else {
            glVertexAttribPointer(this->meshAttribPosition, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->meshAttribTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
        }
    }

    void GlowPPShader::RenderEffect(FBOContainer *source, FBOContainer *&target, Engine *engine) {
        if (AbyssEngine::firstRenderGlow != 0) {
            AbyssEngine::firstRenderGlow = 0;
            this->InternalInit(engine);
            this->backgroundTarget->BeginCapture();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(0x4000);
            this->backgroundTarget->EndCapture();
        }

        memset(engine->projMatrix, 0, sizeof(engine->projMatrix));
        engine->projMatrix[0] = 2.0f / (float) (int) engine->GetDisplayWidth();
        engine->projMatrix[5] = -(2.0f / (float) (int) engine->GetDisplayHeight());
        engine->projMatrix[10] = -0.05f;
        engine->projMatrix[13] = 1.0f;
        engine->projMatrix[15] = 1.0f;
        engine->projMatrix[12] = -1.0f;
        AEMath::Matrix identity;
        engine->SetWorldViewMatrix(identity);
        glDisable(0xb71);
        glDepthMask(0);
        glDisable(0xbe2);

        glUseProgram(this->copyProgram);
        glActiveTexture(0x84c0);
        source->Activate();
        this->copyTarget->BeginCapture();
        glEnableVertexAttribArray(this->copyAttribPosition);
        glEnableVertexAttribArray(this->copyAttribTexCoord);
        glUniformMatrix4fv(this->copyUniformWorldView, 1, 0, engine->worldViewProjMatrix);
        glVertexAttribPointer(this->copyAttribPosition, 3, 0x1406, 0, 0, *(float **) ((char *) engine->quadMesh + 4));
        glVertexAttribPointer(this->copyAttribTexCoord, 2, 0x1406, 0, 0, *(float **) ((char *) engine->quadMesh + 8));
        glClear(0x4000);
        engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
        glDisableVertexAttribArray(this->copyAttribPosition);
        glDisableVertexAttribArray(this->copyAttribTexCoord);

        FBOContainer *blurSource = this->copyTarget;
        for (int i = 0; i < 3; i++) {
            glUseProgram(this->blurXProgram);
            glActiveTexture(0x84c0);
            blurSource->Activate();
            this->blurXTarget->BeginCapture();
            glEnableVertexAttribArray(this->blurXAttribPosition);
            glEnableVertexAttribArray(this->blurXAttribTexCoord);
            glUniformMatrix4fv(this->blurXUniformWorldView, 1, 0, engine->worldViewProjMatrix);
            glVertexAttribPointer(this->blurXAttribPosition, 3, 0x1406, 0, 0,
                                  *(float **) ((char *) engine->quadMesh + 4));
            glVertexAttribPointer(this->blurXAttribTexCoord, 2, 0x1406, 0, 0,
                                  *(float **) ((char *) engine->quadMesh + 8));
            glUniform1f(this->blurXUniformSampleSize, 1.0f / (float) this->blurXTarget->width);
            glClear(0x4000);
            engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
            glDisableVertexAttribArray(this->blurXAttribPosition);
            glDisableVertexAttribArray(this->blurXAttribTexCoord);

            glUseProgram(this->blurYProgram);
            glActiveTexture(0x84c0);
            this->blurXTarget->Activate();
            this->blurYTarget->BeginCapture();
            glEnableVertexAttribArray(this->blurYAttribPosition);
            glEnableVertexAttribArray(this->blurYAttribTexCoord);
            glUniformMatrix4fv(this->blurYUniformWorldView, 1, 0, engine->worldViewProjMatrix);
            glVertexAttribPointer(this->blurYAttribPosition, 3, 0x1406, 0, 0,
                                  *(float **) ((char *) engine->quadMesh + 4));
            glVertexAttribPointer(this->blurYAttribTexCoord, 2, 0x1406, 0, 0,
                                  *(float **) ((char *) engine->quadMesh + 8));
            glUniform1f(this->blurYUniformSampleSize, 1.0f / (float) this->blurYTarget->height);
            glClear(0x4000);
            engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
            glDisableVertexAttribArray(this->blurYAttribPosition);
            glDisableVertexAttribArray(this->blurYAttribTexCoord);
            blurSource = this->blurYTarget;
        }

        FBOContainer *firstTexture = source;
        FBOContainer *secondTexture = blurSource;
        switch (AbyssEngine::Engine::switchGlow) {
            case 0:
                secondTexture = this->backgroundTarget;
                break;
            case 1:
                firstTexture = this->copyTarget;
                secondTexture = this->backgroundTarget;
                break;
            case 2:
                firstTexture = blurSource;
                secondTexture = this->backgroundTarget;
                break;
            case 3:
                secondTexture = this->blurYTarget;
                break;
        }

        glUseProgram(this->combineProgram);
        glActiveTexture(0x84c0);
        firstTexture->Activate();
        glActiveTexture(0x84c1);
        secondTexture->Activate();

        if (target == 0) {
            glBindFramebuffer(0x8d40, engine->viewFramebuffer);
            if (engine->appManager->paintCanvas->gameOrientation == 2) {
                glViewport(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
            } else {
                glViewport(0, 0, engine->GetDisplayHeight(), engine->GetDisplayWidth());
            }
        } else {
            target->BeginCapture();
        }

        glEnableVertexAttribArray(this->combineAttribPosition);
        glEnableVertexAttribArray(this->combineAttribTexCoord);
        glUniformMatrix4fv(this->combineUniformWorldView, 1, 0, engine->worldViewProjMatrix);
        glVertexAttribPointer(this->combineAttribPosition, 3, 0x1406, 0, 0,
                              *(float **) ((char *) engine->quadMesh + 4));
        glVertexAttribPointer(this->combineAttribTexCoord, 2, 0x1406, 0, 0,
                              *(float **) ((char *) engine->quadMesh + 8));
        glClear(0x4000);
        engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
        glDisableVertexAttribArray(this->combineAttribPosition);
        glDisableVertexAttribArray(this->combineAttribTexCoord);
        if (target != 0) {
            target->EndCapture();
        }

        glEnable(0xbe2);
        glBlendFunc(0x302, 0x303);
        glActiveTexture(0x84c0);
    }

    void GlowPPShader::RenderEffect(FBOContainer *source, Engine *engine) {
        FBOContainer *target = 0;
        this->RenderEffect(source, target, engine);
    }

    void GlowPPShader::InternalInit(Engine *engine) {
        this->copyTarget = new FBOContainer(engine, String("GlowPPShader0"));
        this->copyTarget->Create(0x200, 0x200, true, false);

        this->blurXTarget = new FBOContainer(engine, String("GlowPPShader1"));
        this->blurXTarget->Create(0x200, 0x200, true, false);

        this->blurYTarget = new FBOContainer(engine, String("GlowPPShader2"));
        this->blurYTarget->Create(0x200, 0x200, true, false);

        this->backgroundTarget = new FBOContainer(engine, String("GlowPPShader3"));
        this->backgroundTarget->Create(0x200, 0x200, true, false);
    }

    void GlowPPShader::Init(Engine *) {
        const char *vertex = "GlowPPShader.vert";
        this->copyProgram = this->ES2LoadProgram(vertex, "GlowPPShader.copy.frag");
        this->blurXProgram = this->ES2LoadProgram(vertex, "GlowPPShader.blurX.frag");
        this->blurYProgram = this->ES2LoadProgram(vertex, "GlowPPShader.blurY.frag");
        this->combineProgram = this->ES2LoadProgram(vertex, "GlowPPShader.combine.frag");

        const char *position = "position";
        const char *texcoord = "texcoord";
        const char *worldView = "worldView";
        const char *texture = "texture";
        const char *sampleSize = "sampleSize";
        const char *texture2 = "texture2";

        this->copyAttribPosition = glGetAttribLocation(this->copyProgram, position);
        this->copyAttribTexCoord = glGetAttribLocation(this->copyProgram, texcoord);
        this->copyUniformWorldView = glGetUniformLocation(this->copyProgram, worldView);
        this->copyUniformTexture = glGetUniformLocation(this->copyProgram, texture);
        glUseProgram(this->copyProgram);
        glUniform1i(this->copyUniformTexture, 0);

        this->blurXAttribPosition = glGetAttribLocation(this->blurXProgram, position);
        this->blurXAttribTexCoord = glGetAttribLocation(this->blurXProgram, texcoord);
        this->blurXUniformWorldView = glGetUniformLocation(this->blurXProgram, worldView);
        this->blurXUniformTexture = glGetUniformLocation(this->blurXProgram, texture);
        this->blurXUniformSampleSize = glGetUniformLocation(this->blurXProgram, sampleSize);
        glUseProgram(this->blurXProgram);
        glUniform1i(this->blurXUniformTexture, 0);

        this->blurYAttribPosition = glGetAttribLocation(this->blurYProgram, position);
        this->blurYAttribTexCoord = glGetAttribLocation(this->blurYProgram, texcoord);
        this->blurYUniformWorldView = glGetUniformLocation(this->blurYProgram, worldView);
        this->blurYUniformTexture = glGetUniformLocation(this->blurYProgram, texture);
        this->blurYUniformSampleSize = glGetUniformLocation(this->blurYProgram, sampleSize);
        glUseProgram(this->blurYProgram);
        glUniform1i(this->blurYUniformTexture, 0);

        this->combineAttribPosition = glGetAttribLocation(this->combineProgram, position);
        this->combineAttribTexCoord = glGetAttribLocation(this->combineProgram, texcoord);
        this->combineUniformWorldView = glGetUniformLocation(this->combineProgram, worldView);
        this->combineUniformTexture = glGetUniformLocation(this->combineProgram, texture);
        this->combineUniformTexture2 = glGetUniformLocation(this->combineProgram, texture2);
        glUseProgram(this->combineProgram);
        glUniform1i(this->combineUniformTexture, 0);
        glUniform1i(this->combineUniformTexture2, 1);
    }

    GlowPPShader::GlowPPShader() {
        GlowPPShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GlowPPShader");
    }

    GlowPPShader::~GlowPPShader() {
    }
}
