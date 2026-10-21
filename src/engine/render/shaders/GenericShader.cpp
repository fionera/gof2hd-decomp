#include "engine/render/shaders/GenericShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>


static const char *const kGenericShaderVertexSource =
    "attribute highp vec4 \ta_position;  \n"
    "attribute mediump vec2  a_texCoord;    \n"
    "attribute highp vec3  \ta_normal; \t  \n"
    "attribute lowp \tvec3 \ta_tangent;\t\t  \n"
    "attribute lowp \tvec3 \ta_bitangent;\t  \n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying lowp vec3 v_light_dir;  \n"
    "varying lowp vec3 v_eye_dir;  \n"
    "varying lowp vec3 v_lightvec;  \n"
    "varying mediump vec3 v_normal;  \n"
    "varying lowp vec3 v_specular_dir;  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "uniform highp vec3 u_lightdirmodel;  \n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "uniform lowp float u_SpecularPower;\n"
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
    "   v_eye_dir = normalize(tangentSpaceXform*(u_eyeposmodel - a_position.xyz)); \n"
    "}  \n";

static const char *const kGenericShaderFragmentSource =
    "precision lowp float;  \n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying lowp vec3 v_light_dir;  \n"
    "varying lowp vec3 v_eye_dir;  \n"
    "varying lowp vec3 v_lightvec;  \n"
    "varying mediump vec3 v_normal;  \n"
    "varying lowp vec3 v_specular_dir;  \n"
    "uniform sampler2D  s_texture[3];  \n"
    "uniform lowp vec3  u_AmbientColor;  \n"
    "uniform lowp vec3 u_DiffuseColor;\n"
    "uniform lowp vec3 u_SpecularColor;\n"
    "void main()  \n"
    "{  \n"
    "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord, -1.0 );  \n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, -1.0 );  \n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 - 1.0;  \n"
    "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t//vec4(normalTex.a)  \n"
    "   mediump float powExp = 10.0; \n"
    "\tfloat specularIntensity = pow( clamp(dot( v_specular_dir,normalTexX ), 0.0, 1.0)\t, powExp );  \n"
    "\tlowp vec4 specular = texture2D( s_texture[2], vec2(abs(dot( v_specular_dir,normalTexX )),normalTex.a) );  \n"
    "\tlowp float diffuseIntensity  = clamp( dot( normalTexX    ,v_lightvec ), 0.0, 1.0 );// + 0.28;  \n"
    "\tgl_FragColor.xyz = (u_AmbientColor + diffuseIntensity*u_DiffuseColor) * colorTex.xyz  + specularIntensity * specTex.xyz * u_SpecularColor + (smoothstep(0.6, 1.0, 1.0 - dot( v_eye_dir,normalTexX )))* normalTex.a* vec3(2.0, 2.0, 2.0);//specular*u_SpecularColor;  \n"
    "}  \n";

namespace AbyssEngine {
    int GenericShader::ShaderIndex = -1;

    void GenericShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kGenericShaderVertexSource, kGenericShaderFragmentSource);

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
        this->sTexture2Loc = glGetUniformLocation(this->program, "s_texture[2]");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");

        glUseProgram(this->program);

        int *samplers = &this->sTexture0Loc;
        for (int i = 0; i != 3; i++) {
            if (samplers[i] >= 0)
                glUniform1i(samplers[i], i);
        }
    }

    void GenericShader::SetInActive() {
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

    void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uLightdirmodelLoc >= 0)
                glUniform3f(this->uLightdirmodelLoc, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uAmbientColorLoc >= 0)
                glUniform3fv(this->uAmbientColorLoc, 1, (const float *) engine->ambientShaded[0]);
            if (this->uDiffuseColorLoc >= 0)
                glUniform3fv(this->uDiffuseColorLoc, 1, (const float *) engine->diffuseShaded[0]);
            if (this->uSpecularColorLoc >= 0)
                glUniform3fv(this->uSpecularColorLoc, 1, (const float *) engine->specularShaded[0]);
            if (this->uSpecularPowerLoc >= 0)
                glUniform1f(this->uSpecularPowerLoc, engine->materialShininess);
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
    }

    GenericShader::GenericShader() {
        GenericShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GenericShader", false);
    }
}
