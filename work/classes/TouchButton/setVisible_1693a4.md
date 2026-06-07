# TouchButton::setVisible
elf 0x1693a4 body 6
Sig: undefined __thiscall setVisible(TouchButton * this, bool param_1)

## decompile
```c

/* TouchButton::setVisible(bool) */

void __thiscall TouchButton::setVisible(TouchButton *this,bool param_1)

{
  this[0xb2] = (TouchButton)param_1;
  return;
}

```

## target disasm
```
  001793a4: strb.w r1,[r0,#0xb2]
  001793a8: bx lr
```
