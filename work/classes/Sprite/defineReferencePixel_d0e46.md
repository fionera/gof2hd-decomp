# Sprite::defineReferencePixel
elf 0xd0e46 body 6
Sig: undefined __thiscall defineReferencePixel(Sprite * this, int param_1, int param_2)

## decompile
```c

/* Sprite::defineReferencePixel(int, int) */

void __thiscall Sprite::defineReferencePixel(Sprite *this,int param_1,int param_2)

{
  *(int *)(this + 8) = param_1;
  *(int *)(this + 0xc) = param_2;
  return;
}

```

## target disasm
```
  000e0e46: strd r1,r2,[r0,#0x8]
  000e0e4a: bx lr
```
