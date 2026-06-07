# AEGeometry::DEBUG_setMeshMergerIndex
elf 0xa4f9e body 6
Sig: undefined __thiscall DEBUG_setMeshMergerIndex(AEGeometry * this, int param_1, LodMeshMerger * param_2)

## decompile
```c

/* AEGeometry::DEBUG_setMeshMergerIndex(int, LodMeshMerger*) */

void __thiscall
AEGeometry::DEBUG_setMeshMergerIndex(AEGeometry *this,int param_1,LodMeshMerger *param_2)

{
  *(int *)this = param_1;
  *(LodMeshMerger **)(this + 4) = param_2;
  return;
}

```

## target disasm
```
  000b4f9e: strd r1,r2,[r0,#0x0]
  000b4fa2: bx lr
```
