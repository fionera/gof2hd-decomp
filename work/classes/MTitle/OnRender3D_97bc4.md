# MTitle::OnRender3D
elf 0x97bc4 body 36
Sig: undefined __thiscall OnRender3D(MTitle * this)

## decompile
```c

/* MTitle::OnRender3D() */

void __thiscall MTitle::OnRender3D(MTitle *this)

{
  AbyssEngine::PaintCanvas::ClearBuffer(**(uint **)(DAT_000a7be8 + 0xa7bd2));
  AbyssEngine::PaintCanvas::Begin3d();
  (*(code *)(DAT_001ab924 + 0x1ab928))(*(undefined4 *)(this + 4));
  return;
}

```

## target disasm
```
  000a7bc4: push {r4,r6,r7,lr}
  000a7bc6: add r7,sp,#0x8
  000a7bc8: mov r4,r0
  000a7bca: ldr r0,[0x000a7be8]
  000a7bcc: movs r1,#0xff
  000a7bce: add r0,pc
  000a7bd0: ldr r0,[r0,#0x0]
  000a7bd2: ldr r0,[r0,#0x0]
  000a7bd4: blx 0x00071d94
  000a7bd8: ldr r0,[r4,#0x4]
  000a7bda: blx 0x00071da0
  000a7bde: ldr r0,[r4,#0x4]
  000a7be0: pop.w {r4,r6,r7,lr}
  000a7be4: b.w 0x001ab918
```
