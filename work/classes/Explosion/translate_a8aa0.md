# Explosion::translate
elf 0xa8aa0 body 30
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* Explosion::translate(AbyssEngine::AEMath::Vector const&) */

void Explosion::translate(Vector *param_1)

{
  AEGeometry::translate(*(Vector **)(param_1 + 4));
  if (*(int *)(param_1 + 8) == 0) {
    return;
  }
  (*(code *)(DAT_001abef4 + 0x1abef8))();
  return;
}

```

## target disasm
```
  000b8aa0: push {r4,r5,r7,lr}
  000b8aa2: add r7,sp,#0x8
  000b8aa4: mov r5,r0
  000b8aa6: ldr r0,[r0,#0x4]
  000b8aa8: mov r4,r1
  000b8aaa: blx 0x00072460
  000b8aae: ldr r0,[r5,#0x8]
  000b8ab0: cbz r0,0x000b8abc
  000b8ab2: mov r1,r4
  000b8ab4: pop.w {r4,r5,r7,lr}
  000b8ab8: b.w 0x001abee8
  000b8abc: pop {r4,r5,r7,pc}
```
