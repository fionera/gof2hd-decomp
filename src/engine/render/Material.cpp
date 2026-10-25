#include "engine/render/Material.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "engine/render/PaintCanvas.h"

// Material's constructors/destructor are inline in Material.h (the original emits them weak in
// PaintCanvas.cpp).

namespace AbyssEngine {
    int MeshDraw(Engine *engine, Mesh *mesh);

    void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures) {
        if (!canvas || !mat)
            return;

        if (setTextures) {
            engine->SetTexturesExt(mat->textures[0], mat->textures[1], mat->textures[2], 0xffffffff);
        }
        engine->SetAddData(mat->addData, mat->addDataSize);

        // Remember the light's ambient colour so it can be restored after drawing.
        LightColor saved = {0.0f, 0.0f, 0.0f, 0.0f};
        if (mat->ambientColor.x != -10.0f) {
            saved = engine->lightAmbient;
            engine->LightSetLightColorAmbient(mat->ambientColor.x, mat->ambientColor.y, mat->ambientColor.z, 0x4000);
        }

        for (unsigned int i = 0; i < mat->meshes.count; ++i) {
            Matrix world = mat->arr_5c.data_[i] * mat->arr_2c.data_[i];
            canvas->SetWorldViewMatrix(world);
            engine->SetModelMatrix(mat->arr_2c.data_[i]);
            engine->SetUVMatrix(mat->arr_38.data_[i]);

            unsigned int packed = mat->arr_50.data_[i];
            engine->SetColor(static_cast<float>((packed >> 24) / 255.0),
                             static_cast<float>(((packed >> 16) & 0xff) / 255.0),
                             static_cast<float>(((packed >> 8) & 0xff) / 255.0),
                             static_cast<float>((packed & 0xff) / 255.0));

            MeshDraw(engine, mat->meshes.data_[i]);
        }

        if (mat->ambientColor.x != -10.0f) {
            engine->LightSetLightColorAmbient(saved.r, saved.g, saved.b, 0x4000);
        }

        mat->arr_5c.count = 0;
        mat->arr_2c.count = 0;
        mat->arr_38.count = 0;
        mat->meshes.count = 0;
        mat->arr_50.count = 0;
    }
}
