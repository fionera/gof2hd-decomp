# Sprite::setRefPixelPosition
elf 0xd0e4c body 6
Sig: undefined __thiscall setRefPixelPosition(Sprite * this, int param_1, int param_2)

## decompile
```c

/* Sprite::setRefPixelPosition(int, int) */

void __thiscall Sprite::setRefPixelPosition(Sprite *this,int param_1,int param_2)

{
  *(int *)(this + 0x10) = param_1;
  *(int *)(this + 0x14) = param_2;
  return;
}

```

## target disasm
```
  000e0e4c: strd r1,r2,[r0,#0x10]
  000e0e50: bx lr
```
