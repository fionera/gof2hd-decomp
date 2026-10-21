#include "game/core/BumpShaderV3.h"
#include "engine/render/Engine.h"
#include "engine/render/Material.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

namespace AbyssEngine {
    int BumpShaderV3::ShaderIndex = -1;

    BumpShaderV3::BumpShaderV3() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderV3");
    }

    void BumpShaderV3::Init(Engine *) {
        this->program = this->ES2LoadProgram(
            "attribute highp vec4 \ta_position;  \n"
            "attribute mediump vec2  a_texCoord;    \n"
            "attribute highp vec3  \ta_normal; \t  \n"
            "attribute lowp \tvec3 \ta_tangent;\t\t  \n"
            "attribute lowp \tvec3 \ta_bitangent;\t  \n"
            "varying mediump vec2 v_texCoord;  \n"
            "varying lowp vec3 v_eye_dir;  \n"
            "varying mediump vec3 v_normal;  \n"
            "varying lowp vec3 v_specular_dir[2];  \n"
            "varying lowp vec3 v_lightvec[2];  \n"
            "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
            "uniform highp vec3 u_lightdirmodel[2];  \n"
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
            "   v_eye_dir = normalize(tangentSpaceXform*(u_eyeposmodel - a_position.xyz)); \n"
            "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightdirmodel[0]) ;  \n"
            "\tv_specular_dir[0] = tangentSpaceXform * shininesDirection;  \n"
            "\tv_lightvec[0] = tangentSpaceXform * u_lightdirmodel[0];  \n"
            "\tshininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightdirmodel[1]) ;  \n"
            "\tv_specular_dir[1] = tangentSpaceXform * shininesDirection;  \n"
            "\tv_lightvec[1] = tangentSpaceXform * u_lightdirmodel[1];  \n"
            "}  \n",
            "precision lowp float;  \n"
            "varying mediump vec2 v_texCoord;  \n"
            "varying lowp vec3 v_eye_dir;  \n"
            "varying mediump vec3 v_normal;  \n"
            "varying lowp vec3 v_specular_dir[2];  \n"
            "varying lowp vec3 v_lightvec[2];  \n"
            "uniform sampler2D  s_texture[3];  \n"
            "uniform lowp vec3  u_AmbientColor[2];  \n"
            "uniform lowp vec3 u_DiffuseColor[2];\n"
            "uniform lowp vec3 u_SpecularColor[2];\n"
            "uniform mediump float u_SpecularPower;\n"
            "uniform lowp vec3 u_RimColor;\n"
            "uniform highp float u_TexBiasDiffuse; \n"
            "uniform highp float u_TexBiasNormal; \n"
            "uniform lowp float u_IsGlowMat;\n"
            "void main()  \n"
            "{  \n"
            "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord, u_TexBiasDiffuse );  \n"
            "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, u_TexBiasNormal );  \n"
            "\tlowp vec3 normalTexX = normalTex.rgb*2.0 -1.0;  \n"
            "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t \n"
            "\tfloat specularIntensity1 = pow( clamp(dot( v_specular_dir[0],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
            "\tfloat specularIntensity2 = pow( clamp(dot( v_specular_dir[1],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
            "\t//lowp vec4 specular1 = texture2D( s_texture[2], vec2(abs(dot( v_specular_dir[0],normalTexX )),normalTex.a) );  \n"
            "\tlowp float diffuseIntensity1  = clamp( dot( normalTexX    ,v_lightvec[0] ), 0.0, 1.0 );  \n"
            "\t//lowp vec4 specular2 = texture2D( s_texture[2], vec2(abs(dot( v_specular_dir[1],normalTexX )),normalTex.a) );  \n"
            "\tlowp float diffuseIntensity2  = clamp( dot( normalTexX    ,v_lightvec[1] ), 0.0, 1.0 );  \n"
            "\t//lowp vec3 rim_effect = (smoothstep(0.7, 1.0, 1.0 - dot( v_eye_dir,normalTexX )))* normalTex.a* u_RimColor;  \n"
            "   if ( (u_IsGlowMat + colorTex.a) > 1.1) {\n"
            "       gl_FragColor.rgb = colorTex.rgb;\t\n"
            "\t} else {\t\n"
            "\t\tgl_FragColor.rgb = (u_AmbientColor[0] + u_AmbientColor[1] + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2*u_DiffuseColor[1]) * colorTex.rgb  + (specularIntensity1 * u_SpecularColor[0] + specularIntensity2 * u_SpecularColor[1]) * specTex.rgb;  \n"
            "\t\tgl_FragColor.a = colorTex.a; \n"
            "\t\t//gl_FragColor.rgb = vec3( -0.5 * u_TexBias, 0.0, 0.0); \n"
            "}  \n"
            "}  \n");

        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormal = glGetAttribLocation(this->program, "a_normal");
        this->aTangent = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangent = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrix = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrix = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightDirModel[0] = glGetUniformLocation(this->program, "u_lightdirmodel[0]");
        this->uLightDirModel[1] = glGetUniformLocation(this->program, "u_lightdirmodel[1]");
        this->uEyePosModel = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTexture[0] = glGetUniformLocation(this->program, "s_texture[0]");
        this->sTexture[1] = glGetUniformLocation(this->program, "s_texture[1]");
        this->sTexture[2] = glGetUniformLocation(this->program, "s_texture[2]");
        this->uGlColor = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColor[0] = glGetUniformLocation(this->program, "u_AmbientColor[0]");
        this->uDiffuseColor[0] = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->uSpecularColor[0] = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->uAmbientColor[1] = glGetUniformLocation(this->program, "u_AmbientColor[1]");
        this->uDiffuseColor[1] = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->uSpecularColor[1] = glGetUniformLocation(this->program, "u_SpecularColor[1]");
        this->uSpecularPower = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uRimColor = glGetUniformLocation(this->program, "u_RimColor");
        this->uTexBiasDiffuse = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->uTexBiasNormal = glGetUniformLocation(this->program, "u_TexBiasNormal");
        this->uIsGlowMat = glGetUniformLocation(this->program, "u_IsGlowMat");

        glUseProgram(this->program);

        for (int i = 0; i != 3; i++) {
            if (this->sTexture[i] >= 0)
                glUniform1i(this->sTexture[i], i);
        }
    }

    void BumpShaderV3::SetInActive() {
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

    void BumpShaderV3::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrix >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrix, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrix >= 0)
            glUniformMatrix3fv(this->uModelMatrix, 1, 0, engine->normalMatrix);
        if (this->uTexBiasDiffuse >= 0)
            glUniform1f(this->uTexBiasDiffuse, Engine::lodBiasDiffuse);
        if (this->uTexBiasNormal >= 0)
            glUniform1f(this->uTexBiasNormal, Engine::lodBiasNormal);

        if (this->dirty != 0) {
            glUniform3f(this->uLightDirModel[0], engine->lightDir.x, engine->lightDir.y,
                        engine->lightDir.z);
            if (this->uEyePosModel >= 0)
                glUniform3f(this->uEyePosModel, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            if (this->uGlColor >= 0)
                glUniform4fv(this->uGlColor, 1, engine->glColor);
            if (this->uAmbientColor[0] >= 0)
                glUniform3fv(this->uAmbientColor[0], 1, (float *) engine->ambientShaded[0]);
            if (this->uDiffuseColor[0] >= 0)
                glUniform3fv(this->uDiffuseColor[0], 1, (float *) engine->diffuseShaded[0]);
            if (this->uSpecularColor[0] >= 0)
                glUniform3fv(this->uSpecularColor[0], 1, (float *) engine->specularShaded[0]);
            if (this->uSpecularPower >= 0)
                glUniform1f(this->uSpecularPower, engine->materialShininess);
            if (this->uRimColor >= 0)
                glUniform3fv(this->uRimColor, 1, (float *) engine->rimColor);
            if (this->uIsGlowMat >= 0) {
                glUniform1f(this->uIsGlowMat,
                            (float) (static_cast<Material *>(mesh->material)->addData != nullptr));
            }
            if (engine->lightCount >= 2) {
                glUniform3fv(this->uAmbientColor[1], 1, (float *) engine->ambientShaded[1]);
                glUniform3fv(this->uDiffuseColor[1], 1, (float *) engine->diffuseShaded[1]);
                glUniform3fv(this->uSpecularColor[1], 1, (float *) engine->specularShaded[1]);
                glUniform3f(this->uLightDirModel[1], engine->field_0x33c.x, engine->field_0x33c.y,
                            engine->field_0x33c.z);
            } else {
                glUniform3f(this->uAmbientColor[1], 0, 0, 0);
                glUniform3f(this->uDiffuseColor[1], 0, 0, 0);
                glUniform3f(this->uSpecularColor[1], 0, 0, 0);
                glUniform3f(this->uLightDirModel[1], engine->field_0x33c.x, engine->field_0x33c.y,
                            engine->field_0x33c.z);
            }
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

        if (mesh->uploaded == 0) {
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
        } else {
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
        }
    }
}
