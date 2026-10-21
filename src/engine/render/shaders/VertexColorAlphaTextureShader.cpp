#include "engine/render/shaders/VertexColorAlphaTextureShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kVertexColorAlphaTextureShaderVertexSource =
    "attribute highp   vec4 \ta_position;   attribute mediump vec2  a_texCoord;   \tattribute highp   vec3  a_normal; \t\t\tattribute lowp    vec3 \ta_tangent;\t\tattribute lowp    vec3 \ta_bitangent;\tattribute highp    vec4  a_vertexColor;varying mediump vec2 v_texCoord;varying mediump vec3 v_light_dir;varying mediump vec3 v_eye_dir;varying mediump vec3 v_lightvec;varying mediump vec3 v_normal;varying mediump vec3 v_specular_dir;varying highp   vec4 v_color;uniform highp mat4 u_ModelViewProjectionMatrix;uniform highp vec3 u_lightdirmodel;uniform highp vec3 u_eyeposmodel;\n"
    "//uniform highp mat3 u_ModelMatrix;\n"
    "void main(){\tgl_Position = u_ModelViewProjectionMatrix * a_position;\tv_color     = a_vertexColor;\tv_texCoord  = a_texCoord;}";

static const char *const kVertexColorAlphaTextureShaderFragmentSource =
    "precision lowp float;varying mediump vec2 v_texCoord;varying mediump vec3 v_light_dir;varying mediump vec3 v_eye_dir;varying mediump vec3 v_lightvec;varying mediump vec3 v_normal;varying mediump vec3 v_specular_dir;varying highp    vec4 v_color;uniform sampler2D  s_texture;\n"
    "//uniform lowp vec4  u_AmbientColor;\n"
    "//uniform lowp vec4  u_DiffuseColor;\n"
    "//uniform lowp vec4  u_SpecularColor;\n"
    "uniform highp vec4  glColor;\t\n"
    "void main(){\tlowp vec4 colorTex   = texture2D( s_texture, v_texCoord );\tif ( colorTex.a < 0.5)\t\tdiscard;\tgl_FragColor = colorTex * v_color; // * glColor;  (Color scheint nicht benÃ¶tigt zu werden)\n"
    "}";

namespace AbyssEngine {
    int VertexColorAlphaTextureShader::ShaderIndex;

    static int g_shaderIndexSrc;
    static int g_shaderIndexDst;

    VertexColorAlphaTextureShader::VertexColorAlphaTextureShader() {
        VertexColorAlphaTextureShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        g_shaderIndexDst = g_shaderIndexSrc;
        this->name = String("VertexColorAlphaTextureShader", false);
    }

    void VertexColorAlphaTextureShader::Init(Engine *) {
        int program = this->ES2LoadProgram(kVertexColorAlphaTextureShaderVertexSource, kVertexColorAlphaTextureShaderFragmentSource);
        this->program = program;

        this->attrib0 = glGetAttribLocation(program, "a_vertexColor");
        this->attrib1 = glGetAttribLocation(this->program, "a_position");
        this->attrib2 = glGetAttribLocation(this->program, "a_texCoord");
        this->attrib3 = glGetAttribLocation(this->program, "a_normal");
        this->attrib4 = glGetAttribLocation(this->program, "a_tangent");
        this->attrib5 = glGetAttribLocation(this->program, "a_bitangent");

        this->uniform0 = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uniform1 = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uniform2 = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uniform3 = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uniform4 = glGetUniformLocation(this->program, "s_texture");
        this->uniform5 = glGetUniformLocation(this->program, "glColor");
        this->uniform6 = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uniform7 = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uniform8 = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(this->uniform4, 0);
    }

    void VertexColorAlphaTextureShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uniform0 >= 0)
            glUniformMatrix4fv(this->uniform0, 1, 0, engine->worldViewProjMatrix);
        if (this->uniform1 >= 0)
            glUniformMatrix3fv(this->uniform1, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uniform2 >= 0)
                glUniform3f(this->uniform2, engine->lightDir.x,
                            engine->lightDir.y, engine->lightDir.z);
            if (this->uniform3 >= 0)
                glUniform3f(this->uniform3, engine->lightColor.x,
                            engine->lightColor.y, engine->lightColor.z);
            if (this->uniform5 >= 0)
                glUniform4fv(this->uniform5, 1, engine->glColor);
            if (this->uniform6 >= 0)
                glUniform4fv(this->uniform6, 1, engine->materialAmbient);
            if (this->uniform7 >= 0)
                glUniform4fv(this->uniform7, 1, engine->materialDiffuse);
            if (this->uniform8 >= 0)
                glUniform4fv(this->uniform8, 1, engine->materialSpecular);
            this->dirty = 0;
        }

        if (this->attrib1 >= 0)
            glEnableVertexAttribArray(this->attrib1);
        if (this->attrib2 >= 0)
            glEnableVertexAttribArray(this->attrib2);
        if (this->attrib3 >= 0)
            glEnableVertexAttribArray(this->attrib3);
        if (this->attrib4 >= 0)
            glEnableVertexAttribArray(this->attrib4);
        if (this->attrib5 >= 0)
            glEnableVertexAttribArray(this->attrib5);
        if (this->attrib0 >= 0)
            glEnableVertexAttribArray(this->attrib0);

        if (mesh->uploaded == 0) {
            if (this->attrib1 >= 0)
                glVertexAttribPointer(this->attrib1, 3, 0x1406, 0, 0, mesh->positions);
            if (this->attrib2 >= 0)
                glVertexAttribPointer(this->attrib2, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->attrib3 >= 0)
                glVertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, mesh->normals);
            if (this->attrib4 >= 0)
                glVertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->attrib5 >= 0)
                glVertexAttribPointer(this->attrib5, 3, 0x1406, 0, 0, mesh->binormals);
            if (this->attrib0 >= 0)
                glVertexAttribPointer(this->attrib0, 4, 0x1406, 0, 0, mesh->colors);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->attrib1, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->attrib2, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->attrib5, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer(this->attrib0, 4, 0x1406, 0, 0, 0);
        }
    }

    void VertexColorAlphaTextureShader::SetInActive() {
        if (this->attrib1 >= 0)
            glDisableVertexAttribArray(this->attrib1);
        if (this->attrib2 >= 0)
            glDisableVertexAttribArray(this->attrib2);
        if (this->attrib3 >= 0)
            glDisableVertexAttribArray(this->attrib3);
        if (this->attrib4 >= 0)
            glDisableVertexAttribArray(this->attrib4);
        if (this->attrib5 >= 0)
            glDisableVertexAttribArray(this->attrib5);
        if (this->attrib0 >= 0)
            glDisableVertexAttribArray(this->attrib0);
    }
}
