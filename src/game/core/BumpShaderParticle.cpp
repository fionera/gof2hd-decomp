#include "game/core/BumpShaderParticle.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>


namespace AbyssEngine {
    int BumpShaderParticle::ShaderIndex = -1;

    BumpShaderParticle::BumpShaderParticle() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderParticle", false);
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

        aPositionLoc = glGetAttribLocation(this->program, "a_position");
        aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");
        aVertexColorLoc = glGetAttribLocation(this->program, "a_VertexColor");

        uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        sTexture0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        sTexture1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");
        uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        uTexBiasDiffuseLoc = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        uTexBiasNormalLoc = glGetUniformLocation(this->program, "u_TexBiasNormal");

        glUseProgram(this->program);

        int *samplers = &sTexture0Loc;
        for (int i = 0; i != 2; i++) {
            int loc = samplers[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
    }

    void BumpShaderParticle::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (uModelMatrixLoc >= 0)
            glUniformMatrix3fv(uModelMatrixLoc, 1, 0, engine->normalMatrix);
        if (uTexBiasDiffuseLoc >= 0)
            glUniform1f(uTexBiasDiffuseLoc, Engine::lodBiasDiffuse);
        if (uTexBiasNormalLoc >= 0)
            glUniform1f(uTexBiasNormalLoc, Engine::lodBiasNormal);

        if (this->dirty != 0) {
            glUniform3f(uLightdirmodelLoc, engine->lightDir[0].x,
                        engine->lightDir[0].y, engine->lightDir[0].z);
            if (uEyeposmodelLoc >= 0)
                glUniform3f(uEyeposmodelLoc, engine->lightColor.x,
                            engine->lightColor.y, engine->lightColor.z);
            if (uGlColorLoc >= 0)
                glUniform4fv(uGlColorLoc, 1, engine->glColor);
            if (uAmbientColorLoc >= 0)
                glUniform3fv(uAmbientColorLoc, 1, (float *) engine->particleAmbientColor);
            if (uDiffuseColorLoc >= 0)
                glUniform3fv(uDiffuseColorLoc, 1, (float *) engine->diffuseShaded[0]);
            if (uSpecularColorLoc >= 0)
                glUniform3fv(uSpecularColorLoc, 1, (float *) engine->specularShaded[0]);
            if (uSpecularPowerLoc >= 0)
                glUniform1f(uSpecularPowerLoc, engine->materialShininess);
            this->dirty = 0;
        }

        if (aPositionLoc >= 0)
            glEnableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glEnableVertexAttribArray(aTexCoordLoc);
        if (aNormalLoc >= 0)
            glEnableVertexAttribArray(aNormalLoc);
        if (aTangentLoc >= 0)
            glEnableVertexAttribArray(aTangentLoc);
        if (aBitangentLoc >= 0)
            glEnableVertexAttribArray(aBitangentLoc);
        if (aVertexColorLoc >= 0)
            glEnableVertexAttribArray(aVertexColorLoc);

        if (mesh->uploaded == 0) {
            if (aPositionLoc >= 0)
                glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (aTexCoordLoc >= 0)
                glVertexAttribPointer(aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (aNormalLoc >= 0)
                glVertexAttribPointer(aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
            if (aTangentLoc >= 0)
                glVertexAttribPointer(aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
            if (aBitangentLoc >= 0)
                glVertexAttribPointer(aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
            if (aVertexColorLoc >= 0)
                glVertexAttribPointer(aVertexColorLoc, 4, 0x1406, 0, 0, mesh->colors);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(aNormalLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(aTangentLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(aBitangentLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer(aVertexColorLoc, 4, 0x1406, 0, 0, 0);
        }
    }

    void BumpShaderParticle::SetInActive() {
        if (aPositionLoc >= 0)
            glDisableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glDisableVertexAttribArray(aTexCoordLoc);
        if (aNormalLoc >= 0)
            glDisableVertexAttribArray(aNormalLoc);
        if (aTangentLoc >= 0)
            glDisableVertexAttribArray(aTangentLoc);
        if (aBitangentLoc >= 0)
            glDisableVertexAttribArray(aBitangentLoc);
        if (aVertexColorLoc >= 0)
            glDisableVertexAttribArray(aVertexColorLoc);
    }
}
