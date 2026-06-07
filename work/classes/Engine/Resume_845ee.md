# Engine::Resume
elf 0x845ee body 40
Sig: undefined __thiscall Resume(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::Resume() */

undefined4 __thiscall AbyssEngine::Engine::Resume(Engine *this)

{
  int iVar1;
  
  PaintCanvas::Resume((PaintCanvas *)**(undefined4 **)(this + 0x30));
  for (iVar1 = 0; iVar1 != 0x14; iVar1 = iVar1 + 1) {
    *(undefined4 *)(this + iVar1 * 4 + 0x7c) = 0xffffffff;
  }
  return 1;
}

```

## target disasm
```
  000945ee: push {r4,r6,r7,lr}
  000945f0: add r7,sp,#0x8
  000945f2: mov r4,r0
  000945f4: ldr r0,[r0,#0x30]
  000945f6: ldr r0,[r0,#0x0]
  000945f8: blx 0x0007054c
  000945fc: add.w r0,r4,#0x7c
  00094600: movs r1,#0x0
  00094602: mov.w r2,#0xffffffff
  00094606: b 0x0009460e
  00094608: str.w r2,[r0,r1,lsl #0x2]
  0009460c: adds r1,#0x1
  0009460e: cmp r1,#0x14
  00094610: bne 0x00094608
  00094612: movs r0,#0x1
  00094614: pop {r4,r6,r7,pc}
```
