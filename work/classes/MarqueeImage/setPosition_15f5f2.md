# MarqueeImage::setPosition
elf 0x15f5f2 body 6
Sig: undefined __thiscall setPosition(MarqueeImage * this, int param_1, int param_2)

## decompile
```c

/* MarqueeImage::setPosition(int, int) */

void __thiscall MarqueeImage::setPosition(MarqueeImage *this,int param_1,int param_2)

{
  *(int *)(this + 0xc) = param_1;
  *(int *)(this + 0x10) = param_2;
  return;
}

```

## target disasm
```
  0016f5f2: strd r1,r2,[r0,#0xc]
  0016f5f6: bx lr
```
