#include "game/core/BumpRimCubeShader_new.h"
#include <GLES2/gl2.h>
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"


static const char *const kBumpRimCubeShader_newVertexSource =
    "attribute highp vec3 \ta_position;  \n"
    "attribute highp vec2  a_texCoord;    \n"
    "attribute highp vec3  \ta_normal; \t  \n"
    "attribute lowp \tvec3 \ta_tangent;\t\t  \n"
    "attribute lowp \tvec3 \ta_bitangent;\t  \n"
    "varying highp vec2 v_texCoord;  \n"
    "varying lowp vec3 v_eye_dir;  \n"
    "varying lowp vec3 v_specular_dir[2];  \n"
    "varying lowp vec3 v_lightvec[2];  \n"
    "varying mediump vec3 v_reflectdir;   \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "uniform highp vec3 u_lightdirmodel[2];  \n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform mediump mat4 u_ModelMatrixFull;  \n"
    "uniform highp float u_fogMaxDist; \n"
    "uniform highp float u_fogMinDist;\n"
    "varying lowp float v_FogFactor;\n"
    "varying highp float v_z;\n"
    "lowp float computeLinearFogFactor(highp float eyeDist) { \n"
    "    highp float factor; \n"
    "    // Compute linear fog equation \n"
    "    factor = (u_fogMaxDist - eyeDist) / (u_fogMaxDist - u_fogMinDist);\n"
    "    // Clamp in the [0,1] range \n"
    "    factor = (1.0 - clamp(factor, 0.0, 1.0));\n"
    "    return factor; \n"
    "}    \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);  \n"
    "   v_z = gl_Position.z; \n"
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
    "   highp float eyeDist = distance(u_eyeposmodel, a_position); \n"
    "   v_FogFactor = computeLinearFogFactor(eyeDist); \n"
    "}  \n";

static const char *const kBumpRimCubeShader_newFragmentSource =
    "precision lowp float;  \n"
    "varying highp vec2 v_texCoord;  \n"
    "varying lowp vec3 v_eye_dir;  \n"
    "varying lowp vec3 v_specular_dir[2];  \n"
    "varying lowp vec3 v_lightvec[2];  \n"
    "uniform sampler2D  s_texture[2];  \n"
    "uniform samplerCube u_texture_cubemap;\t\t\t \n"
    "uniform lowp vec3  u_AmbientColor;  \n"
    "uniform lowp vec3 u_DiffuseColor[2];\n"
    "uniform lowp vec3 u_SpecularColor[2];\n"
    "uniform mediump float u_SpecularPower;\n"
    "uniform lowp vec3 u_RimColor;\n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform highp float u_TexBiasDiffuse; \n"
    "uniform highp float u_TexBiasNormal; \n"
    "uniform lowp float u_IsGlowMat;\n"
    "uniform bool u_EnableFog; \n"
    "uniform vec3 u_fogColor; \n"
    "uniform highp float u_lodDist; \n"
    "varying lowp float v_FogFactor;\n"
    "varying highp float v_z;\n"
    "vec3 outColor; \n"
    "lowp vec4 colorTex; \n"
    "void main()  \n"
    "{  \n"
    "\tcolorTex   = texture2D( s_texture[0], v_texCoord, u_TexBiasDiffuse );  \n"
    "   if ( v_z > u_lodDist ) {\n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, u_TexBiasNormal );  \n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 -1.0;  \n"
    "\tfloat specularIntensity1 = pow( clamp(dot( v_specular_dir[0],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
    "\tlowp float diffuseIntensity1  = clamp( dot( normalTexX    ,v_lightvec[0] ), 0.0, 1.0 );  \n"
    "\tlowp float diffuseIntensity2  = clamp( dot( normalTexX    ,v_lightvec[1] ), 0.0, 1.0 );  \n"
    "       outColor = (u_AmbientColor + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2*u_DiffuseColor[1])* colorTex.rgb  + (specularIntensity1 * u_SpecularColor[0]) * normalTex.a;  \n"
    "   } else {\n"
    "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, u_TexBiasNormal );  \n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 -1.0;  \n"
    "\tfloat specularIntensity1 = pow( clamp(dot( v_specular_dir[0],normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
    "\tlowp float diffuseIntensity1  = clamp( dot( normalTexX    ,v_lightvec[0] ), 0.0, 1.0 );  \n"
    "\tlowp float diffuseIntensity2  = clamp( dot( normalTexX    ,v_lightvec[1] ), 0.0, 1.0 );  \n"
    "\tfloat reflectTex = (normalTex.a*normalTex.a);\t\n"
    "\tfloat rim_intensity = (smoothstep(0.8, 1.0, 1.0 - dot( v_eye_dir,normalTexX )));\t\n"
    "\tlowp vec3 rim_effect = rim_intensity* reflectTex* u_RimColor;  \n"
    "\tfloat reflectivity = clamp(smoothstep(0.6, 1.0, 1.0 - dot( v_eye_dir,normalTexX )),reflectTex,(normalTex.a * 2.0 + rim_intensity));  \n"
    "\tmediump vec3 reflectDir = u_ModelMatrix * reflect(v_eye_dir, normalTexX);\n"
    "       outColor = (u_AmbientColor + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2*u_DiffuseColor[1])* colorTex.rgb  + textureCube(u_texture_cubemap, reflectDir).rgb * reflectivity + (specularIntensity1 * u_SpecularColor[0]) * normalTex.a + rim_effect;  \n"
    "   } \n"
    "   if ( u_EnableFog ) { \n"
    "       gl_FragColor.rgb = mix(outColor, u_fogColor, v_FogFactor); \n"
    "       gl_FragColor.a = colorTex.a;\n"
    "   } else { \n"
    "       gl_FragColor.rgb = outColor; \n"
    "       gl_FragColor.a = colorTex.a;\n"
    "   } \n"
    "}  \n";

