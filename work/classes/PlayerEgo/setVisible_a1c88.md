# PlayerEgo::setVisible
elf 0xa1c88 body 14
Sig: undefined __thiscall setVisible(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setVisible(bool) */

void __thiscall PlayerEgo::setVisible(PlayerEgo *this,bool param_1)

{
  *(bool *)((int)&this[0x15].field_14 + 1) = param_1;
  *(bool *)((int)&this[0x16].field_14 + 2) = param_1;
  (*(code *)(DAT_001abbe4 + 0x1abbe8))();
  return;
}

```

## target disasm
```
  000b1c88: strb.w r1,[r0,#0x309]
  000b1c8c: strb.w r1,[r0,#0x32e]
  000b1c90: b.w 0x001abbd8
  001abbd8: bx pc
```
