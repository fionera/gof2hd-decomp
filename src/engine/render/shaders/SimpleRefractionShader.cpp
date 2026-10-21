#include "engine/render/shaders/SimpleRefractionShader.h"

#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kSimpleRefractionShaderVertexSource =
    "attribute highp    vec3 a_position;\n"
    "attribute mediump  vec2 a_texCoord;\n"
    "attribute lowp\t\tvec3  \ta_normal; \t  \n"
    "attribute lowp \tvec3 \ta_tangent;\t\t  \n"
    "attribute lowp \tvec3 \ta_bitangent;\t  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;\n"
    "varying lowp vec3 v_eye_dir;  \n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "varying mediump vec2 v_texCoord;\n"
    "varying mediump vec3 v_normal;  \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);\n"
    "\tv_texCoord  = a_texCoord;\n"
    "\tv_normal\t= a_normal;\n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent.x, a_bitangent.x, a_normal.x,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.y, a_bitangent.y, a_normal.y,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.z, a_bitangent.z, a_normal.z  \n"
    "\t\t\t\t\t\t\t\t\t\t);  \n"
    "   v_eye_dir = normalize(tangentSpaceXform*(u_eyeposmodel - a_position)); \n"
    "}\n";

static const char *const kSimpleRefractionShaderFragmentSource =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "varying lowp vec3 v_eye_dir;\n"
    "uniform sampler2D  s_texture[2];\n"
    "uniform sampler2D s_texture_base;\n"
    "uniform lowp vec4  glColor; \n"
    "uniform highp float u_TexBias; \n"
    "uniform highp vec2 u_texelSize;\n"
    "uniform mediump float u_CloakRate;\n"
    "void main()\n"
    "{\n"
    "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord, u_TexBias);\n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, u_TexBias);\n"
    "\tlowp vec3 normalTexX = colorTex.rgb * 2.0 - 1.0;\n"
    "\tfloat rim_intensity = (smoothstep(0.0, 1.0, 1.0 - dot( v_eye_dir,normalTexX )));\n"
    "\tfloat time = (u_CloakRate);\n"
    "   vec2 coord;\n"
    "   coord = (gl_FragCoord.xy + ((glColor.a*(colorTex.xy-0.5)*rim_intensity)*180.0)) * u_texelSize;\n"
    "\tgl_FragColor.rgb = texture2D(s_texture_base, coord).rgb;\n"
    "\tgl_FragColor.a = colorTex.a;\n"
    "}\n";

namespace AbyssEngine {
    int SimpleRefractionShader::ShaderIndex;

    SimpleRefractionShader::SimpleRefractionShader() {
        SimpleRefractionShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("SimpleRefractionShader", false);
    }

    void SimpleRefractionShader::Init(::Engine *engine) {
        int program = this->ES2LoadProgram(
            kSimpleRefractionShaderVertexSource, kSimpleRefractionShaderFragmentSource);
        this->program = program;

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTexture0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        this->sTexture1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uTexBiasLoc = glGetUniformLocation(this->program, "u_TexBias");
        this->uTexelSizeLoc = glGetUniformLocation(this->program, "u_texelSize");

        glActiveTexture(0x84c7);
        engine->ActivateRefractFBO();

        this->uCloakRateLoc = glGetUniformLocation(this->program, "u_CloakRate");
        this->sTextureBaseLoc = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);

        for (int i = 0; i != 2; i = i + 1) {
            if ((&this->sTexture0Loc)[i] >= 0)
                glUniform1i((&this->sTexture0Loc)[i], i);
        }
        glUniform1i(this->sTextureBaseLoc, 7);
    }

    void SimpleRefractionShader::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glDisableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glDisableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glDisableVertexAttribArray(this->aBitangentLoc);
    }

    void SimpleRefractionShader::UpdateMeshData(Mesh *mesh, ::Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            if (this->uTexBiasLoc >= 0)
                glUniform1f(this->uTexBiasLoc, -2.0f);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            int loc = this->uTexelSizeLoc;
            float w = (float) (int32_t) engine->GetDisplayWidth();
            float h = (float) (int32_t) engine->GetDisplayHeight();
            glUniform2f(loc, 1.0f / w, 1.0f / h);
            glActiveTexture(0x84c7);
            engine->ActivateRefractFBO();
            glUniform1f(this->uCloakRateLoc, mesh->shaderAnimValue0);
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glEnableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glEnableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glEnableVertexAttribArray(this->aBitangentLoc);

        if (mesh->uploaded == 0) {
            if (this->aPositionLoc >= 0)
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aTexCoordLoc >= 0)
                glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aNormalLoc >= 0)
                glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
            if (this->aTangentLoc >= 0)
                glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->aBitangentLoc >= 0)
                glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, 0);
        }
    }
}
