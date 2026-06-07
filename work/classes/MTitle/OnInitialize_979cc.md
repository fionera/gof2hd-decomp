# MTitle::OnInitialize
elf 0x979cc body 76
Sig: undefined __thiscall OnInitialize(MTitle * this)

## decompile
```c

/* MTitle::OnInitialize() */

undefined4 __thiscall MTitle::OnInitialize(MTitle *this)

{
  undefined4 *puVar1;
  float fVar2;
  
  puVar1 = *(undefined4 **)(DAT_000a7a18 + 0xa79e0);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar1,7000,(uint *)(this + 0x10));
  fVar2 = (float)AbyssEngine::PaintCanvas::Image2DCreate
                           ((PaintCanvas *)*puVar1,0x1b59,(uint *)(this + 0x14));
  FModSound::play(**(int **)(DAT_000a7a1c + 0xa7a00),(Vector *)0x91,(Vector *)0x0,fVar2);
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0xc) = 100;
  return 0;
}

```

## target disasm
```
  000a79cc: push {r2,r3,r4,r5,r7,lr}
  000a79ce: add r7,sp,#0x10
  000a79d0: mov r4,r0
  000a79d2: ldr r0,[0x000a7a18]
  000a79d4: add.w r2,r4,#0x10
  000a79d8: movw r1,#0x1b58
  000a79dc: add r0,pc
  000a79de: ldr r5,[r0,#0x0]
  000a79e0: ldr r0,[r5,#0x0]
  000a79e2: blx 0x00071cf8
  000a79e6: ldr r0,[r5,#0x0]
  000a79e8: add.w r2,r4,#0x14
  000a79ec: movw r1,#0x1b59
  000a79f0: blx 0x00071cf8
  000a79f4: ldr r0,[0x000a7a1c]
  000a79f6: movs r5,#0x0
  000a79f8: movs r1,#0x91
  000a79fa: movs r2,#0x0
  000a79fc: add r0,pc
  000a79fe: movs r3,#0x0
  000a7a00: str r5,[sp,#0x0]
  000a7a02: ldr r0,[r0,#0x0]
  000a7a04: ldr r0,[r0,#0x0]
  000a7a06: blx 0x00071548
  000a7a0a: movs r0,#0x64
  000a7a0c: strd r5,r5,[r4,#0x18]
  000a7a10: str r0,[r4,#0xc]
  000a7a12: movs r0,#0x0
  000a7a14: add sp,#0x8
  000a7a16: pop {r4,r5,r7,pc}
```
