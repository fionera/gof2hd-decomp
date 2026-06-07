# PlayerEgo::setComputerControlled
elf 0x9b7fa body 6
Sig: undefined __thiscall setComputerControlled(PlayerEgo * this, bool param_1)

## decompile
```c

/* PlayerEgo::setComputerControlled(bool) */

void __thiscall PlayerEgo::setComputerControlled(PlayerEgo *this,bool param_1)

{
  *(bool *)&this[0xd].field_20 = param_1;
  return;
}

```

## target disasm
```
  000ab7fa: strb.w r1,[r0,#0x1f4]
  000ab7fe: bx lr
```
