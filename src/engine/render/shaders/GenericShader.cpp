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
    int GenericShader::ShaderIndex;

    void GenericShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kGenericShaderVertexSource, kGenericShaderFragmentSource);

        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormal = glGetAttribLocation(this->program, "a_normal");
        this->aTangent = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangent = glGetAttribLocation(this->program, "a_bitangent");

        this->uMvpMatrix = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uNormalMatrix = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightPosition = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyePosition = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uTexture0 = glGetUniformLocation(this->program, "s_texture[0]");
        this->uTexture1 = glGetUniformLocation(this->program, "s_texture[1]");
        this->uTexture2 = glGetUniformLocation(this->program, "s_texture[2]");
        this->uAmbientColor = glGetUniformLocation(this->program, "glColor");
        this->uDiffuseColor = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uSpecularColor = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uEmissiveColor = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uMaterialShininess = glGetUniformLocation(this->program, "u_SpecularPower");

        glUseProgram(this->program);

        int *samplers = &this->uTexture0;
        for (int i = 0; i != 3; i++) {
            if (samplers[i] >= 0)
                glUniform1i(samplers[i], i);
        }
    }

    void GenericShader::SetInActive() {
        if (this->aPosition >= 0)
            glDisableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glDisableVertexAttribArray(this->aTexCoord);
        if (this->aNormal >= 0)
            glDisableVertexAttribArray(this->aNormal);
        if (this->aTangent >= 0)
            glDisableVertexAttribArray(this->aTangent);
        if (this->aBitangent >= 0)
            glDisableVertexAttribArray(this->aBitangent);
    }

    void GenericShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uMvpMatrix >= 0)
            glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
        if (this->uNormalMatrix >= 0)
            glUniformMatrix3fv(this->uNormalMatrix, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uLightPosition >= 0)
                glUniform3f(this->uLightPosition, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);
            if (this->uEyePosition >= 0)
                glUniform3f(this->uEyePosition, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            if (this->uAmbientColor >= 0)
                glUniform4fv(this->uAmbientColor, 1, engine->glColor);
            if (this->uDiffuseColor >= 0)
                glUniform3fv(this->uDiffuseColor, 1, (const float *) &engine->lightAmbientShaded);
            if (this->uSpecularColor >= 0)
                glUniform3fv(this->uSpecularColor, 1, (const float *) &engine->field_0x2fc);
            if (this->uEmissiveColor >= 0)
                glUniform3fv(this->uEmissiveColor, 1, (const float *) &engine->lightDiffuseShaded);
            if (this->uMaterialShininess >= 0)
                glUniform1f(this->uMaterialShininess, engine->materialShininess);
            this->dirty = 0;
        }

        if (this->aPosition >= 0)
            glEnableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glEnableVertexAttribArray(this->aTexCoord);
        if (this->aNormal >= 0)
            glEnableVertexAttribArray(this->aNormal);
        if (this->aTangent >= 0)
            glEnableVertexAttribArray(this->aTangent);
        if (this->aBitangent >= 0)
            glEnableVertexAttribArray(this->aBitangent);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, 0);
            return;
        }

        if (this->aPosition >= 0)
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
        if (this->aTexCoord >= 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
        if (this->aNormal >= 0)
            glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, mesh->normals);
        if (this->aTangent >= 0)
            glVertexAttribPointer(this->aTangent, 3, 0x1406, 0, 0, mesh->tangents);
        if (this->aBitangent >= 0)
            glVertexAttribPointer(this->aBitangent, 3, 0x1406, 0, 0, mesh->binormals);
    }

    GenericShader::GenericShader() {
        GenericShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GenericShader", false);
    }
}
