# TouchButton::setPressProgressHighlight
elf 0x1693d0 body 6
Sig: undefined __thiscall setPressProgressHighlight(TouchButton * this, bool param_1)

## decompile
```c

/* TouchButton::setPressProgressHighlight(bool) */

void __thiscall TouchButton::setPressProgressHighlight(TouchButton *this,bool param_1)

{
  this[0xb8] = (TouchButton)param_1;
  return;
}

```

## target disasm
```
  001793d0: strb.w r1,[r0,#0xb8]
  001793d4: bx lr
```
