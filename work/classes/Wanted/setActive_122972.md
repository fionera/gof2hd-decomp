# Wanted::setActive
elf 0x122972 body 6
Sig: undefined __thiscall setActive(Wanted * this, bool param_1)

## decompile
```c

/* Wanted::setActive(bool) */

void __thiscall Wanted::setActive(Wanted *this,bool param_1)

{
  this[0x51] = (Wanted)param_1;
  return;
}

```

## target disasm
```
  00132972: strb.w r1,[r0,#0x51]
  00132976: bx lr
```
