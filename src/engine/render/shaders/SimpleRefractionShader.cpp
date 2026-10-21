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
        this->aNormalLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_tangent");

        this->uM0Loc = glGetUniformLocation(this->program, "a_bitangent");
        this->uM1Loc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uM2Loc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uTex0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        this->uTex1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        this->uM3Loc = glGetUniformLocation(this->program, "glColor");
        this->uM4Loc = glGetUniformLocation(this->program, "u_TexBias");
        this->uM5Loc = glGetUniformLocation(this->program, "u_texelSize");

        glActiveTexture(0x84c7);
        engine->ActivateRefractFBO();

        this->uRefractLoc = glGetUniformLocation(this->program, "u_CloakRate");
        this->uM6Loc = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);

        int samplerLocs[2] = {this->uTex0Loc, this->uTex1Loc};
        for (int i = 0; i != 2; i = i + 1) {
            if (samplerLocs[i] >= 0)
                glUniform1i(samplerLocs[i], i);
        }
        glUniform1i(this->uM6Loc, 7);
    }

    void SimpleRefractionShader::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aNormalLoc >= 0)
            glDisableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glDisableVertexAttribArray(this->aTangentLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->uM0Loc >= 0)
            glDisableVertexAttribArray(this->uM0Loc);
    }

    void SimpleRefractionShader::UpdateMeshData(Mesh *mesh, ::Engine *engine) {
        if (this->uM1Loc >= 0)
            glUniformMatrix4fv(this->uM1Loc, 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            if (this->uM4Loc >= 0)
                glUniform1f(this->uM4Loc, -2.0f);
            if (this->uM3Loc >= 0)
                glUniform4fv(this->uM3Loc, 1, engine->glColor);
            if (this->uM2Loc >= 0)
                glUniform3f(this->uM2Loc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            int loc = this->uM5Loc;
            float w = (float) (int32_t) engine->GetDisplayWidth();
            float h = (float) (int32_t) engine->GetDisplayHeight();
            glUniform2f(loc, 1.0f / w, 1.0f / h);
            glActiveTexture(0x84c7);
            engine->ActivateRefractFBO();
            glUniform1f(this->uRefractLoc, mesh->shaderAnimValue0);
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aNormalLoc >= 0)
            glEnableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glEnableVertexAttribArray(this->aTangentLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->uM0Loc >= 0)
            glEnableVertexAttribArray(this->uM0Loc);

        if (mesh->uploaded == 0) {
            if (this->aPositionLoc >= 0)
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aNormalLoc >= 0)
                glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aTangentLoc >= 0)
                glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, mesh->normals);
            if (this->aTexCoordLoc >= 0)
                glVertexAttribPointer(this->aTexCoordLoc, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->uM0Loc >= 0)
                glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aNormalLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->uM0Loc, 3, 0x1406, 0, 0, 0);
        }
    }
}
