#include "engine/render/shaders/SandboxShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static AbyssEngine::SandboxShader *SandboxShader_registerSrc = nullptr;
static AbyssEngine::SandboxShader **SandboxShader_registerDst = &SandboxShader_registerSrc;

static const char kSandboxShaderVertexSource[] =
    "attribute highp vec4 \ta_position;   \t\n"
    "attribute mediump vec2  a_texCoord;   \t\n"
    "attribute highp vec3  \ta_normal; \t\t\n"
    "attribute lowp vec3 \ta_tangent;\t\t\n"
    "attribute lowp vec3 \ta_bitangent;\t\n"
    "varying mediump vec2 v_texCoord;\n"
    "varying mediump vec3 v_light_dir;\n"
    "varying mediump vec3 v_eye_dir;\n"
    "varying mediump vec3 v_lightvec;\n"
    "uniform highp mat4 u_WorldMatrix;\n"
    "uniform highp vec3 u_light_dir;\n"
    "uniform highp vec3 u_eye_pos;\n"
    "void main()\n"
    "{\n"
    "\tgl_Position = u_WorldMatrix * a_position;\t\t\n"
    "\tv_texCoord  = a_texCoord;  \n"
    "\t//mediump vec3 bitangent = normalize(cross(a_normal, a_tangent)); \n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent, a_bitangent, a_normal);\n"
    "\t//v_eye_dir   = tangentSpaceXform * normalize(u_eye_pos - a_position.xyz);\n"
    "\t//v_light_dir = tangentSpaceXform * u_light_dir;\n"
    "\tvec3 shininesDirection = normalize(normalize(u_eye_pos - a_position.xyz) + u_light_dir) ;\n"
    "\tv_lightvec = tangentSpaceXform * shininesDirection;\n"
    "\t//u_lightposmodel\n"
    "\t//v_DiffuseLight.rgb = vec3(max(dot(a_normal, u_LightDirection), 0.0));\n"
    "\t//v_DiffuseLight.a = 1.0;\n"
    "}\n";

static const char kSandboxShaderFragmentSource[] =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "varying mediump vec3 v_light_dir;\n"
    "varying mediump vec3 v_eye_dir;\n"
    "varying mediump vec3 v_lightvec;\n"
    "uniform sampler2D  s_texture_norm;\n"
    "uniform sampler2D  s_texture_base;\n"
    "uniform lowp vec4  glColor;\n"
    "uniform lowp vec4  u_AmbientColor;\n"
    "void main()\n"
    "{\n"
    "\tlowp vec3 normal = normalize(texture2D(s_texture_norm, v_texCoord).rgb*2.0 - 1.0);\n"
    "\t//lowp vec3 lightvec = normalize(v_eye_dir+v_light_dir);\n"
    "\tlowp vec3 lightvec = normalize(v_lightvec);\n"
    "\tlowp float specularIntensity = abs( dot(lightvec, normal) );\n"
    "\t//lowp float diffuseIntensity  = 0.0; //max( dot(v_light_dir, normal), 0.0 );\n"
    "\tlowp vec4 textureColor = texture2D( s_texture_base, v_texCoord );// * v_color;\n"
    "\tgl_FragColor =  ( specularIntensity/* + diffuseIntensity*/) * textureColor * glColor;\n"
    "}\t\n";

namespace AbyssEngine {
    int SandboxShader::ShaderIndex = -1;

    void SandboxShader::UpdateMeshData(Mesh *meshArg, Engine *engine) {
        AbyssEngine::Mesh *mesh = meshArg;

        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            glUniform3f(this->uLightDirLoc, engine->lightDir[0].x, engine->lightDir[0].y,
                        engine->lightDir[0].z);
            glUniform3f(this->uEyePosLoc, engine->lightColor.x, engine->lightColor.y,
                        engine->lightColor.z);
            this->dirty = 0;
        }

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);
        glEnableVertexAttribArray(this->aNormalLoc);
        glEnableVertexAttribArray(this->aTangentLoc);
        glEnableVertexAttribArray(this->aBitangentLoc);

        if (mesh->uploaded != 0) {
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
            return;
        }

        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
        glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
        glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
        glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
    }

    void SandboxShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kSandboxShaderVertexSource, kSandboxShaderFragmentSource);

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uLightDirLoc = glGetUniformLocation(this->program, "u_light_dir");
        this->uEyePosLoc = glGetUniformLocation(this->program, "u_eye_pos");
        this->sTextureBaseLoc = glGetUniformLocation(this->program, "s_texture_base");
        this->sTextureNormLoc = glGetUniformLocation(this->program, "s_texture_norm");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");

        glUseProgram(this->program);
        glUniform1i(this->sTextureBaseLoc, 0);
        glUniform1i(this->sTextureNormLoc, 1);
    }

    void SandboxShader::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
        glDisableVertexAttribArray(this->aNormalLoc);
        glDisableVertexAttribArray(this->aTangentLoc);
        glDisableVertexAttribArray(this->aBitangentLoc);
    }

    SandboxShader::SandboxShader() {
        SandboxShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        *SandboxShader_registerDst = SandboxShader_registerSrc;
        this->name = String("SandboxShader", false);
    }
}
