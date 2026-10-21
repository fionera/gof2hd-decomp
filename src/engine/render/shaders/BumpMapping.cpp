#include "engine/render/shaders/BumpMapping.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kBumpMappingVertexSource =
    "attribute highp vec4 a_position;\n"
    "attribute mediump vec3 a_normal;\n"
    "attribute mediump vec3 a_tangent;\n"
    "attribute mediump vec3 a_bitangent;\n"
    "attribute mediump vec2 a_texCoord;\n"
    "varying mediump vec2 v_texCoord;\n"
    "varying mediump vec3 v_lightvec;\n"
    "uniform highp mat4 u_WorldMatrix;\n"
    "uniform highp vec3 u_lightposmodel;\n"
    "void main()\n"
    "{\n"
    "\tgl_Position = u_WorldMatrix * a_position;\n"
    "\thighp mat3 tangentSpaceXform = mat3(a_tangent, a_bitangent, a_normal);\n"
    "\tv_lightvec =   u_lightposmodel * tangentSpaceXform;\n"
    "\tv_texCoord = a_texCoord;\n"
    "}\n";

static const char *const kBumpMappingFragmentSource =
    "precision mediump float;   \n"
    "varying mediump vec2 v_texCoord;    \n"
    "varying mediump vec3 v_lightvec;    \n"
    "uniform sampler2D s_texture_base;   \n"
    "uniform sampler2D s_texture_normal;\n"
    "void main()                      \n"
    "{                              \n"
    "\tmediump vec3 normal = normalize(texture2D(s_texture_normal, v_texCoord).rgb*2.0 - 1.0);\n"
    "\tmediump vec3 lightvec = normalize(v_lightvec);\n"
    "\tmediump float lightIntensity = max(dot(lightvec, normal), 0.0);\n"
    "\tlowp vec4 textureColor = texture2D( s_texture_base, v_texCoord );// * v_color;\t\n"
    "\t// Compute Ambient term \n"
    "\tlowp vec4 ambient = vec4(0.3, 0.3, 0.3, 1.0) * textureColor; \n"
    "\tgl_FragColor = ambient;\n"
    "\tif (lightIntensity>0.0) {\t\n"
    "\t\t// Compute Diffuse term \n"
    "\t\tlowp vec4 diffuse =  vec4(0.7, 0.7, 0.7, 1.0) * lightIntensity * textureColor;\n"
    "\t\tgl_FragColor += diffuse; \n"
    "\t}\n"
    "\t//gl_FragColor = vec4(0.5,0.5,0.5,0.5);\n"
    "}\n";

namespace AbyssEngine {
    int BumpMapping::ShaderIndex;

    BumpMapping::BumpMapping() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpMapping", false);
    }

    void BumpMapping::Init(Engine *) {
        this->program = this->ES2LoadProgram(kBumpMappingVertexSource, kBumpMappingFragmentSource);

        this->a0Loc = glGetAttribLocation(this->program, "a_position");
        this->a1Loc = glGetAttribLocation(this->program, "a_normal");
        this->a2Loc = glGetAttribLocation(this->program, "a_tangent");
        this->a3Loc = glGetAttribLocation(this->program, "a_bitangent");
        this->a4Loc = glGetAttribLocation(this->program, "a_texCoord");

        this->u0Loc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->u1Loc = glGetUniformLocation(this->program, "u_lightposmodel");
        this->u2Loc = glGetUniformLocation(this->program, "s_texture_base");
        this->u3Loc = glGetUniformLocation(this->program, "s_texture_normal");

        glUseProgram(this->program);
        glUniform1i(this->u2Loc, 0);
        glUniform1i(this->u3Loc, 1);
    }

    void BumpMapping::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->dirty != 0)
            this->dirty = 0;

        glUniformMatrix4fv(this->u0Loc, 1, 0, engine->worldViewProjMatrix);
        glUniform3f(this->u1Loc, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);

        glEnableVertexAttribArray(this->a0Loc);
        glEnableVertexAttribArray(this->a1Loc);
        glEnableVertexAttribArray(this->a2Loc);
        glEnableVertexAttribArray(this->a3Loc);
        glEnableVertexAttribArray(this->a4Loc);

        glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, mesh->positions);
        if ((mesh->vertexFormat & 2) != 0) {
            glVertexAttribPointer(this->a4Loc, 2, 0x1406, 0, 0, mesh->texCoords);
        }
        if ((mesh->vertexFormat & 4) != 0) {
            glVertexAttribPointer(this->a1Loc, 3, 0x1406, 0, 0, mesh->normals);
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, mesh->tangents);
            glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, mesh->binormals);
        }
    }

    void BumpMapping::SetInActive() {
        glDisableVertexAttribArray(this->a0Loc);
        glDisableVertexAttribArray(this->a1Loc);
        glDisableVertexAttribArray(this->a2Loc);
        glDisableVertexAttribArray(this->a3Loc);
        glDisableVertexAttribArray(this->a4Loc);
    }
}
