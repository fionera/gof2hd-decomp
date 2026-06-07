# PlayerStation::getProjectionVector
elf 0x122858 body 36
Sig: undefined __thiscall getProjectionVector(PlayerStation * this, Vector * param_1)

## decompile
```c

/* PlayerStation::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerStation::getProjectionVector(PlayerStation *this,Vector *param_1)

{
  if (*(int *)(param_1 + 0x130) != 0) {
    BoundingVolume::getProjectionVector((Vector *)this);
    return;
  }
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return;
}

```

## target disasm
```
  00132858: ldr.w r3,[r1,#0x130]
  0013285c: cbz r3,0x00132872
  0013285e: push {r7,lr}
  00132860: mov r7,sp
  00132862: ldr.w r1,[r1,#0x150]
  00132866: ldr r3,[r3,#0x4]
  00132868: ldr.w r1,[r3,r1,lsl #0x2]
  0013286c: blx 0x00075fe8
  00132870: pop {r7,pc}
  00132872: movs r1,#0x0
  00132874: strd r1,r1,[r0,#0x0]
  00132878: str r1,[r0,#0x8]
  0013287a: bx lr
```
