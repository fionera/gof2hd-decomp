# Status::setLastXP
elf 0xac604 body 6
Sig: undefined __thiscall setLastXP(Status * this, int param_1)

## decompile
```c

/* Status::setLastXP(int) */

void __thiscall Status::setLastXP(Status *this,int param_1)

{
  *(int *)(this + 0x1cc) = param_1;
  return;
}

```
## target disasm
```
  000bc604: str.w r1,[r0,#0x1cc]
  000bc608: bx lr
```
