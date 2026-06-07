#include "class.h"

__attribute__((minsize))
Array<Agent *> *FileRead::loadAgents()
{
    uint32_t exists;
    {
        String path("agents_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<Agent *> *agents = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("agents_binary.bin", &handle);

        agents = new Array<Agent *>();
        ArraySetLength<Agent *>(0x1b, agents);

        String name;
        int32_t f = -1;
        uint32_t index = 0;
        while (index < agents->size) {
            int32_t id;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t flag;
            int32_t d;
            int32_t e;
            int32_t g;
            int32_t h;
            int32_t imageCount;

            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(id, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(flag, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);

            Agent *agent = new Agent(id, name, a, b, c, flag == 1, d, e, f, g);
            agents->data[index] = agent;

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index++;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                for (uint32_t i = 0; i < 5; i++) {
                    char part;
                    AEFile::Read(part, handle);
                    parts[i] = part;
                }
                agents->data[oldIndex]->setImageParts(parts);
            }
        }
        AEFile::Close(handle);
    }
    return agents;
}
