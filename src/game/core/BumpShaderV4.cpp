#include "game/core/BumpShaderV4.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kBumpShaderV4VertexSource =
    "attribute highp vec4 \ta_position;\t\t\n"
    "attribute mediump vec2  a_texCoord;   \t\n"
    "attribute highp vec3  \ta_normal; \t\t\n"
    "attribute lowp vec3 \ta_tangent;\t\t\n"
    "attribute lowp vec3 \ta_bitangent;\t\n"
    "varying mediump vec2 v_texCoord;\t\t\n"
    "varying mediump vec3 v_light_dir;\t\t\n"
    "varying mediump vec3 v_eye_dir;\t\t\n"
    "varying mediump vec3 v_lightvec;\t\t\n"
    "varying mediump vec3 v_normal;\t\t\t\n"
    "varying mediump vec3 v_specular_dir;\t\n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;\t\n"
    "uniform highp vec3 u_lightdirmodel;\t\n"
    "uniform highp vec3 u_eyeposmodel;\t\t\n"
    "//uniform highp mat3 u_ModelMatrix;\t\n"
    "void main()\t\t\t\t\t\t\t\n"
    "{\t\t\t\t\t\t\t\t\t\t\n"
    "\tgl_Position = u_ModelViewProjectionMatrix * a_position;\t\n"
    "\tv_texCoord  = a_texCoord;\t\t\t\n"
    "\tv_normal = a_normal;\t\t\t\t\n"
    "\t//mediump vec3 bitangent = normalize(cross(a_normal, a_tangent)); \n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent.x, a_bitangent.x, a_normal.x,\t\n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.y, a_bitangent.y, a_normal.y,\t\n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.z, a_bitangent.z, a_normal.z\t\n"
    "\t\t\t\t\t\t\t\t\t\t); \n"
    "\t//v_eye_dir   = tangentSpaceXform * normalize(u_eyeposmodel - a_position.xyz); \n"
    "\t//v_light_dir = tangentSpaceXform * u_lightdirmodel; \n"
    "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightdirmodel) ; \n"
    "\tv_specular_dir = tangentSpaceXform * shininesDirection; \n"
    "\tv_lightvec = tangentSpaceXform * u_lightdirmodel; //tangentSpaceXform * shininesDirection; \n"
    "\t//u_lightposmodel \n"
    "\t//v_DiffuseLight.rgb = vec3(max(dot(a_normal, u_LightDirection), 0.0)); \n"
    "\t//v_DiffuseLight.a = 1.0; \n"
    "} \n";

static const char *const kBumpShaderV4FragmentSource =
    "precision lowp float;\t\n"
    "varying mediump vec2 v_texCoord;\t\n"
    "varying mediump vec3 v_light_dir;\t\n"
    "varying mediump vec3 v_eye_dir;\t\n"
    "varying mediump vec3 v_lightvec;\t\n"
    "varying mediump vec3 v_normal;\t\n"
    "varying mediump vec3 v_specular_dir;\t\n"
    "uniform sampler2D  s_texture[2];\t\n"
    "void main()\t\n"
    "{\t\n"
    "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord );\t\n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord );\t\n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 - 1.0;\t\n"
    "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t\n"
    "\tfloat specularIntensity = dot( v_specular_dir,normalTexX );\t\n"
    "\tfloat diffuseIntensity  = max( dot( normalTexX    ,v_lightvec ), 0.22 );\t\n"
    "\tlowp vec4 color;\t\n"
    "\tif (specularIntensity > 0.99)\t\n"
    "\t\tcolor = vec4(0.9, 0.9, 1.0, 1.0);\t\n"
    "\telse if (specularIntensity > 0.95)\t\n"
    "\t\tcolor = vec4(0.7, 0.7, 1.0, 1.0);\t\n"
    "\telse if (specularIntensity > 0.5)\t\n"
    "\t\tcolor = vec4(0.3, 0.3, 0.6, 1.0);\t\n"
    "\telse if (specularIntensity > 0.25)\t\n"
    "\t\tcolor = vec4(0.2, 0.2, 0.4, 1.0);\t\n"
    "\telse \tcolor = vec4(0.1, 0.1, 0.2, 1.0);\t\n"
    "\tgl_FragColor = colorTex*0.5+color; //diffuseIntensity * colorTex + specularIntensity * specTex;\t\n"
    "}\t\n";

namespace AbyssEngine {
    int BumpShaderV4::ShaderIndex;

    BumpShaderV4::BumpShaderV4() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderV4", false);
    }

    void BumpShaderV4::Init(Engine *) {
        int program = this->ES2LoadProgram(kBumpShaderV4VertexSource, kBumpShaderV4FragmentSource);
        this->program = program;

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTexture0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        this->sTexture1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        for (int i = 0; i != 2; i++) {
            int loc = (&this->sTexture0Loc)[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
    }

    void BumpShaderV4::SetInActive() {
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

    void BumpShaderV4::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uLightdirmodelLoc >= 0)
                glUniform3f(this->uLightdirmodelLoc, engine->lightDir.x,
                            engine->lightDir.y, engine->lightDir.z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x,
                            engine->lightColor.y, engine->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uAmbientColorLoc >= 0)
                glUniform4fv(this->uAmbientColorLoc, 1, engine->materialAmbient);
            if (this->uDiffuseColorLoc >= 0)
                glUniform4fv(this->uDiffuseColorLoc, 1, engine->materialDiffuse);
            if (this->uSpecularColorLoc >= 0)
                glUniform4fv(this->uSpecularColorLoc, 1, engine->materialSpecular);
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
