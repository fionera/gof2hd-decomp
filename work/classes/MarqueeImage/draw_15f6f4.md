# MarqueeImage::draw
elf 0x15f6f4 body 10
Sig: undefined __thiscall draw(MarqueeImage * this)

## decompile
```c

/* MarqueeImage::draw() */

void __thiscall MarqueeImage::draw(MarqueeImage *this)

{
  (*(code *)(DAT_001ac624 + 0x1ac628))
            (this,*(undefined4 *)(this + 0xc),*(undefined4 *)(this + 0x10));
  return;
}

```

## target disasm
```
  0016f6f4: ldrd r1,r2,[r0,#0xc]
  0016f6f8: b.w 0x001ac618
  001ac618: bx pc
```
