# Mission::setVisible
elf 0x15bab4 body 6
Sig: undefined __thiscall setVisible(Mission * this, bool param_1)

## decompile
```c

/* Mission::setVisible(bool) */

void __thiscall Mission::setVisible(Mission *this,bool param_1)

{
  this[0x74] = (Mission)param_1;
  return;
}

```

## target disasm
```
  0016bab4: strb.w r1,[r0,#0x74]
  0016bab8: bx lr
```
