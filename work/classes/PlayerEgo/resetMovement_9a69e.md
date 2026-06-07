# PlayerEgo::resetMovement
elf 0x9a69e body 20
Sig: undefined __thiscall resetMovement(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::resetMovement() */

void __thiscall PlayerEgo::resetMovement(PlayerEgo *this)

{
  this[0x11].field_14 = (int *)0x0;
  this[0x11].field_18 = (int *)0x0;
  this[0x11].field_4 = (int *)0x0;
  this[0x11].field_8 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  this[0x11].m_pLevel = *(SingleLevel **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  this[0x11].field_10 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return;
}

```

## target disasm
```
  000aa69e: movs r1,#0x0
  000aa6a0: vmov.i32 q8,#0x0
  000aa6a4: strd r1,r1,[r0,#0x278]
  000aa6a8: add.w r0,r0,#0x268
  000aa6ac: vst1.64 {d16,d17},[r0]
  000aa6b0: bx lr
```
