# TouchButton::resetTouch
elf 0x1699c8 body 8
Sig: undefined __thiscall resetTouch(TouchButton * this)

## decompile
```c

/* TouchButton::resetTouch() */

void __thiscall TouchButton::resetTouch(TouchButton *this)

{
  this[0xb0] = (TouchButton)0x0;
  return;
}

```

## target disasm
```
  001799c8: movs r1,#0x0
  001799ca: strb.w r1,[r0,#0xb0]
  001799ce: bx lr
```
