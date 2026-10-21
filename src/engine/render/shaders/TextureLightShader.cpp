#include "engine/render/shaders/TextureLightShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kTextureLightShaderVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "attribute highp vec3  a_normal; \n"
    "varying mediump vec2 v_texCoord;     \n"
    "varying lowp vec3  v_DiffuseLight[2];\t\n"
    "varying lowp vec3  v_SpecularLight[2];\t\n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "uniform highp mat4 u_UvMatrix;  \n"
    "uniform mediump mat4 u_FullModelMatrix;  \n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform highp vec4 u_LightDirection[2]; \n"
    "uniform highp vec3 u_EyePosModel;  \n"
    "uniform lowp vec3 u_AmbientColor[2];\n"
    "uniform lowp vec3 u_DiffuseColor[2];\n"
    "uniform lowp vec3 u_SpecularColor[2];\n"
    "uniform lowp float u_SpecularPower;\n"
    "uniform bool u_UVAnimation; \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   if ( u_UVAnimation ) \n"
    "       v_texCoord = (u_UvMatrix*vec4(a_texCoord.x, a_texCoord.y, 0.0, 1.0)).xy;  \n"
    "   else \n"
    "       v_texCoord = a_texCoord; \n"
    "\thighp vec3 lightDirLocal; \n"
    "\thighp float nDotL; \n"
    "\tif ( u_LightDirection[0].w == 0.0 ) {\n"
    "\t\tlightDirLocal = u_LightDirection[0].xyz; \n"
    "\t\tnDotL = dot(a_normal, lightDirLocal);\n"
    "\t} else {\n"
    "\t\t// lightDirLocal = normalize(u_LightDirection[0].xyz - a_position.xyz);\n"
    "\t\t// nDotL = dot(a_normal, lightDirLocal);\n"
    "\t\thighp vec4 positionWorld = u_FullModelMatrix * a_position; \n"
    "\t\tlightDirLocal = normalize(u_LightDirection[0].xyz - positionWorld.xyz);\n"
    "\t\thighp vec3 normalWorld = normalize(u_ModelMatrix * a_normal.xyz); \n"
    "\t\tnDotL = dot(normalWorld, lightDirLocal);\n"
    "\t} \n"
    "\tv_DiffuseLight[0] = vec3(max(nDotL, 0.0)); \n"
    "\t// Compute\treflection vector \n"
    "\thighp vec3 reflection = (2.0 * a_normal * nDotL) - lightDirLocal;\t\n"
    "   reflection = normalize(-reflect(lightDirLocal, a_normal)); \n"
    "\t// Compute R.V \n"
    "\thighp float rDotV = max(0.0, dot(reflection, normalize(u_EyePosModel)));\n"
    "\t// Compute Specular term \n"
    "\tv_SpecularLight[0] = u_SpecularColor[0] * pow(rDotV, u_SpecularPower);\t\n"
    "\tv_DiffuseLight[0] *= u_DiffuseColor[0];\n"
    "\tv_DiffuseLight[0] += u_AmbientColor[0];\n"
    "\tif ( u_LightDirection[1].w == 0.0 ) {\n"
    "\t\tlightDirLocal = u_LightDirection[1].xyz; \n"
    "\t\tnDotL = dot(a_normal, lightDirLocal);\n"
    "\t} else {\n"
    "\t\thighp vec4 positionWorld = u_FullModelMatrix * a_position; \n"
    "\t\tlightDirLocal = normalize(u_LightDirection[1].xyz - positionWorld.xyz);\n"
    "\t\thighp vec3 normalWorld = normalize(u_ModelMatrix * a_normal.xyz); \n"
    "\t\tnDotL = dot(normalWorld, lightDirLocal);\n"
    "\t} \n"
    "\tv_DiffuseLight[1] = vec3(max(nDotL, 0.0)); \n"
    "\t// Compute reflection vector \n"
    "\treflection = (2.0 * a_normal * nDotL) - lightDirLocal;\t\n"
    "   reflection = normalize(-reflect(lightDirLocal, a_normal)); \n"
    "\t// Compute R.V \n"
    "\trDotV = max(0.0, dot(reflection, normalize(u_EyePosModel)));\n"
    "\t// Compute Specular term \n"
    "\tv_SpecularLight[1] = u_SpecularColor[1] * pow(rDotV, u_SpecularPower);\t\n"
    "\tv_DiffuseLight[1] *= u_DiffuseColor[1];\n"
    "\tv_DiffuseLight[1] += u_AmbientColor[1];\n"
    "}                            \n";

