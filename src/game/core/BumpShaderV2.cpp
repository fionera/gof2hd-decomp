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
    int BumpShaderV2::ShaderIndex;

    BumpShaderV2::BumpShaderV2() {
        BumpShaderV2::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderV2", false);
    }

    void BumpShaderV2::Init(Engine *) {
        this->program = this->ES2LoadProgram(kBumpShaderV2VertexSource, kBumpShaderV2FragmentSource);

        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aNormal = glGetAttribLocation(this->program, "a_texCoord");
        this->aTexCoord = glGetAttribLocation(this->program, "a_normal");
        this->aTangent = glGetAttribLocation(this->program, "a_tangent");
        this->aBinormal = glGetAttribLocation(this->program, "a_bitangent");

        this->uMvpMatrix = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uNormalMatrix = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightDir = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyePos = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uTexture0 = glGetUniformLocation(this->program, "s_texture[0]");
        this->uTexture1 = glGetUniformLocation(this->program, "s_texture[1]");
        this->uAmbient = glGetUniformLocation(this->program, "glColor");
        this->uDiffuse = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uSpecular = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uEmissive = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        for (int i = 0; i != 2; i++) {
            int loc = (&this->uTexture0)[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
    }

    void BumpShaderV2::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uMvpMatrix >= 0)
            glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
        if (this->uNormalMatrix >= 0)
            glUniformMatrix3fv(this->uNormalMatrix, 1, 0, engine->normalMatrix);

        if (this->dirty) {
            if (this->uLightDir >= 0)
                glUniform3f(this->uLightDir, engine->lightDir.x,
                            engine->lightDir.y, engine->lightDir.z);
            if (this->uEyePos >= 0)
                glUniform3f(this->uEyePos, engine->lightColor.x,
                            engine->lightColor.y, engine->lightColor.z);
            if (this->uAmbient >= 0)
                glUniform4fv(this->uAmbient, 1, engine->glColor);
            if (this->uDiffuse >= 0)
                glUniform4fv(this->uDiffuse, 1, engine->materialAmbient);
            if (this->uSpecular >= 0)
                glUniform4fv(this->uSpecular, 1, engine->materialDiffuse);
            if (this->uEmissive >= 0)
                glUniform4fv(this->uEmissive, 1, engine->materialSpecular);
            this->dirty = 0;
        }

        if (this->aPosition >= 0)
            glEnableVertexAttribArray(this->aPosition);
        if (this->aNormal >= 0)
            glEnableVertexAttribArray(this->aNormal);
        if (this->aTexCoord >= 0)
            glEnableVertexAttribArray(this->aTexCoord);
        if (this->aTangent >= 0)
            glEnableVertexAttribArray(this->aTangent);
        if (this->aBinormal >= 0)
            glEnableVertexAttribArray(this->aBinormal);

        if (mesh->uploaded == 0) {
            if (this->aPosition >= 0)
                glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aNormal >= 0)
                glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aTexCoord >= 0)
                glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, mesh->normals);
            if (this->aTangent >= 0)
                glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->aBinormal >= 0)
                glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aNormal, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aTexCoord, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBinormal, 3, 0x1406, 0, 0, 0);
        }
    }

    void BumpShaderV2::SetInActive() {
        if (this->aPosition >= 0)
            glDisableVertexAttribArray(this->aPosition);
        if (this->aNormal >= 0)
            glDisableVertexAttribArray(this->aNormal);
        if (this->aTexCoord >= 0)
            glDisableVertexAttribArray(this->aTexCoord);
        if (this->aTangent >= 0)
            glDisableVertexAttribArray(this->aTangent);
        if (this->aBinormal >= 0)
            glDisableVertexAttribArray(this->aBinormal);
    }
}
