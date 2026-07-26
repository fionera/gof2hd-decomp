#include "game/core/BumpShaderParticle.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static float g_particleGlobalA;
static float g_particleGlobalB;

namespace AbyssEngine {
    int BumpShaderParticle::ShaderIndex;

    BumpShaderParticle::BumpShaderParticle() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = u"BumpShaderParticle";
    }

    void BumpShaderParticle::Init(Engine *) {
        this->program = this->ES2LoadProgram(
            "attribute highp vec4 \ta_position;  \n"
            "attribute mediump vec2  a_texCoord;    \n"
            "attribute highp vec3  \ta_normal; \t  \n"
            "attribute lowp \tvec3 \ta_tangent;\t\t  \n"
            "attribute lowp \tvec3 \ta_bitangent;\t  \n"
            "attribute mediump vec4 a_VertexColor;   \n"
            "varying mediump vec4 v_VertexColor;     \n"
            "varying mediump vec2 v_texCoord;  \n"
            "varying lowp vec3 v_eye_dir;  \n"
            "varying mediump vec3 v_normal;  \n"
            "varying lowp vec3 v_specular_dir;  \n"
            "varying lowp vec3 v_lightvec;  \n"
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
            "   v_eye_dir = normalize(tangentSpaceXform*(u_eyeposmodel - a_position.xyz)); \n"
            "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightdirmodel) ;  \n"
            "\tv_specular_dir = tangentSpaceXform * shininesDirection;  \n"
            "\tv_lightvec = tangentSpaceXform * u_lightdirmodel;  \n"
            "   v_VertexColor = a_VertexColor;  \n"
            "}  \n",
            "precision lowp float;  \n"
            "varying mediump vec4 v_VertexColor;     \n"
            "varying mediump vec2 v_texCoord;  \n"
            "varying lowp vec3 v_eye_dir;  \n"
            "varying mediump vec3 v_normal;  \n"
            "varying lowp vec3 v_specular_dir;  \n"
            "varying lowp vec3 v_lightvec;  \n"
            "uniform sampler2D  s_texture[2];  \n"
            "uniform lowp vec3  u_AmbientColor;  \n"
            "uniform lowp vec3 u_DiffuseColor;\n"
            "uniform lowp vec3 u_SpecularColor;\n"
            "uniform mediump float u_SpecularPower;\n"
            "uniform highp float u_TexBiasDiffuse; \n"
            "uniform highp float u_TexBiasNormal; \n"
            "void main()  \n"
            "{  \n"
            "\tlowp vec4 colorTex   = texture2D( s_texture[0], v_texCoord, u_TexBiasDiffuse );  \n"
            "\tlowp vec4 normalTex  = texture2D( s_texture[1], v_texCoord, u_TexBiasNormal );  \n"
            "\tlowp vec3 normalTexX = normalTex.rgb*2.0 -1.0;  \n"
            "\tlowp vec4 specTex = vec4(vec3(normalTex.a), 0.0);\t \n"
            "\tfloat specularIntensity = pow( clamp(dot( v_specular_dir,normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
            "\tlowp float diffuseIntensity  = clamp( dot( normalTexX    ,v_lightvec ), 0.0, 1.0 );  \n"
            "\tgl_FragColor.rgb = v_VertexColor.rgb*((u_AmbientColor + diffuseIntensity*u_DiffuseColor) * colorTex.rgb  + (specularIntensity * u_SpecularColor) * specTex.rgb);  \n"
            "\tgl_FragColor.a = v_VertexColor.a*colorTex.a; \n"
            "}  \n");

        attribA0 = glGetAttribLocation(this->program, "a_position");
        attribA1 = glGetAttribLocation(this->program, "a_texCoord");
        attribA2 = glGetAttribLocation(this->program, "a_normal");
        attribA3 = glGetAttribLocation(this->program, "a_tangent");
        attribA4 = glGetAttribLocation(this->program, "a_bitangent");
        attribA5 = glGetAttribLocation(this->program, "a_VertexColor");

        uniformU0 = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        uniformU1 = glGetUniformLocation(this->program, "u_ModelMatrix");
        uniformU2 = glGetUniformLocation(this->program, "u_lightdirmodel");
        uniformU3 = glGetUniformLocation(this->program, "u_eyeposmodel");
        uniformU4 = glGetUniformLocation(this->program, "s_texture[0]");
        uniformU5 = glGetUniformLocation(this->program, "s_texture[1]");
        uniformU6 = glGetUniformLocation(this->program, "glColor");
        uniformU7 = glGetUniformLocation(this->program, "u_AmbientColor");
        uniformU8 = glGetUniformLocation(this->program, "u_DiffuseColor");
        uniformU9 = glGetUniformLocation(this->program, "u_SpecularColor");
        uniformU10 = glGetUniformLocation(this->program, "u_SpecularPower");
        uniformU11 = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        uniformU12 = glGetUniformLocation(this->program, "u_TexBiasNormal");

        glUseProgram(this->program);

        int *samplers = &uniformU4;
        for (int i = 0; i != 2; i++) {
            int loc = samplers[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
    }

    void BumpShaderParticle::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (uniformU0 >= 0)
            glUniformMatrix4fv(uniformU0, 1, 0, engine->worldViewProjMatrix);
        if (uniformU1 >= 0)
            glUniformMatrix3fv(uniformU1, 1, 0, engine->normalMatrix);
        if (uniformU11 >= 0)
            glUniform1f(uniformU11, g_particleGlobalA);
        if (uniformU12 >= 0)
            glUniform1f(uniformU12, g_particleGlobalB);

        if (this->dirty != 0) {
            glUniform3f(uniformU2, engine->lightDir.x,
                        engine->lightDir.y, engine->lightDir.z);
            if (uniformU3 >= 0)
                glUniform3f(uniformU3, engine->lightColor.x,
                            engine->lightColor.y, engine->lightColor.z);
            if (uniformU6 >= 0)
                glUniform4fv(uniformU6, 1, engine->glColor);
            if (uniformU7 >= 0)
                glUniform3fv(uniformU7, 1, (float *) &engine->field_0x314);
            if (uniformU8 >= 0)
                glUniform3fv(uniformU8, 1, (float *) &engine->field_0x2fc);
            if (uniformU9 >= 0)
                glUniform3fv(uniformU9, 1, (float *) &engine->lightDiffuseShaded);
            if (uniformU10 >= 0)
                glUniform1f(uniformU10, engine->materialShininess);
            this->dirty = 0;
        }

        if (attribA0 >= 0)
            glEnableVertexAttribArray(attribA0);
        if (attribA1 >= 0)
            glEnableVertexAttribArray(attribA1);
        if (attribA2 >= 0)
            glEnableVertexAttribArray(attribA2);
        if (attribA3 >= 0)
            glEnableVertexAttribArray(attribA3);
        if (attribA4 >= 0)
            glEnableVertexAttribArray(attribA4);
        if (attribA5 >= 0)
            glEnableVertexAttribArray(attribA5);

        if (mesh->uploaded == 0) {
            if (attribA0 >= 0)
                glVertexAttribPointer(attribA0, 3, 0x1406, 0, 0, mesh->positions);
            if (attribA1 >= 0)
                glVertexAttribPointer(attribA1, 2, 0x1406, 0, 0, mesh->texCoords);
            if (attribA2 >= 0)
                glVertexAttribPointer(attribA2, 3, 0x1406, 0, 0, mesh->normals);
            if (attribA3 >= 0)
                glVertexAttribPointer(attribA3, 3, 0x1406, 0, 0, mesh->tangents);
            if (attribA4 >= 0)
                glVertexAttribPointer(attribA4, 3, 0x1406, 0, 0, mesh->binormals);
            if (attribA5 >= 0)
                glVertexAttribPointer(attribA5, 4, 0x1406, 0, 0, mesh->colors);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(attribA0, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(attribA1, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(attribA2, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(attribA3, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(attribA4, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer(attribA5, 4, 0x1406, 0, 0, 0);
        }
    }

    void BumpShaderParticle::SetInActive() {
        if (attribA0 >= 0)
            glDisableVertexAttribArray(attribA0);
        if (attribA1 >= 0)
            glDisableVertexAttribArray(attribA1);
        if (attribA2 >= 0)
            glDisableVertexAttribArray(attribA2);
        if (attribA3 >= 0)
            glDisableVertexAttribArray(attribA3);
        if (attribA4 >= 0)
            glDisableVertexAttribArray(attribA4);
        if (attribA5 >= 0)
            glDisableVertexAttribArray(attribA5);
    }
}
