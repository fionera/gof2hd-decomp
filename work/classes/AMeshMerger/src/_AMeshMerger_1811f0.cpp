#include "class.h"

// AMeshMerger::~AMeshMerger()
//   Array<AbyssEngine::Mesh*>::~Array(this + 0x08);
struct AMeshMerger { ~AMeshMerger(); };
AMeshMerger::~AMeshMerger()
{
    AMeshMerger_ArrayMesh_dtor((char *)this + 0x08);
}
