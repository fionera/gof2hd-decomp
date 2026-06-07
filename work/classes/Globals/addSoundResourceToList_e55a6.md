# Globals::addSoundResourceToList
elf 0xe55a6 body 38
Sig: undefined __stdcall addSoundResourceToList(int param_1)

## decompile
```c

/* Globals::addSoundResourceToList(int) */

void Globals::addSoundResourceToList(int param_1)

{
  int iVar1;
  int in_r1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(param_1 + 4);
  if (puVar2 != (uint *)0x0) {
    uVar3 = 0;
    do {
      if (*puVar2 <= uVar3) {
        (*(code *)(DAT_001ac1b4 + 0x1ac1b8))();
        return;
      }
      iVar1 = uVar3 * 4;
      uVar3 = uVar3 + 1;
    } while (*(int *)(puVar2[1] + iVar1) != in_r1);
  }
  return;
}

```

## target disasm
```
  000f55a6: ldr r2,[r0,#0x4]
  000f55a8: cbz r2,0x000f55c0
  000f55aa: ldr.w r12,[r2,#0x0]
  000f55ae: movs r3,#0x0
  000f55b0: cmp r3,r12
  000f55b2: bcs 0x000f55c2
  000f55b4: ldr r0,[r2,#0x4]
  000f55b6: ldr.w r0,[r0,r3,lsl #0x2]
  000f55ba: adds r3,#0x1
  000f55bc: cmp r0,r1
  000f55be: bne 0x000f55b0
  000f55c0: bx lr
  000f55c2: mov r0,r1
  000f55c4: mov r1,r2
  000f55c6: b.w 0x001ac1a8
  001ac1a8: bx pc
```
