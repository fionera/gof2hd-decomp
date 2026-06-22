#include "engine/core/GameData.h"
#include "game/core/Globals.h"

GameData::GameData() {
}

GameData::~GameData() {
}

namespace AbyssEngine {
    class ApplicationManager {
    public:
        void *GetApplicationData();
    };
} // namespace AbyssEngine

void OnDestroyApplication(AbyssEngine::Engine *engine) {
    // The engine owns its ApplicationManager at offset 0x30.
    AbyssEngine::ApplicationManager *app =
            *reinterpret_cast<AbyssEngine::ApplicationManager **>(
                reinterpret_cast<char *>(engine) + 0x30);

    GameData * data = static_cast<GameData *>(app->GetApplicationData());

    // GameData owns the game-wide Globals singleton (member `globals`, offset 0).
    if (data->globals != nullptr) {
        delete data->globals;
    }
    data->globals = nullptr;

    if (data != nullptr) {
        delete data;
    }
}
