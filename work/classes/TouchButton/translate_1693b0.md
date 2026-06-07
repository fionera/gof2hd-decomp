# TouchButton::translate
elf 0x1693b0 body 14
Sig: undefined __thiscall translate(TouchButton * this, int param_1, int param_2)

## decompile
```c

/* TouchButton::translate(int, int) */

void __thiscall TouchButton::translate(TouchButton *this,int param_1,int param_2)

{
  *(int *)(this + 0x78) = param_1 + *(int *)(this + 0x78);
  *(int *)(this + 0x7c) = param_2 + *(int *)(this + 0x7c);
  return;
}

```

## target disasm
```
  001793b0: ldrd r3,r12,[r0,#0x78]
  001793b4: add r2,r12
  001793b6: add r1,r3
  001793b8: strd r1,r2,[r0,#0x78]
  001793bc: bx lr
```
