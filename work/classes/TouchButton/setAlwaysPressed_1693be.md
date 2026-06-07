# TouchButton::setAlwaysPressed
elf 0x1693be body 6
Sig: undefined __thiscall setAlwaysPressed(TouchButton * this, bool param_1)

## decompile
```c

/* TouchButton::setAlwaysPressed(bool) */

void __thiscall TouchButton::setAlwaysPressed(TouchButton *this,bool param_1)

{
  this[0xb1] = (TouchButton)param_1;
  return;
}

```

## target disasm
```
  001793be: strb.w r1,[r0,#0xb1]
  001793c2: bx lr
```
