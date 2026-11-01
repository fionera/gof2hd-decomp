#include "platform/android/NdkHooks.h"

#include "game/mission/RecordHandler.h"

// Original TU split: the body's RecordHandler relocations are emitted after
// checkFirstCreditPackBoughtWriteAction, not at ndk_autosave's first call site.
void ndk_autosave() {
    RecordHandler *handler = new RecordHandler;
    handler->recordStoreWrite(0);
    handler->recordStoreWritePreview(0);
    delete handler;
}
