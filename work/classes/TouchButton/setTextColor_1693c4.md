# TouchButton::setTextColor
elf 0x1693c4 body 6
Sig: undefined __thiscall setTextColor(TouchButton * this, int param_1)

## decompile
```c

/* TouchButton::setTextColor(int) */

void __thiscall TouchButton::setTextColor(TouchButton *this,int param_1)

{
  *(int *)(this + 0xac) = param_1;
  return;
}

```

## target disasm
```
  001793c4: str.w r1,[r0,#0xac]
  001793c8: bx lr
```
