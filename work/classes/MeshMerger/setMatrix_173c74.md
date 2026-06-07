# MeshMerger::setMatrix
elf 0x173c74 body 16
Sig: undefined __thiscall setMatrix(MeshMerger * this, int param_1, Matrix * param_2)

## decompile
```c

/* MeshMerger::setMatrix(int, AbyssEngine::AEMath::Matrix const&) */

void __thiscall MeshMerger::setMatrix(MeshMerger *this,int param_1,Matrix *param_2)

{
  (*(code *)(DAT_001abdb4 + 0x1abdb8))(*(int *)(this + 0x1c) + param_1 * 0x3c,param_2);
  return;
}

```

## target disasm
```
  00183c74: ldr r0,[r0,#0x1c]
  00183c76: rsb r1,r1,r1, lsl #0x4
  00183c7a: add.w r0,r0,r1, lsl #0x2
  00183c7e: mov r1,r2
  00183c80: b.w 0x001abda8
```
