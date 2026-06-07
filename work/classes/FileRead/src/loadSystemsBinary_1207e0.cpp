#include "class.h"

__attribute__((minsize))
Array<SolarSystem *> *FileRead::loadSystemsBinary()
{
    uint32_t exists;
    {
        String path("systems_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<SolarSystem *> *systems = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("systems_binary.bin", &handle);

        systems = new Array<SolarSystem *>();
        ArraySetLength<SolarSystem *>(0x22, systems);

        String name;
        for (uint32_t i = 0; i < 0x22; i++) {
            int32_t faction;
            int32_t flag;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            uint32_t count;

            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(faction, handle);
            AEFile::ReadSwitched(flag, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);

            AEFile::ReadSwitched((int32_t &)count, handle);
            int32_t *routes = new int32_t[count];
            for (int32_t j = 0; j < (int32_t)count; j++) {
                AEFile::ReadSwitched(routes[j], handle);
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *stations;
            if ((int32_t)count < 1) {
                stations = 0;
            } else {
                stations = new Array<int32_t>();
                ArraySetLength<int32_t>(count, stations);
                for (uint32_t j = 0; j < stations->size; j++) {
                    AEFile::ReadSwitched(stations->data[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *wrecks;
            if ((int32_t)count < 1) {
                wrecks = 0;
            } else {
                wrecks = new Array<int32_t>();
                ArraySetLength<int32_t>(count, wrecks);
                for (uint32_t j = 0; j < wrecks->size; j++) {
                    AEFile::ReadSwitched(wrecks->data[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *statics;
            if ((int32_t)count < 1) {
                statics = 0;
            } else {
                statics = new Array<int32_t>();
                ArraySetLength<int32_t>(count, statics);
                for (uint32_t j = 0; j < statics->size; j++) {
                    AEFile::ReadSwitched(statics->data[j], handle);
                }
            }

            SolarSystem *system = new SolarSystem(i, name, faction, flag == 1, a, b, c, d, e, f,
                                                  routes, stations, wrecks, statics);
            systems->data[i] = system;
            delete[] routes;
        }
        AEFile::Close(handle);
    }
    return systems;
}
