#include "engine/render/shaders/BlurShader.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include <GLES2/gl2.h>
#include <cstring>


namespace AbyssEngine {
    int BlurShader::ShaderIndex;

    BlurShader::BlurShader() {
        BlurShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BlurShader");
        this->strength = 0.1667f;
        this->blurScale = 2.0f;
    }

    BlurShader::~BlurShader() {
    }

    void BlurShader::Init(Engine *) {
        this->blurProgram = this->ES2LoadProgram(
            "attribute highp vec4 a_position;\n"
            "uniform highp mat4 u_WorldMatrix;\n"
            "varying mediump vec2 uv;\n"
            "void main(void)\n"
            "{\n"
            "\tgl_Position = vec4((u_WorldMatrix * a_position).xy, 0.0, 1.0 );\n"
            "\tgl_Position = sign( gl_Position );\n"
            "\t// Texture coordinate for screen aligned (in correct range):\n"
            "\tuv = (gl_Position.xy + 1.0) * 0.5;\n"
            "}\n",
            "precision lowp float;\n"
            "uniform sampler2D s_texture_base;\n"
            "uniform lowp vec2 u_radial_origin;   // blur origin\n"
            "uniform lowp float u_blur_strength;  // blur strength\n"
            "varying mediump vec2 uv;\n"
            "const lowp float sampleDist = 1.0;\n"
            "void main(void)\n"
            "{\n"
            "    mediump vec2 dir = u_radial_origin - uv;\n"
            "    float dist = length(dir);\n"
            "    dir = dir/dist;\n"
            "    mediump vec4 color = texture2D(s_texture_base, uv);\n"
            "    mediump vec4 sum = color;\n"
            "    float samples[6];\n"
            "    samples[0] = -0.03;\n"
            "    samples[1] = -0.02;\n"
            "    samples[2] = -0.01;\n"
            "    samples[3] =  0.01;\n"
            "    samples[4] =  0.02;\n"
            "    samples[5] =  0.03;\n"
            "    for (int i = 0; i < 5; i++)\n"
            "\t\tsum += texture2D( s_texture_base, uv + dir * samples[i]);\n"
            "\tsum *= 0.1429;\n"
            "\tgl_FragColor = mix( color, sum, clamp( dist * u_blur_strength ,0.0,1.0) );\n"
            "}\n");
        this->aPosition = glGetAttribLocation(this->blurProgram, "a_position");
        this->aTexCoord = glGetAttribLocation(this->blurProgram, "a_texCoord");
        this->uMvpMatrix = glGetUniformLocation(this->blurProgram, "u_WorldMatrix");
        this->sTexture = glGetUniformLocation(this->blurProgram, "s_texture_base");
        this->uTexelSize = glGetUniformLocation(this->blurProgram, "u_texel_size");
        this->uCenter = glGetUniformLocation(this->blurProgram, "u_radial_origin");
        this->uBlurAmount = glGetUniformLocation(this->blurProgram, "u_blur_strength");
        this->uStrength = glGetUniformLocation(this->blurProgram, "u_radial_bright");
        glUseProgram(this->blurProgram);
        glUniform1i(this->sTexture, 0);
    }

    void BlurShader::SetInActive() {
        glDisableVertexAttribArray(this->positionAttrib);
        glDisableVertexAttribArray(this->texCoordAttrib);
    }

    void BlurShader::RenderEffect(FBOContainer *fbo, FBOContainer *&target, Engine *engine,
                                  float amount, Vector vector) {
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
        glUseProgram(this->blurProgram);
        glActiveTexture(0x84c0);
        fbo->Activate();

        if (target != nullptr) {
            target->BeginCapture();
        } else {
            glBindFramebuffer(0x8d40, engine->viewFramebuffer);
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
        }

        if (this->aPosition >= 0)
            glEnableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glEnableVertexAttribArray(this->aTexCoord);
        if (this->uMvpMatrix >= 0)
            glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);

        int texelLocation = this->uTexelSize;
        if (texelLocation >= 0) {
            if (engine->appManager->paintCanvas->gameOrientation == 2) {
                glUniform2f(texelLocation, 1.0f / (float) (int) engine->GetDisplayWidth(),
                            1.0f / (float) (int) engine->GetDisplayHeight());
            } else {
                glUniform2f(texelLocation, 1.0f / (float) (int) engine->GetDisplayHeight(),
                            1.0f / (float) (int) engine->GetDisplayWidth());
            }
        }
        int centerLocation = this->uCenter;
        if (centerLocation >= 0)
            glUniform2f(centerLocation, vector[0], vector[1]);
        if (this->uStrength >= 0)
            glUniform1f(this->uStrength, this->strength);
        if (this->uBlurAmount >= 0)
            glUniform1f(this->uBlurAmount, (amount < 0.0f ? 0.0f : amount) * this->blurScale);

        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, engine->quadMesh->positions);
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, engine->quadMesh->texCoords);

        glClear(0x4000);
        engine->DrawQuad(0, 0, engine->GetDisplayWidth(), engine->GetDisplayHeight());
        if (this->aPosition >= 0)
            glDisableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glDisableVertexAttribArray(this->aTexCoord);
        if (target != nullptr)
            target->EndCapture();
        glEnable(0xbe2);
        glBlendFunc(0x302, 0x303);
        glActiveTexture(0x84c0);
    }

    void BlurShader::RenderEffect(FBOContainer *fbo, Engine *engine, float amount, Vector vector) {
        FBOContainer *target = nullptr;
        this->RenderEffect(fbo, target, engine, amount, vector);
    }

    void BlurShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        int matrixLocation = this->uMvpMatrix;
        if (matrixLocation >= 0) {
            glUniformMatrix4fv(matrixLocation, 1, 0, engine->worldViewProjMatrix);
        }

        int texelLocation = this->uTexelSize;
        if (texelLocation >= 0) {
            if (engine->appManager->paintCanvas->gameOrientation == 2) {
                glUniform2f(texelLocation, 1.0f / (float) (int) engine->GetDisplayWidth(),
                            1.0f / (float) (int) engine->GetDisplayHeight());
            } else {
                glUniform2f(texelLocation, 1.0f / (float) (int) engine->GetDisplayHeight(),
                            1.0f / (float) (int) engine->GetDisplayWidth());
            }
        }

        int centerLocation = this->uCenter;
        if (centerLocation >= 0) {
            glUniform2f(centerLocation, 0.5f, 0.3f);
        }

        int strengthLocation = this->uStrength;
        if (strengthLocation >= 0) {
            glUniform1f(strengthLocation, this->strength);
        }

        int amountLocation = this->uBlurAmount;
        if (amountLocation >= 0) {
            glUniform1f(amountLocation, this->blurScale);
        }

        if (this->dirty != 0) {
            this->dirty = 0;
        }

        glEnableVertexAttribArray(this->positionAttrib);
        glEnableVertexAttribArray(this->texCoordAttrib);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->positionAttrib, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->texCoordAttrib, 2, 0x1406, 0, 0, mesh->texCoords);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->positionAttrib, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->texCoordAttrib, 2, 0x1406, 0, 0, 0);
        }
    }
}
