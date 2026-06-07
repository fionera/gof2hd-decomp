# ScrollTouchBox::setPosition
elf 0x13576c body 6
Sig: undefined __thiscall setPosition(ScrollTouchBox * this, int param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::setPosition(int, int) */

void __thiscall ScrollTouchBox::setPosition(ScrollTouchBox *this,int param_1,int param_2)

{
  *(int *)(this + 4) = param_1;
  *(int *)(this + 8) = param_2;
  return;
}

```

## target disasm
```
  0014576c: strd r1,r2,[r0,#0x4]
  00145770: bx lr
```
