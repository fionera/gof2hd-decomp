#include "class.h"

// AMeshMerger::~AMeshMerger()
//   Array<AbyssEngine::Mesh*>::~Array(this + 0x08);
struct AMeshMerger {
    // @portable-fields
    unsigned char _pad_0[20];
    int f_14; // 0x14
    unsigned char _pad_18[4];
    int f_1c; // 0x1c
 ~AMeshMerger(); };
AMeshMerger::~AMeshMerger()
{
    AMeshMerger_ArrayMesh_dtor((char *)this + 0x08);
}
