# Mission::setInstantActionMission
elf 0x15bb72 body 6
Sig: undefined __thiscall setInstantActionMission(Mission * this, bool param_1)

## decompile
```c

/* Mission::setInstantActionMission(bool) */

void __thiscall Mission::setInstantActionMission(Mission *this,bool param_1)

{
  this[0x5c] = (Mission)param_1;
  return;
}

```

## target disasm
```
  0016bb72: strb.w r1,[r0,#0x5c]
  0016bb76: bx lr
```
