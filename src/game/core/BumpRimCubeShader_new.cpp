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
    int BumpRimCubeShader_new::ShaderIndex;

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

        this->attrib0 = glGetAttribLocation(program, "a_position");
        this->attrib1 = glGetAttribLocation(this->program, "a_texCoord");
        this->attrib2 = glGetAttribLocation(this->program, "a_normal");
        this->attrib3 = glGetAttribLocation(this->program, "a_tangent");
        this->attrib4 = glGetAttribLocation(this->program, "a_bitangent");

        this->uniform0 = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uniform1 = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uniform2 = glGetUniformLocation(this->program, "u_ModelMatrixFull");
        this->uniform3 = glGetUniformLocation(this->program, "u_lightdirmodel[0]");
        this->uniform4 = glGetUniformLocation(this->program, "u_lightdirmodel[1]");
        this->uniform5 = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uniform6 = glGetUniformLocation(this->program, "s_texture[0]");
        this->uniform7 = glGetUniformLocation(this->program, "s_texture[1]");
        this->uniform8 = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uniform9 = glGetUniformLocation(this->program, "glColor");
        this->uniform10 = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uniform11 = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->uniform13 = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->uniform12 = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->uniform14 = glGetUniformLocation(this->program, "u_SpecularColor[1]");
        this->uniform15 = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uniform16 = glGetUniformLocation(this->program, "u_RimColor");
        this->uniform17 = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->uniform18 = glGetUniformLocation(this->program, "u_TexBiasNormal");
        this->uniform19 = glGetUniformLocation(this->program, "u_IsGlowMat");
        this->uniform20 = glGetUniformLocation(this->program, "u_fogColor");
        this->uniform21 = glGetUniformLocation(this->program, "u_fogMaxDist");
        this->uniform22 = glGetUniformLocation(this->program, "u_fogMinDist");
        this->uniform23 = glGetUniformLocation(this->program, "u_EnableFog");
        this->uniform24 = glGetUniformLocation(this->program, "u_lodDist");

        glUseProgram(this->program);
        for (int i = 0; i != 2; i++) {
            int loc = (&this->uniform6)[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
        glUniform1i(this->uniform8, 7);
    }

    void BumpRimCubeShader_new::UpdateMeshData(Mesh *mesh, Engine *engine) {
        ::Engine *eng = (::Engine *) engine;
        if (this->uniform0 >= 0)
            glUniformMatrix4fv(this->uniform0, 1, 0, eng->worldViewProjMatrix);
        if (this->uniform1 >= 0)
            glUniformMatrix3fv(this->uniform1, 1, 0, eng->normalMatrix);
        if (this->uniform2 >= 0)
            glUniformMatrix4fv(this->uniform2, 1, 0, eng->modelMatrixGL);
        if (this->uniform17 >= 0)
            glUniform1f(this->uniform17, Engine::lodBiasDiffuse);
        if (this->uniform18 >= 0)
            glUniform1f(this->uniform18, Engine::lodBiasNormal);
        if (this->uniform24 >= 0)
            glUniform1f(this->uniform24, Engine::LodDistShader);

        if (this->dirty != 0) {
            glUniform3f(this->uniform3, eng->lightDir.x,
                        eng->lightDir.y, eng->lightDir.z);
            if (this->uniform5 >= 0)
                glUniform3f(this->uniform5, eng->lightColor.x,
                            eng->lightColor.y, eng->lightColor.z);
            if (this->uniform9 >= 0)
                glUniform4fv(this->uniform9, 1, eng->glColor);
            float buf[3];
            if (this->uniform10 >= 0) {
                buf[0] = eng->lightAmbientShaded.x + eng->lightSpecularShaded.x;
                buf[1] = eng->lightAmbientShaded.y + eng->lightSpecularShaded.y;
                buf[2] = eng->lightAmbientShaded.z + eng->lightSpecularShaded.z;
                glUniform3fv(this->uniform10, 1, buf);
            }
            if (this->uniform11 >= 0)
                glUniform3fv(this->uniform11, 1, (float *) &eng->field_0x2fc);
            if (this->uniform13 >= 0)
                glUniform3fv(this->uniform13, 1, (float *) &eng->lightDiffuseShaded);
            if (this->uniform15 >= 0)
                glUniform1f(this->uniform15, eng->materialShininess);
            if (this->uniform16 >= 0)
                glUniform3fv(this->uniform16, 1, (float *) &eng->rimColor);
            if (this->uniform20 >= 0)
                glUniform3fv(this->uniform20, 1, eng->fogColor);
            if (this->uniform22 >= 0)
                glUniform1f(this->uniform22, eng->fogMinDist);
            if (this->uniform21 >= 0)
                glUniform1f(this->uniform21, eng->fogMaxDist);
            if (this->uniform23 >= 0)
                glUniform1i(this->uniform23, Engine::fogEnabled);
            if (this->uniform19 >= 0) {
                float v = 0.0f;
                int *m30 = (int *) mesh->material;
                if (m30 != 0) {
                    v = 1.0f;
                    if (m30[9] == 0)
                        v = 0.0f;
                }
                glUniform1f(this->uniform19, v);
            }
            int loc5c = this->uniform10;
            if (eng->lightCount >= 2) {
                if (loc5c >= 0) {
                    buf[0] = eng->lightAmbientShaded.x + eng->lightSpecularShaded.x;
                    buf[1] = eng->lightAmbientShaded.y + eng->lightSpecularShaded.y;
                    buf[2] = eng->lightAmbientShaded.z + eng->lightSpecularShaded.z;
                    glUniform3fv(loc5c, 1, buf);
                }
                glUniform3fv(this->uniform12, 1, (float *) &eng->field_0x308);
                glUniform3fv(this->uniform14, 1, (float *) &eng->particleAmbient);
                glUniform3f(this->uniform4, eng->field_0x33c.x,
                            eng->field_0x33c.y, eng->field_0x33c.z);
            } else {
                glUniform3f(loc5c, 0, 0, 0);
                glUniform3f(this->uniform12, 0, 0, 0);
                glUniform3f(this->uniform14, 0, 0, 0);
                glUniform3f(this->uniform4, eng->field_0x33c.x,
                            eng->field_0x33c.y, eng->field_0x33c.z);
            }
            this->dirty = 0;
        }

        if (this->attrib0 >= 0)
            glEnableVertexAttribArray(this->attrib0);
        if (this->attrib1 >= 0)
            glEnableVertexAttribArray(this->attrib1);
        if (this->attrib2 >= 0)
            glEnableVertexAttribArray(this->attrib2);
        if (this->attrib3 >= 0)
            glEnableVertexAttribArray(this->attrib3);
        if (this->attrib4 >= 0)
            glEnableVertexAttribArray(this->attrib4);

        if (mesh->uploaded == 0) {
            if (this->attrib0 >= 0)
                glVertexAttribPointer(this->attrib0, 3, 0x1406, 0, 0, mesh->positions);
            if (this->attrib1 >= 0)
                glVertexAttribPointer(this->attrib1, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->attrib2 >= 0)
                glVertexAttribPointer(this->attrib2, 3, 0x1406, 0, 0, mesh->normals);
            if (this->attrib3 >= 0)
                glVertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->attrib4 >= 0)
                glVertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->attrib0, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->attrib1, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->attrib2, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, 0);
        }
    }

    void BumpRimCubeShader_new::SetInActive() {
        int loc;
        loc = this->attrib0;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->attrib1;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->attrib2;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->attrib3;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
        loc = this->attrib4;
        if (loc >= 0)
            glDisableVertexAttribArray(loc);
    }
}
