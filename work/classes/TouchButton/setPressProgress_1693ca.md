# TouchButton::setPressProgress
elf 0x1693ca body 6
Sig: undefined __thiscall setPressProgress(TouchButton * this, float param_1)

## decompile
```c

/* TouchButton::setPressProgress(float) */

void __thiscall TouchButton::setPressProgress(TouchButton *this,float param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(this + 0xbc) = in_r1;
  return;
}

```

## target disasm
```
  001793ca: str.w r1,[r0,#0xbc]
  001793ce: bx lr
```
