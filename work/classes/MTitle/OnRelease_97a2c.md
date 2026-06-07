# MTitle::OnRelease
elf 0x97a2c body 76
Sig: undefined __stdcall OnRelease(void)

## decompile
```c

/* MTitle::OnRelease() */

void MTitle::OnRelease(void)

{
  int iVar1;
  int *piVar2;
  
  AbyssEngine::PaintCanvas::ReleaseAllResources();
  iVar1 = **(int **)(DAT_000a7a7c + 0xa7a42);
  GameText::getLanguage();
  Globals::loadFont(iVar1);
  piVar2 = *(int **)(DAT_000a7a80 + 0xa7a56);
  if (*piVar2 == 0) {
    return;
  }
  Layout::reload();
  ImageFactory::reload();
  (*(code *)(DAT_001ab8f4 + 0x1ab8f8))(*piVar2);
  return;
}

```

## target disasm
```
  000a7a2c: push {r4,r6,r7,lr}
  000a7a2e: add r7,sp,#0x8
  000a7a30: ldr r0,[0x000a7a78]
  000a7a32: add r0,pc
  000a7a34: ldr r0,[r0,#0x0]
  000a7a36: ldr r0,[r0,#0x0]
  000a7a38: blx 0x0006f964
  000a7a3c: ldr r0,[0x000a7a7c]
  000a7a3e: add r0,pc
  000a7a40: ldr r0,[r0,#0x0]
  000a7a42: ldr r4,[r0,#0x0]
  000a7a44: blx 0x0006f544
  000a7a48: mov r1,r0
  000a7a4a: mov r0,r4
  000a7a4c: blx 0x00071d04
  000a7a50: ldr r0,[0x000a7a80]
  000a7a52: add r0,pc
  000a7a54: ldr r4,[r0,#0x0]
  000a7a56: ldr r0,[r4,#0x0]
  000a7a58: cbz r0,0x000a7a74
  000a7a5a: blx 0x00071d10
  000a7a5e: ldr r0,[0x000a7a84]
  000a7a60: add r0,pc
  000a7a62: ldr r0,[r0,#0x0]
  000a7a64: ldr r0,[r0,#0x0]
  000a7a66: blx 0x00071d1c
  000a7a6a: ldr r0,[r4,#0x0]
  000a7a6c: pop.w {r4,r6,r7,lr}
  000a7a70: b.w 0x001ab8e8
  000a7a74: pop {r4,r6,r7,pc}
  001ab8e8: bx pc
```
