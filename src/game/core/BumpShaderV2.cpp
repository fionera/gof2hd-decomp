#include "game/core/BumpShaderV2.h"
#include <GLES2/gl2.h>
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"

static const char *const kBumpShaderV2VertexSource =
    "attribute highp vec4 \ta_position;   \t  \n"
    "attribute mediump vec2  a_texCoord;  \n"
    "attribute highp vec3  \ta_normal; \t  \n"
    "attribute lowp \tvec3 \ta_tangent;\t  \n"
    "attribute lowp \tvec3 \ta_bitangent;  \n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying mediump vec3 v_light_dir;  \n"
    "varying mediump vec3 v_eye_dir;  \n"
    "varying mediump vec3 v_lightvec;  \n"
    "varying mediump vec3 v_normal;  \n"
    "varying mediump vec3 v_specular_dir;  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "uniform highp vec3 u_lightdirmodel;  \n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * a_position;  \n"
    "\tv_texCoord  = a_texCoord;    \n"
    "\tv_normal \t= a_normal;  \n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent.x, a_bitangent.x, a_normal.x,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.y, a_bitangent.y, a_normal.y,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.z, a_bitangent.z, a_normal.z  \n"
    "\t\t\t\t\t\t\t\t\t\t);  \n"
    "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightdirmodel) ;  \n"
    "\tv_specular_dir = tangentSpaceXform * shininesDirection;  \n"
    "\tv_lightvec = tangentSpaceXform * u_lightdirmodel;  \n"
    "}  \n";

static const char *const kBumpShaderV2FragmentSource =
    "precision lowp float;  \n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying mediump vec3 v_light_dir;  \n"
    "varying mediump vec3 v_eye_dir;  \n"
    "varying mediump vec3 v_lightvec;  \n"
    "varying mediump vec3 v_normal;  \n"
    "varying mediump vec3 v_specular_dir;  \n"
    "uniform sampler2D  s_texture[2];  \n"
    "uniform lowp vec4  u_AmbientColor;  \n"
    "void main()  \n"
    "{  \n"
    "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord, -1.0 );  \n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, -1.0 );  \n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 - 1.0;  \n"
    "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t//vec4(normalTex.a)  \n"
    "   mediump float powExp = 10.0; \n"
    "\tfloat specularIntensity = pow( dot( v_specular_dir,normalTexX ), powExp );  \n"
    "\tfloat diffuseIntensity  = max( dot( normalTexX    ,v_lightvec ), 0.5 );// + 0.28;  \n"
    "\tgl_FragColor = diffuseIntensity * colorTex * u_AmbientColor + specularIntensity * specTex;  \n"
    "}  \n";

namespace AbyssEngine {
    int BumpShaderV2::ShaderIndex = -1;

    BumpShaderV2::BumpShaderV2() {
        BumpShaderV2::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderV2", false);
    }

    void BumpShaderV2::Init(Engine *) {
        this->program = this->ES2LoadProgram(kBumpShaderV2VertexSource, kBumpShaderV2FragmentSource);

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
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

    void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty) {
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

    void BumpShaderV2::SetInActive() {
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
}
