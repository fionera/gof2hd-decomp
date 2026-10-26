#include "engine/render/shaders/SpecCubeMapping.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kSpecCubeMappingVertexSource =
    "attribute highp\tvec3 a_position;   \n"
    "attribute mediump\tvec3 a_normal;\t\t\n"
    "attribute mediump\tvec2 a_texCoord;   \n"
    "uniform highp\t mat4 u_WorldMatrix;  \n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform mediump vec3 u_eyeposmodel;  \n"
    "uniform highp\t vec4 u_lightposmodel;  \n"
    "varying mediump vec3 v_reflectdir;   \n"
    "varying mediump    vec2 v_texCoord;     \n"
    "varying lowp vec3  v_DiffuseLight;\t\n"
    "varying lowp vec3  v_SpecularLight;\t\n"
    "varying highp float\tv_rimFactor; \n"
    "uniform lowp vec3   u_AmbientColor;\n"
    "uniform lowp vec3   u_DiffuseColor;\n"
    "uniform lowp vec3\tu_SpecularColor;\n"
    "uniform mediump float u_SpecularPower;\n"
    "void main()                  \n"
    "{                            \n"
    "\t// Transform position\n"
    "   gl_Position = u_WorldMatrix * vec4(a_position, 1.0);\t\t\n"
    "\t// Calculate eye direction in model space\n"
    "\tmediump vec3 eyeDir = normalize( a_position - u_eyeposmodel);\n"
    "\t//mediump vec3 eyeDir = normalize( u_eyeposmodel - a_position);\n"
    "\t// reflect eye direction over normal and transform to world space\n"
    "\tv_reflectdir = u_ModelMatrix * reflect(eyeDir, a_normal);\n"
    "\thighp vec3 lightDirLocal; \n"
    "\thighp float nDotL; \n"
    "\tlightDirLocal = u_lightposmodel.xyz; \n"
    "\tnDotL = dot(a_normal, lightDirLocal);\n"
    "\tv_DiffuseLight = vec3(max(nDotL, 0.0)); \n"
    "\t// Compute\treflection vector \n"
    "\thighp vec3 reflection = (2.0 * a_normal * nDotL) - lightDirLocal;\t\n"
    "   reflection = normalize(-reflect(lightDirLocal, a_normal)); \n"
    "\t// Compute R.V \n"
    "\thighp float rDotV = max(0.0, dot(reflection, normalize(u_eyeposmodel)));\n"
    "\t// Compute Specular term \n"
    "\tv_SpecularLight = u_SpecularColor * pow(rDotV, u_SpecularPower);\t\n"
    "\tv_DiffuseLight *= u_DiffuseColor;\n"
    "\tv_DiffuseLight += u_AmbientColor;\n"
    "   v_texCoord = a_texCoord;  \n"
    "\t//v_rimFactor = clamp( 1.2 - dot( normalize( u_eyeposmodel - a_position), a_normal ), 0.0, 1.0);  \n"
    "}                            \n";

static const char kSpecCubeMappingFragmentSource[] =
    "precision mediump float;                            \n"
    "uniform samplerCube u_texture_cubemap;\t\t\t \n"
    "uniform sampler2D\t u_texture_base;                        \n"
    "uniform highp float u_envValue;  \n"
    "uniform lowp vec4   u_glColor;\t\n"
    "varying mediump vec3 v_reflectdir;   \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying lowp vec3    v_DiffuseLight;\t\n"
    "varying lowp vec3  v_SpecularLight;\t\n"
    "varying highp float\tv_rimFactor; \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\t//lowp vec4 combineColor = mix(textureCube(u_texture_cubemap, v_reflectdir), texture2D( u_texture_base, v_texCoord ), (1.0- v_rimFactor)); \n"
    "\t//lowp vec4 combineColor = mix(textureCube(u_texture_cubemap, v_reflectdir), texture2D( u_texture_base, v_texCoord ), u_envValue); \n"
    "   lowp vec4 diffuseColor = texture2D( u_texture_base, v_texCoord ); \n"
    "\t//gl_FragColor.xyz =  u_glColor.xyz*((v_DiffuseLight) * combineColor.xyz + v_SpecularLight);\n"
    "\tgl_FragColor.xyz =  u_glColor.xyz*((v_DiffuseLight) * diffuseColor.xyz + 0.5*diffuseColor.a * textureCube(u_texture_cubemap, v_reflectdir).xyz);\n"
    "\tgl_FragColor.a = u_glColor.a; \n"
    "   //gl_FragColor = vec4(v_rimFactor, v_rimFactor, v_rimFactor, 1.0); \n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int SpecCubeMapping::ShaderIndex = -1;

    SpecCubeMapping::SpecCubeMapping() {
        SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("SpecCubeMapping", false);
    }

    void SpecCubeMapping::Init(Engine *) {
        int program = this->LoadBindShader("data/shader/SpecCubeMapping.vs", "data/shader/SpecCubeMapping.fs");
        this->program = program;
        if (program == 0) {
            program = this->ES2LoadProgram(kSpecCubeMappingVertexSource, kSpecCubeMappingFragmentSource);
            this->program = program;
        }

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");

        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uLightposmodelLoc = glGetUniformLocation(this->program, "u_lightposmodel");
        this->uTextureBaseLoc = glGetUniformLocation(this->program, "u_texture_base");
        this->uTextureCubemapLoc = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uEnvValueLoc = glGetUniformLocation(this->program, "u_envValue");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uGlColorLoc = glGetUniformLocation(this->program, "u_glColor");

        glUseProgram(this->program);
        glUniform1i(this->uTextureBaseLoc, 0);
        glUniform1i(this->uTextureCubemapLoc, 1);
    }

    void SpecCubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            glUniform3fv(this->uAmbientColorLoc, 1, (const float *) engine->ambientShaded[0]);
            glUniform3fv(this->uDiffuseColorLoc, 1, (const float *) engine->diffuseShaded[0]);
            glUniform3fv(this->uSpecularColorLoc, 1, (const float *) engine->specularShaded[0]);
            glUniform1f(this->uSpecularPowerLoc, engine->materialShininess);
            this->dirty = 0;
        }

        glUniform1f(this->uEnvValueLoc, engine->field_0xcc);
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);
        glUniform4f(this->uLightposmodelLoc,
                    engine->lightDir[0].x, engine->lightDir[0].y,
                    engine->lightDir[0].z, engine->lightDirty[0]);
        glUniform3f(this->uEyeposmodelLoc,
                    engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);

        glEnableVertexAttribArray(this->aPositionLoc);
        glEnableVertexAttribArray(this->aTexCoordLoc);
        glEnableVertexAttribArray(this->aNormalLoc);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            uint8_t flags = mesh->vertexFormat;
            if (flags & 2) {
                glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
                flags = mesh->vertexFormat;
            }
            if (((uint32_t) flags << 0x1d) & 0x80000000u) {
                glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
            }
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, 0);
        }
    }

    void SpecCubeMapping::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
        glDisableVertexAttribArray(this->aNormalLoc);
        glDisableVertexAttribArray(this->aTexCoordLoc);
    }
}
