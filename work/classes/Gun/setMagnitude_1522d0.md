# Gun::setMagnitude
elf 0x1522d0 body 6
Sig: undefined __thiscall setMagnitude(Gun * this, int param_1)

## decompile
```c

/* Gun::setMagnitude(int) */

void __thiscall Gun::setMagnitude(Gun *this,int param_1)

{
  *(int *)(this + 0x100) = param_1;
  return;
}

```

## target disasm
```
  001622d0: str.w r1,[r0,#0x100]
  001622d4: bx lr
```
