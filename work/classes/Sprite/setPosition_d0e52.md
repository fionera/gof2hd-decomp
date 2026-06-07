# Sprite::setPosition
elf 0xd0e52 body 6
Sig: undefined __thiscall setPosition(Sprite * this, int param_1, int param_2)

## decompile
```c

/* Sprite::setPosition(int, int) */

void __thiscall Sprite::setPosition(Sprite *this,int param_1,int param_2)

{
  *(int *)(this + 0x10) = param_1;
  *(int *)(this + 0x14) = param_2;
  return;
}

```

## target disasm
```
  000e0e52: strd r1,r2,[r0,#0x10]
  000e0e56: bx lr
```
