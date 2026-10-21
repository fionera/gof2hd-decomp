#include "engine/render/Material.h"
#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

// Material's constructors/destructor are inline in Material.h (the original emits them weak in
// PaintCanvas.cpp).

namespace AbyssEngine {
    int MeshDraw(Engine * engine, Mesh * mesh);

    void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures) {
        if (canvas == 0 || mat == 0)
            return;

        if (setTextures) {
            engine->SetTexturesExt(mat->textures[0], mat->textures[1], mat->textures[2], 0xffffffff);
        }
        engine->SetAddData(mat->addData, (int) mat->addDataSize);

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
            engine->SetColor((float) ((packed >> 24) / 255.0), (float) (((packed >> 16) & 0xff) / 255.0),
                             (float) (((packed >> 8) & 0xff) / 255.0), (float) ((packed & 0xff) / 255.0));

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
