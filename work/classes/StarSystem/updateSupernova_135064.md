# StarSystem::updateSupernova
elf 0x135064 body 74
Sig: undefined __thiscall updateSupernova(StarSystem * this, int param_1)

## decompile
```c

/* StarSystem::updateSupernova(int) */

void __thiscall StarSystem::updateSupernova(StarSystem *this,int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  
  if (*(int *)(this + 0x40) != 0) {
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_001450b0 + 0x145076));
    AbyssEngine::Transform::Update((ulonglong)uVar1,SUB41(param_1,0));
  }
  if (**(int **)(*(int *)(this + 0x1c) + 4) != 0) {
    uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_001450b4 + 0x145098));
    AbyssEngine::Transform::Update(CONCAT44(1,uVar2),SUB41(param_1,0));
  }
  return;
}

```

## target disasm
```
  00145064: push {r2,r3,r4,r5,r7,lr}
  00145066: add r7,sp,#0x10
  00145068: mov r5,r0
  0014506a: ldr r0,[r0,#0x40]
  0014506c: mov r4,r1
  0014506e: cbz r0,0x0014508a
  00145070: ldr r1,[0x001450b0]
  00145072: add r1,pc
  00145074: ldr r2,[r1,#0x0]
  00145076: ldr r1,[r0,#0xc]
  00145078: ldr r0,[r2,#0x0]
  0014507a: blx 0x00072088
  0014507e: movs r1,#0x0
  00145080: asrs r3,r4,#0x1f
  00145082: mov r2,r4
  00145084: str r1,[sp,#0x0]
  00145086: blx 0x0006f7cc
  0014508a: ldr r0,[r5,#0x1c]
  0014508c: ldr r0,[r0,#0x4]
  0014508e: ldr r0,[r0,#0x0]
  00145090: cbz r0,0x001450ac
  00145092: ldr r1,[0x001450b4]
  00145094: add r1,pc
  00145096: ldr r2,[r1,#0x0]
  00145098: ldr r1,[r0,#0xc]
  0014509a: ldr r0,[r2,#0x0]
  0014509c: blx 0x00072088
  001450a0: movs r1,#0x1
  001450a2: asrs r3,r4,#0x1f
  001450a4: mov r2,r4
  001450a6: str r1,[sp,#0x0]
  001450a8: blx 0x0006f7cc
  001450ac: pop {r2,r3,r4,r5,r7,pc}
```
