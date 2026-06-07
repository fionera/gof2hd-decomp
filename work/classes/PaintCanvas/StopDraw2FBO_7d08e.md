# PaintCanvas::StopDraw2FBO
elf 0x7d08e body 38
Sig: undefined __thiscall StopDraw2FBO(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::StopDraw2FBO() */

void __thiscall AbyssEngine::PaintCanvas::StopDraw2FBO(PaintCanvas *this)

{
  Engine::DeactivateRender2TextureFBO(*(Engine **)(this + 0x34));
  SetBlendMode(this,0);
  Engine::DoPostEffect();
  (*(code *)(DAT_001ab404 + 0x1ab408))(*(undefined4 *)(this + 0x34));
  return;
}

```

## target disasm
```
  0008d08e: push {r4,r6,r7,lr}
  0008d090: add r7,sp,#0x8
  0008d092: mov r4,r0
  0008d094: ldr r0,[r0,#0x34]
  0008d096: blx 0x0006ee84
  0008d09a: mov r0,r4
  0008d09c: movs r1,#0x0
  0008d09e: blx 0x0006febc
  0008d0a2: ldr r0,[r4,#0x34]
  0008d0a4: blx 0x0006fec8
  0008d0a8: ldr r0,[r4,#0x34]
  0008d0aa: pop.w {r4,r6,r7,lr}
  0008d0ae: b.w 0x001ab3f8
  001ab3f8: bx pc
```
