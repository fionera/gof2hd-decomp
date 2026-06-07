# TouchSlider::setHalfTransparent
elf 0xa2426 body 6
Sig: undefined __thiscall setHalfTransparent(TouchSlider * this, bool param_1)

## decompile
```c

/* TouchSlider::setHalfTransparent(bool) */

void __thiscall TouchSlider::setHalfTransparent(TouchSlider *this,bool param_1)

{
  this[0x35] = (TouchSlider)param_1;
  return;
}

```

## target disasm
```
  000b2426: strb.w r1,[r0,#0x35]
  000b242a: bx lr
```
