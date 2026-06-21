#include "engine/core/GameData.h"
#include "game/core/Globals.h"

// The eight embedded AbyssEngine::String members are default-constructed and
// destroyed by the compiler-generated member init/teardown, in declaration order.

GameData::GameData() {}

GameData::~GameData() {}

// Minimal view of the owning ApplicationManager: the teardown hook only needs to
// recover the application-data pointer it stashed at startup. The method mangles
// into namespace AbyssEngine to match the shipped ApplicationManager symbols.
namespace AbyssEngine {
class ApplicationManager {
public:
    void* GetApplicationData();
};
} // namespace AbyssEngine

void OnDestroyApplication(AbyssEngine::Engine* engine) {
    // The engine owns its ApplicationManager at offset 0x30.
    AbyssEngine::ApplicationManager* app =
        *reinterpret_cast<AbyssEngine::ApplicationManager**>(
            reinterpret_cast<char*>(engine) + 0x30);

    GameData* data = static_cast<GameData*>(app->GetApplicationData());

    // GameData owns the game-wide Globals singleton (member `globals`, offset 0).
    if (data->globals != nullptr) {
        delete data->globals;
    }
    data->globals = nullptr;

    if (data != nullptr) {
        delete data;
    }
}
