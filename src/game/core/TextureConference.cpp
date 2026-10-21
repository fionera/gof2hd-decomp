#include "game/core/TextureConference.h"
#include <GLES2/gl2.h>
#include "engine/core/ApplicationManager.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"

static const char *const kTextureConferenceVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "uniform highp int u_time;   \n"
    "void main()                  \n"
    "{                            \n"
    "\tfloat angle= (mod(u_time,360.0)*2.0); \n"
    "\tvec4 pos = a_position; \n"
    "\tpos.z  = sin( radians(pos.y+angle)); \n"
    "\tpos.z += sin( radians(pos.x/2.0+angle));\n"
    "\tpos.z *= pos.y * 0.09;\n"
    "\tgl_Position = u_WorldMatrix * pos;\t\t\n"
    "   v_texCoord = a_texCoord;  \n"
    "}                            \n";

static const char *const kTextureConferenceFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying highp float v_dummy;     \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tgl_FragColor = glColor * textureColor;\t\t\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int TextureConference::ShaderIndex;

    TextureConference::TextureConference() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TextureConference", false);
        this->animTime = 0;
    }

    void TextureConference::Init(Engine *) {
        this->program = this->ES2LoadProgram(kTextureConferenceVertexSource, kTextureConferenceFragmentSource);

        this->sTexture = glGetUniformLocation(this->program, "s_texture");
        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->uMvpMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uColor = glGetUniformLocation(this->program, "u_time");
        this->uOffset = glGetUniformLocation(this->program, "glColor");

        glUseProgram(this->program);
        glUniform1i(this->sTexture, 0);
    }

    void TextureConference::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uMvpMatrix, 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            glUniform4fv(this->uOffset, 1, engine->glColor);
            this->dirty = 0;
        }

        long long elapsed =
                (long long) engine->appManager->GetElapsedTimeMillis();
        long long t = this->animTime + elapsed / 5;
        this->animTime = (t > 3600) ? (t - 3600) : t;
        glUniform1i(this->uColor, (int) this->animTime);

        glEnableVertexAttribArray(this->aPosition);
        glEnableVertexAttribArray(this->aTexCoord);
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
        if ((mesh->vertexFormat & 2) != 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
    }

    void TextureConference::SetInActive() {
        glDisableVertexAttribArray(this->aPosition);
        glDisableVertexAttribArray(this->aTexCoord);
    }
}