static const char *const kTextureLightShaderFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying lowp vec3  v_DiffuseLight[2];\t\n"
    "varying lowp vec3  v_SpecularLight[2];\t\n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "   lowp vec3 color = (v_DiffuseLight[0] + v_DiffuseLight[1]) * (textureColor.rgb) + v_SpecularLight[0] + v_SpecularLight[1]; \n"
    "\tgl_FragColor.rgb = glColor.rgb * color;\n"
    "\tgl_FragColor.a = glColor.a * textureColor.a; \n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int TextureLightShader::ShaderIndex;

    TextureLightShader::TextureLightShader() {
        TextureLightShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TextureLightShader", false);
    }

    void TextureLightShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kTextureLightShaderVertexSource, kTextureLightShaderFragmentSource);

        this->uUvMatrixLoc = glGetUniformLocation(this->program, "u_UvMatrix");
        this->uUVAnimationLoc = glGetUniformLocation(this->program, "u_UVAnimation");

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");

        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uFullModelMatrixLoc = glGetUniformLocation(this->program, "u_FullModelMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightDirection0Loc = glGetUniformLocation(this->program, "u_LightDirection[0]");
        this->uLightDirection1Loc = glGetUniformLocation(this->program, "u_LightDirection[1]");
        this->uEyePosModelLoc = glGetUniformLocation(this->program, "u_EyePosModel");
        this->sTextureLoc = glGetUniformLocation(this->program, "s_texture");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColor0Loc = glGetUniformLocation(this->program, "u_AmbientColor[0]");
        this->uDiffuseColor0Loc = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->uSpecularColor0Loc = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uAmbientColor1Loc = glGetUniformLocation(this->program, "u_AmbientColor[1]");
        this->uDiffuseColor1Loc = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->uSpecularColor1Loc = glGetUniformLocation(this->program, "u_SpecularColor[1]");

        glUseProgram(this->program);
        glUniform1i(this->sTextureLoc, 0);
    }

    void TextureLightShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uUvMatrixLoc >= 0)
            glUniformMatrix4fv(this->uUvMatrixLoc, 1, 0, engine->uvMatrix);
        glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);
        if (this->uFullModelMatrixLoc >= 0)
            glUniformMatrix4fv(this->uFullModelMatrixLoc, 1, 0, engine->modelMatrixGL);

        if (this->dirty != 0) {
            if (this->uUVAnimationLoc >= 0)
                glUniform1i(this->uUVAnimationLoc, mesh->hasAnimation);
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);

            glUniform3fv(this->uAmbientColor0Loc, 1, (float *) engine->ambientShaded[0]);
            glUniform3fv(this->uDiffuseColor0Loc, 1, (float *) engine->diffuseShaded[0]);
            glUniform3fv(this->uSpecularColor0Loc, 1, (float *) engine->specularShaded[0]);

            glUniform4f(this->uLightDirection0Loc, engine->lightDir.x, engine->lightDir.y,
                        engine->lightDir.z, engine->lightDirty[0]);

            if (engine->lightCount >= 2) {
                glUniform3fv(this->uAmbientColor1Loc, 1, (float *) engine->ambientShaded[1]);
                glUniform3fv(this->uDiffuseColor1Loc, 1, (float *) engine->diffuseShaded[1]);
                glUniform3fv(this->uSpecularColor1Loc, 1, (float *) engine->specularShaded[1]);
                glUniform4f(this->uLightDirection1Loc, engine->field_0x33c.x, engine->field_0x33c.y,
                            engine->field_0x33c.z, engine->lightDirty[1]);
            } else {
                glUniform3f(this->uAmbientColor1Loc, 0, 0, 0);
                glUniform3f(this->uDiffuseColor1Loc, 0, 0, 0);
                glUniform3f(this->uSpecularColor1Loc, 0, 0, 0);
                glUniform4f(this->uLightDirection1Loc, engine->field_0x33c.x, engine->field_0x33c.y,
                            engine->field_0x33c.z, engine->lightDirty[1]);
            }
            glUniform1f(this->uSpecularPowerLoc, engine->materialShininess);
            if (this->uEyePosModelLoc >= 0)
                glUniform3f(this->uEyePosModelLoc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            this->dirty = 0;
        }

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);
        glEnableVertexAttribArray(this->aNormalLoc);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, 0);
            return;
        }

        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
        glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
    }

    void TextureLightShader::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
        glDisableVertexAttribArray(this->aNormalLoc);
    }
}
