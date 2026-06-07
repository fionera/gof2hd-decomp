# ScrollTouchBox::setTextCentered
elf 0x135910 body 6
Sig: undefined __thiscall setTextCentered(ScrollTouchBox * this, bool param_1)

## decompile
```c

/* ScrollTouchBox::setTextCentered(bool) */

void __thiscall ScrollTouchBox::setTextCentered(ScrollTouchBox *this,bool param_1)

{
  this[0x38] = (ScrollTouchBox)param_1;
  return;
}

```

## target disasm
```
  00145910: strb.w r1,[r0,#0x38]
  00145914: bx lr
```
