#include "AMeshMerger.h"




// ---- render_1811e4.cpp ----
// AMeshMerger::render()
//   tail-call engine draw: drawMeshes(this->field_14, this->field_1c, 0).
struct AMeshMerger {
    // @portable-fields
    unsigned char _pad_0[20];
    int f_14; // 0x14
    unsigned char _pad_18[4];
    int f_1c; // 0x1c
 void render(); };
void AMeshMerger::render()
{
    AMeshMerger_drawMeshes(this->f_14, this->f_1c, 0);
}

// ---- _AMeshMerger_1811f0.cpp ----
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