namespace AbyssEngine {
    int BumpRimCubeShader_new::ShaderIndex = -1;

    BumpRimCubeShader_new::BumpRimCubeShader_new() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpRimCubeShader_new", false);
    }

    void BumpRimCubeShader_new::Init(Engine *) {
        int program = this->LoadBindShader("data/shader/BumpRimCubeShader_new.vs", "data/shader/BumpRimCubeShader_new.fs");
        this->program = program;
        if (program == 0) {
            program = this->ES2LoadProgram(kBumpRimCubeShader_newVertexSource, kBumpRimCubeShader_newFragmentSource);
            this->program = program;
        }

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uModelMatrixFullLoc = glGetUniformLocation(this->program, "u_ModelMatrixFull");
        this->uLightdirmodel0Loc = glGetUniformLocation(this->program, "u_lightdirmodel[0]");
        this->uLightdirmodel1Loc = glGetUniformLocation(this->program, "u_lightdirmodel[1]");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTexture0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        this->sTexture1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        this->uTextureCubemapLoc = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColor0Loc = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->uSpecularColor0Loc = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->uDiffuseColor1Loc = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->uSpecularColor1Loc = glGetUniformLocation(this->program, "u_SpecularColor[1]");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uRimColorLoc = glGetUniformLocation(this->program, "u_RimColor");
        this->uTexBiasDiffuseLoc = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->uTexBiasNormalLoc = glGetUniformLocation(this->program, "u_TexBiasNormal");
        this->uIsGlowMatLoc = glGetUniformLocation(this->program, "u_IsGlowMat");
        this->uFogColorLoc = glGetUniformLocation(this->program, "u_fogColor");
        this->uFogMaxDistLoc = glGetUniformLocation(this->program, "u_fogMaxDist");
        this->uFogMinDistLoc = glGetUniformLocation(this->program, "u_fogMinDist");
        this->uEnableFogLoc = glGetUniformLocation(this->program, "u_EnableFog");
        this->uLodDistLoc = glGetUniformLocation(this->program, "u_lodDist");

        glUseProgram(this->program);
        for (int i = 0; i != 2; i++) {
            int loc = (&this->sTexture0Loc)[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
        glUniform1i(this->uTextureCubemapLoc, 7);
    }

    void BumpRimCubeShader_new::UpdateMeshData(Mesh *mesh, Engine *engine) {
        ::Engine *eng = (::Engine *) engine;
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, eng->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, eng->normalMatrix);
        if (this->uModelMatrixFullLoc >= 0)
            glUniformMatrix4fv(this->uModelMatrixFullLoc, 1, 0, eng->modelMatrixGL);
        if (this->uTexBiasDiffuseLoc >= 0)
            glUniform1f(this->uTexBiasDiffuseLoc, Engine::lodBiasDiffuse);
        if (this->uTexBiasNormalLoc >= 0)
            glUniform1f(this->uTexBiasNormalLoc, Engine::lodBiasNormal);
        if (this->uLodDistLoc >= 0)
            glUniform1f(this->uLodDistLoc, Engine::LodDistShader);

        if (this->dirty != 0) {
            glUniform3f(this->uLightdirmodel0Loc, eng->lightDir[0].x,
                        eng->lightDir[0].y, eng->lightDir[0].z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, eng->lightColor.x,
                            eng->lightColor.y, eng->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, eng->glColor);
            float buf[3];
            if (this->uAmbientColorLoc >= 0) {
                buf[0] = eng->ambientShaded[0][0] + eng->ambientShaded[1][0];
                buf[1] = eng->ambientShaded[0][1] + eng->ambientShaded[1][1];
                buf[2] = eng->ambientShaded[0][2] + eng->ambientShaded[1][2];
                glUniform3fv(this->uAmbientColorLoc, 1, buf);
            }
            if (this->uDiffuseColor0Loc >= 0)
                glUniform3fv(this->uDiffuseColor0Loc, 1, (float *) eng->diffuseShaded[0]);
            if (this->uSpecularColor0Loc >= 0)
                glUniform3fv(this->uSpecularColor0Loc, 1, (float *) eng->specularShaded[0]);
            if (this->uSpecularPowerLoc >= 0)
                glUniform1f(this->uSpecularPowerLoc, eng->materialShininess);
            if (this->uRimColorLoc >= 0)
                glUniform3fv(this->uRimColorLoc, 1, (float *) eng->rimColor);
            if (this->uFogColorLoc >= 0)
                glUniform3fv(this->uFogColorLoc, 1, eng->fogColor);
            if (this->uFogMinDistLoc >= 0)
                glUniform1f(this->uFogMinDistLoc, eng->fogMinDist);
            if (this->uFogMaxDistLoc >= 0)
                glUniform1f(this->uFogMaxDistLoc, eng->fogMaxDist);
            if (this->uEnableFogLoc >= 0)
                glUniform1i(this->uEnableFogLoc, Engine::fogEnabled);
            if (this->uIsGlowMatLoc >= 0) {
                float v = 0.0f;
                int *m30 = (int *) mesh->material;
                if (m30 != 0) {
                    v = 1.0f;
                    if (m30[9] == 0)
                        v = 0.0f;
                }
                glUniform1f(this->uIsGlowMatLoc, v);
            }
            int loc5c = this->uAmbientColorLoc;
            if (eng->lightCount >= 2) {
                if (loc5c >= 0) {
                    buf[0] = eng->ambientShaded[0][0] + eng->ambientShaded[1][0];
                    buf[1] = eng->ambientShaded[0][1] + eng->ambientShaded[1][1];
                    buf[2] = eng->ambientShaded[0][2] + eng->ambientShaded[1][2];
                    glUniform3fv(loc5c, 1, buf);
                }
                glUniform3fv(this->uDiffuseColor1Loc, 1, (float *) eng->diffuseShaded[1]);
                glUniform3fv(this->uSpecularColor1Loc, 1, (float *) eng->specularShaded[1]);
                glUniform3f(this->uLightdirmodel1Loc, eng->lightDir[1].x,
                            eng->lightDir[1].y, eng->lightDir[1].z);
            } else {
                glUniform3f(loc5c, 0, 0, 0);
                glUniform3f(this->uDiffuseColor1Loc, 0, 0, 0);
                glUniform3f(this->uSpecularColor1Loc, 0, 0, 0);
                glUniform3f(this->uLightdirmodel1Loc, eng->lightDir[1].x,
                            eng->lightDir[1].y, eng->lightDir[1].z);
            }
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

    void BumpRimCubeShader_new::SetInActive() {
        int loc;
        loc = this->aPositionLoc;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->aTexCoordLoc;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->aNormalLoc;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->aTangentLoc;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->aBitangentLoc;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
    }
}
