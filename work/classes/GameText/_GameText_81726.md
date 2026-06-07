# GameText::~GameText
elf 0x81726 body 42
Sig: undefined __thiscall ~GameText(GameText * this)

## decompile
```c

/* GameText::~GameText() */

void __thiscall GameText::~GameText(GameText *this)

{
  release(this);
  if (*(void **)(this + 0xc) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0xc));
  }
  *(undefined4 *)(this + 0xc) = 0;
  AbyssEngine::String::~String((String *)(this + 0x10));
  (*(code *)(DAT_001ab4c4 + 0x1ab4c8))(this);
  return;
}

```

## target disasm
```
  00091726: push {r4,r6,r7,lr}
  00091728: add r7,sp,#0x8
  0009172a: mov r4,r0
  0009172c: blx 0x0006ffe8
  00091730: ldr r0,[r4,#0xc]
  00091732: cbz r0,0x00091738
  00091734: blx 0x0006ebfc
  00091738: movs r0,#0x0
  0009173a: str r0,[r4,#0xc]
  0009173c: add.w r0,r4,#0x10
  00091740: blx 0x0006ee30
  00091744: mov r0,r4
  00091746: pop.w {r4,r6,r7,lr}
  0009174a: b.w 0x001ab4b8
  001ab4b8: bx pc
```
