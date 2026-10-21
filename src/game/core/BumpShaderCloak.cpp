#include "game/core/BumpShaderCloak.h"
#include <GLES2/gl2.h>
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"

namespace AbyssEngine {
    int BumpShaderCloak::ShaderIndex = -1;

    BumpShaderCloak::BumpShaderCloak() {
        BumpShaderCloak::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderCloak");
    }

    void BumpShaderCloak::Init(Engine *engine) {
        this->program = this->ES2LoadProgram(
            "attribute highp vec3 a_position;\n"
            "attribute mediump vec2 a_texCoord;\n"
            "attribute highp vec3 a_normal;\n"
            "attribute lowp vec3 a_tangent;\n"
            "attribute lowp vec3 a_bitangent;\n"
            "varying mediump vec2 v_texCoord;  \n"
            "varying lowp vec3 v_eye_dir;  \n"
            "varying lowp vec3 v_specular_dir[2];\n"
            "varying lowp vec3 v_lightvec[2];\n"
            "uniform highp mat4 u_ModelViewProjectionMatrix;\n"
            "uniform highp vec3 u_lightdirmodel[2];\n"
            "uniform highp vec3 u_eyeposmodel;\n"
            "uniform mediump mat3 u_ModelMatrix;\n"
            "void main()  \n"
            "{  \n"
            "\thighp vec3 vertex = a_position;\n"
            "\tgl_Position = u_ModelViewProjectionMatrix * vec4(vertex, 1.0);\n"
            "\tv_texCoord  = a_texCoord;    \n"
            "\t// Calculate eye direction in model space\n"
            "\tmediump vec3 eyeDir = normalize( a_position - u_eyeposmodel);\n"
            "\t// reflect eye direction over normal and transform to world space\n"
            "\t//v_reflectdir = u_ModelMatrix * reflect(eyeDir, a_normal);\n"
            "\thighp mat3 tangentSpaceXform = mat3(a_tangent.x, a_bitangent.x, a_normal.x,  \n"
            "\t\t\t\t\t\t\t\t\t\ta_tangent.y, a_bitangent.y, a_normal.y,  \n"
            "\t\t\t\t\t\t\t\t\t\ta_tangent.z, a_bitangent.z, a_normal.z  \n"
            "\t\t\t\t\t\t\t\t\t\t);  \n"
            "   v_eye_dir = normalize(tangentSpaceXform*(u_eyeposmodel - a_position)); \n"
            "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position) + u_lightdirmodel[0]) ;  \n"
            "\tv_specular_dir[0] = tangentSpaceXform * shininesDirection;  \n"
            "\tv_lightvec[0] = tangentSpaceXform * u_lightdirmodel[0];  \n"
            "\tshininesDirection = normalize(normalize(u_eyeposmodel - a_position) + u_lightdirmodel[1]) ;  \n"
            "\tv_specular_dir[1] = tangentSpaceXform * shininesDirection;  \n"
            "\tv_lightvec[1] = tangentSpaceXform * u_lightdirmodel[1];  \n"
            "}\n",
            "precision lowp float;  \n"
            "varying mediump vec2 v_texCoord;\n"
            "varying lowp vec3 v_eye_dir;\n"
            "varying lowp vec3 v_specular_dir[2];\n"
            "varying lowp vec3 v_lightvec[2];\n"
            "uniform sampler2D  s_texture[3];\n"
            "uniform lowp vec3 u_AmbientColor[2];\n"
            "uniform lowp vec3 u_DiffuseColor[2];\n"
            "uniform lowp vec3 u_SpecularColor[2];\n"
            "uniform mediump float u_SpecularPower;\n"
            "uniform lowp vec3 u_RimColor;\n"
            "uniform mediump vec2 u_texelSize;\n"
            "uniform mediump float u_AnimValue;\n"
            "uniform highp float u_CloakRate;\n"
            "uniform sampler2D s_texture_base;\n"
            "uniform highp mat3 u_ModelMatrix;\n"
            "void main()\n"
            "{\n"
            "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord);\n"
            "\tmediump vec4 normalTex  = texture2D( s_texture[1], v_texCoord);\n"
            "\tmediump vec4 cloakTex   = texture2D( s_texture[2], v_texCoord);\n"
            "\tmediump vec3 normalTexX = normalTex.rgb * 2.0 - 1.0;\n"
            "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t \n"
            "\tfloat specularIntensity1 = pow( clamp(dot( v_specular_dir[0],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
            "\tfloat specularIntensity2 = pow( clamp(dot( v_specular_dir[1],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
            "\tlowp float diffuseIntensity1  = clamp( dot( normalTexX ,v_lightvec[0] ), 0.0, 1.0 );\n"
            "\tlowp float diffuseIntensity2  = clamp( dot( normalTexX ,v_lightvec[1] ), 0.0, 1.0 );\n"
            "\tfloat reflectTex = (normalTex.a*normalTex.a);\t\n"
            "\tfloat rim_intensity = (smoothstep(0.8, 1.0, 1.0 - dot( v_eye_dir,normalTexX )));\n"
            "\tlowp vec3 rim_effect = rim_intensity* reflectTex* u_RimColor;  \n"
            "\tmediump vec3 color;\n"
            "\t{\n"
            "\t\tcolor = (u_AmbientColor[0] + u_AmbientColor[1] + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2 * u_DiffuseColor[1]) * colorTex.rgb + (specularIntensity1 * u_SpecularColor[0] + specularIntensity2 * u_SpecularColor[1]) * specTex.rgb + rim_effect;\n"
            "   }\n"
            "   mediump vec2 coord;\n"
            "   coord.x = (gl_FragCoord.x + (normalTexX.x*100.0*(sin(u_CloakRate)))) * u_texelSize.x;\n"
            "   coord.y = (gl_FragCoord.y + (normalTexX.y*75.0 *(cos(u_CloakRate)))) * u_texelSize.y;\n"
            "\thighp vec3 color2 = color;\n"
            "\tif (cloakTex.r - 0.025 < u_AnimValue) {\n"
            "\t\tcolor2 = texture2D(s_texture_base, coord).rgb + vec3(0.6, 0.8, 1.0) * step(u_AnimValue-0.05, cloakTex.r);\n"
            "\t}\n"
            "\tgl_FragColor.rgb = mix(color, color2, clamp(u_AnimValue*4.0, 0.0, 1.0)) * step(u_AnimValue, 0.5) + mix(color2, texture2D(s_texture_base, coord).rgb, clamp((u_AnimValue - 0.5)*4.0, 0.0, 1.0)) * step(0.5, u_AnimValue);\n"
            "}\n");

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
        this->uTexelSize = glGetUniformLocation(this->program, "u_texelSize");

        glActiveTexture(0x84c7);
        ((::Engine *) engine)->ActivateRefractFBO();

        this->uAnimValue = glGetUniformLocation(this->program, "u_AnimValue");
        this->uCloakRate = glGetUniformLocation(this->program, "u_CloakRate");
        this->sTextureBase = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);
        glUniform1i(this->sTexture[0], 0);
        glUniform1i(this->sTexture[1], 1);
        glUniform1i(this->sTexture[2], 6);
        glUniform1i(this->sTextureBase, 7);
    }

    void BumpShaderCloak::SetInActive() {
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

    void BumpShaderCloak::UpdateMeshData(Mesh *mesh, Engine *engine) {
        ::Engine *eng = (::Engine *) engine;

        if (this->uModelViewProjectionMatrix >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrix, 1, 0, eng->worldViewProjMatrix);
        if (this->uModelMatrix >= 0)
            glUniformMatrix3fv(this->uModelMatrix, 1, 0, eng->normalMatrix);

        if (this->dirty) {
            glUniform3f(this->uLightDirModel[0], eng->lightDir.x, eng->lightDir.y, eng->lightDir.z);
            if (this->uEyePosModel >= 0)
                glUniform3f(this->uEyePosModel, eng->lightColor.x, eng->lightColor.y, eng->lightColor.z);
            if (this->uGlColor >= 0)
                glUniform4fv(this->uGlColor, 1, eng->glColor);
            if (this->uAmbientColor[0] >= 0)
                glUniform3fv(this->uAmbientColor[0], 1, (const float *) eng->ambientShaded[0]);
            if (this->uDiffuseColor[0] >= 0)
                glUniform3fv(this->uDiffuseColor[0], 1, (const float *) eng->diffuseShaded[0]);
            if (this->uSpecularColor[0] >= 0)
                glUniform3fv(this->uSpecularColor[0], 1, (const float *) eng->specularShaded[0]);
            if (this->uSpecularPower >= 0)
                glUniform1f(this->uSpecularPower, eng->materialShininess);
            if (this->uRimColor >= 0)
                glUniform3fv(this->uRimColor, 1, (const float *) eng->rimColor);

            int texelLoc = this->uTexelSize;
            if (texelLoc >= 0) {
                if (eng->appManager->paintCanvas->gameOrientation == 2) {
                    glUniform2f(texelLoc, 1.0f / (float) (int) eng->GetDisplayWidth(),
                                1.0f / (float) (int) eng->GetDisplayHeight());
                } else {
                    glUniform2f(texelLoc, 1.0f / (float) (int) eng->GetDisplayHeight(),
                                1.0f / (float) (int) eng->GetDisplayWidth());
                }
            }

            glActiveTexture(0x84c7);
            eng->ActivateRefractFBO();

            if (this->uAnimValue >= 0)
                glUniform1f(this->uAnimValue, reinterpret_cast<float &>(mesh->materialId));
            if (this->uCloakRate >= 0)
                glUniform1f(this->uCloakRate, mesh->shaderAnimValue0);

            if (eng->lightCount >= 2) {
                glUniform3fv(this->uAmbientColor[1], 1, (const float *) eng->ambientShaded[1]);
                glUniform3fv(this->uDiffuseColor[1], 1, (const float *) eng->diffuseShaded[1]);
                glUniform3fv(this->uSpecularColor[1], 1, (const float *) eng->specularShaded[1]);
                glUniform3f(this->uLightDirModel[1], eng->field_0x33c.x, eng->field_0x33c.y, eng->field_0x33c.z);
            } else {
                glUniform3f(this->uAmbientColor[1], 0.0f, 0.0f, 0.0f);
                glUniform3f(this->uDiffuseColor[1], 0.0f, 0.0f, 0.0f);
                glUniform3f(this->uSpecularColor[1], 0.0f, 0.0f, 0.0f);
                glUniform3f(this->uLightDirModel[1], eng->field_0x33c.x, eng->field_0x33c.y, eng->field_0x33c.z);
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
        } else {
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
    }
}
