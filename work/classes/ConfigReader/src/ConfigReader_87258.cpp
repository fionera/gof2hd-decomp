#include "class.h"

static void config_reader_read_file_callback(void *, void *) {}
static void config_reader_keys_for_action_callback(void *, void *) {}
static void config_reader_register_token_callback(void *, void *) {}

namespace AbyssEngine {

__attribute__((minsize)) ConfigReader::ConfigReader(Engine *engine)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    new ((Array<TokenStruct *> *)this) Array<TokenStruct *>();
    config_engine(this) = engine;

    RegisterTokenReadFunction(
        String("ConfigReadFile", false),
        (ConfigTokenReadFunction)config_reader_read_file_callback, engine);
    RegisterTokenReadFunction(
        String("ConfigGetKeysForAction", false),
        (ConfigTokenReadFunction)config_reader_keys_for_action_callback, engine);
    RegisterTokenReadFunction(
        String("ConfigRegisterTokenReadFunction", false),
        (ConfigTokenReadFunction)config_reader_register_token_callback, engine);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
}

} // namespace AbyssEngine
