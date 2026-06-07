# LodMeshMerger::setMatrix
elf 0x181428 body 18
Sig: undefined __thiscall setMatrix(LodMeshMerger * this, int param_1, Matrix * param_2)

## decompile
```c

/* LodMeshMerger::setMatrix(int, AbyssEngine::AEMath::Matrix const&) */

void __thiscall LodMeshMerger::setMatrix(LodMeshMerger *this,int param_1,Matrix *param_2)

{
  (*(code *)(DAT_001abdb4 + 0x1abdb8))(*(int *)(this + 0x28) + param_1 * 0x3c,param_2);
  return;
}

```

## target disasm
```
  00191428: ldr r0,[r0,#0x28]
  0019142a: rsb r1,r1,r1, lsl #0x4
  0019142e: add.w r0,r0,r1, lsl #0x2
  00191432: mov r1,r2
  00191434: b.w 0x001abda8
  001abda8: bx pc
```
