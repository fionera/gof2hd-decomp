#include "engine/render/shaders/CubeNormalMapping.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kCubeNormalMappingVertexSource =
    "attribute highp\tvec3 a_position;   \n"
    "attribute mediump\tvec3 a_normal;\t\t\n"
    "attribute mediump\tvec2 a_texCoord;   \n"
    "attribute lowp \tvec3 a_tangent;\t\t  \n"
    "attribute lowp \tvec3 a_bitangent;\t  \n"
    "uniform highp\t mat4 u_WorldMatrix;  \n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform mediump vec3 u_eyeposmodel;  \n"
    "uniform highp\t vec3 u_lightposmodel;  \n"
    "varying mediump vec3 v_reflectdir;   \n"
    "varying mediump    vec2 v_texCoord;     \n"
    "varying lowp vec4  v_DiffuseLight;\t\n"
    "varying lowp vec3 v_specular_dir;  \n"
    "void main()                  \n"
    "{                            \n"
    "\t// Transform position\n"
    "   gl_Position = u_WorldMatrix * vec4(a_position, 1.0);\t\t\n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent.x, a_bitangent.x, a_normal.x,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.y, a_bitangent.y, a_normal.y,  \n"
    "\t\t\t\t\t\t\t\t\t\ta_tangent.z, a_bitangent.z, a_normal.z  \n"
    "\t\t\t\t\t\t\t\t\t\t);  \n"
    "\tvec3 shininesDirection = normalize(normalize(u_eyeposmodel - a_position.xyz) + u_lightposmodel) ;  \n"
    "\tv_specular_dir = tangentSpaceXform * shininesDirection;  \n"
    "\t// Calculate eye direction in model space\n"
    "\tmediump vec3 eyeDir = normalize(a_position - u_eyeposmodel);\n"
    "\t// reflect eye direction over normal and transform to world space\n"
    "\tv_reflectdir = u_ModelMatrix * reflect(eyeDir, a_normal);\n"
    "   v_texCoord = a_texCoord;  \n"
    "\tv_DiffuseLight.rgb = vec3(max(dot(normalize(a_normal), normalize(u_lightposmodel)), 0.0)); \n"
    "\tv_DiffuseLight.a = 1.0; \n"
    "}                            \n";

static const char *const kCubeNormalMappingFragmentSource =
    "precision mediump float;                            \n"
    "uniform samplerCube u_texture_cubemap;\t\t\t \n"
    "uniform sampler2D\t u_texture_base;                        \n"
    "uniform sampler2D\t u_normalTexture;\t\n"
    "uniform highp float u_envValue;  \n"
    "uniform lowp vec4   u_AmbientColor;\n"
    "uniform lowp vec4   u_DiffuseColor;\n"
    "uniform lowp vec4   u_SpecularColor;\n"
    "uniform lowp vec4   u_glColor;\t\n"
    "uniform mediump float  u_SpecularPower;\n"
    "varying mediump vec3 v_reflectdir;   \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying lowp vec4    v_DiffuseLight;\t\n"
    "varying lowp vec3\t  v_specular_dir;  \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tlowp vec4 normalTex  = texture2D( u_normalTexture, v_texCoord, -1.0 );  \n"
    "\tlowp vec3 normalTexX = normalTex.rgb*2.0 -1.0;  \n"
    "\tfloat specularIntensity = pow( clamp(dot( v_specular_dir,normalTexX ), 0.0, 1.0), u_SpecularPower );  \n"
    "\tlowp vec4 combineColor = mix(textureCube(u_texture_cubemap, v_reflectdir), texture2D( u_texture_base, v_texCoord ), u_envValue); \n"
    "\tgl_FragColor =  u_glColor*(u_AmbientColor + v_DiffuseLight * u_DiffuseColor) * combineColor + (specularIntensity * u_SpecularColor);\n"
    "\t//gl_FragColor =  textureCube(u_texture_cubemap, v_reflectdir);\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int CubeNormalMapping::ShaderIndex = -1;

    CubeNormalMapping::CubeNormalMapping() {
        CubeNormalMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("CubeNormalMapping", false);
    }

    void CubeNormalMapping::Init(Engine *) {
        int program = this->ES2LoadProgram(kCubeNormalMappingVertexSource, kCubeNormalMappingFragmentSource);
        this->program = program;

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uLightposmodelLoc = glGetUniformLocation(this->program, "u_lightposmodel");
        this->uTextureBaseLoc = glGetUniformLocation(this->program, "u_texture_base");
        this->uTextureCubemapLoc = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uNormalTextureLoc = glGetUniformLocation(this->program, "u_normalTexture");
        this->uEnvValueLoc = glGetUniformLocation(this->program, "u_envValue");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uGlColorLoc = glGetUniformLocation(this->program, "u_glColor");

        glUseProgram(this->program);
        glUniform1i(this->uTextureBaseLoc, 0);
        glUniform1i(this->uNormalTextureLoc, 1);
        glUniform1i(this->uTextureCubemapLoc, 2);
    }

    void CubeNormalMapping::UpdateMeshData(Mesh *meshArg, Engine *engine) {
        Mesh *mesh = meshArg;
        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            glUniform4fv(this->uAmbientColorLoc, 1, (float *) &engine->lightAmbientShaded);
            glUniform4fv(this->uDiffuseColorLoc, 1, (float *) &engine->field_0x2fc);
            glUniform4fv(this->uSpecularColorLoc, 1, (float *) &engine->lightDiffuseShaded);
            glUniform1f(this->uSpecularPowerLoc, engine->materialShininess);
            this->dirty = 0;
        }

        glUniform1f(this->uEnvValueLoc, engine->field_0xcc);
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);
        glUniform3f(this->uLightposmodelLoc, engine->lightDir.x, engine->lightDir.y,
                    engine->lightDir.z);
        glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y,
                    engine->lightColor.z);

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);
        glEnableVertexAttribArray(this->aNormalLoc);
        glEnableVertexAttribArray(this->aTangentLoc);
        glEnableVertexAttribArray(this->aBitangentLoc);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if ((mesh->vertexFormat & 2) != 0)
                glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if ((mesh->vertexFormat & 4) != 0)
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

    void CubeNormalMapping::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aNormalLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
        glDisableVertexAttribArray(this->aTangentLoc);
        glDisableVertexAttribArray(this->aBitangentLoc);
    }
}
