# PlayerJumpgate::addJumpAnimationHandle
elf 0xa5110 body 6
Sig: undefined __thiscall addJumpAnimationHandle(PlayerJumpgate * this, uint param_1)

## decompile
```c

/* PlayerJumpgate::addJumpAnimationHandle(unsigned int) */

void __thiscall PlayerJumpgate::addJumpAnimationHandle(PlayerJumpgate *this,uint param_1)

{
  *(uint *)(this + 0x144) = param_1;
  return;
}

```

## target disasm
```
  000b5110: str.w r1,[r0,#0x144]
  000b5114: bx lr
```
