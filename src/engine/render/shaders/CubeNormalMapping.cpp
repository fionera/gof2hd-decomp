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
    int CubeNormalMapping::ShaderIndex;

    CubeNormalMapping::CubeNormalMapping() {
        CubeNormalMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("CubeNormalMapping", false);
    }

    void CubeNormalMapping::Init(Engine *) {
        int program = this->ES2LoadProgram(kCubeNormalMappingVertexSource, kCubeNormalMappingFragmentSource);
        this->program = program;

        this->attribA0 = glGetAttribLocation(program, "a_position");
        this->attribA1 = glGetAttribLocation(this->program, "a_normal");
        this->attribA2 = glGetAttribLocation(this->program, "a_texCoord");
        this->attribA3 = glGetAttribLocation(this->program, "a_tangent");
        this->attribA4 = glGetAttribLocation(this->program, "a_bitangent");

        this->uniformU0 = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uniformU1 = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uniformU2 = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uniformU3 = glGetUniformLocation(this->program, "u_lightposmodel");
        this->uniformU4 = glGetUniformLocation(this->program, "u_texture_base");
        this->uniformU5 = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uniformU6 = glGetUniformLocation(this->program, "u_normalTexture");
        this->uniformU7 = glGetUniformLocation(this->program, "u_envValue");
        this->uniformU8 = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uniformU9 = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uniformU10 = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uniformU11 = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uniformU12 = glGetUniformLocation(this->program, "u_glColor");

        glUseProgram(this->program);
        glUniform1i(this->uniformU4, 0);
        glUniform1i(this->uniformU6, 1);
        glUniform1i(this->uniformU5, 2);
    }

    void CubeNormalMapping::UpdateMeshData(Mesh *meshArg, Engine *engine) {
        Mesh *mesh = meshArg;
        if (this->dirty != 0) {
            glUniform4fv(this->uniformU12, 1, engine->glColor);
            glUniform4fv(this->uniformU8, 1, (float *) &engine->lightAmbientShaded);
            glUniform4fv(this->uniformU9, 1, (float *) &engine->field_0x2fc);
            glUniform4fv(this->uniformU10, 1, (float *) &engine->lightDiffuseShaded);
            glUniform1f(this->uniformU11, engine->materialShininess);
            this->dirty = 0;
        }

        glUniform1f(this->uniformU7, engine->field_0xcc);
        glUniformMatrix4fv(this->uniformU0, 1, 0, engine->worldViewProjMatrix);
        glUniformMatrix3fv(this->uniformU1, 1, 0, engine->normalMatrix);
        glUniform3f(this->uniformU3, engine->lightDir.x, engine->lightDir.y,
                    engine->lightDir.z);
        glUniform3f(this->uniformU2, engine->lightColor.x, engine->lightColor.y,
                    engine->lightColor.z);

        glEnableVertexAttribArray(this->attribA0);
        glEnableVertexAttribArray(this->attribA2);
        glEnableVertexAttribArray(this->attribA1);
        glEnableVertexAttribArray(this->attribA3);
        glEnableVertexAttribArray(this->attribA4);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->attribA0, 3, 0x1406, 0, 0, mesh->positions);
            if ((mesh->vertexFormat & 2) != 0)
                glVertexAttribPointer(this->attribA2, 2, 0x1406, 0, 0, mesh->texCoords);
            if ((mesh->vertexFormat & 4) != 0)
                glVertexAttribPointer(this->attribA1, 3, 0x1406, 0, 0, mesh->normals);
            if (this->attribA3 >= 0)
                glVertexAttribPointer(this->attribA3, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->attribA4 >= 0)
                glVertexAttribPointer(this->attribA4, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->attribA0, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->attribA2, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->attribA1, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->attribA3, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->attribA4, 3, 0x1406, 0, 0, 0);
        }
    }

    void CubeNormalMapping::SetInActive() {
        glDisableVertexAttribArray(this->attribA0);
        glDisableVertexAttribArray(this->attribA1);
        glDisableVertexAttribArray(this->attribA2);
        glDisableVertexAttribArray(this->attribA3);
        glDisableVertexAttribArray(this->attribA4);
    }
}
