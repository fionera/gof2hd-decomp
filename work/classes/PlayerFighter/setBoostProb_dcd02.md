# PlayerFighter::setBoostProb
elf 0xdcd02 body 6
Sig: undefined __thiscall setBoostProb(PlayerFighter * this, int param_1)

## decompile
```c

/* PlayerFighter::setBoostProb(int) */

void __thiscall PlayerFighter::setBoostProb(PlayerFighter *this,int param_1)

{
  *(int *)(this + 0x1b4) = param_1;
  return;
}

```

## target disasm
```
  000ecd02: str.w r1,[r0,#0x1b4]
  000ecd06: bx lr
```
