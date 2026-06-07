# TouchButton::~TouchButton
elf 0x169188 body 34
Sig: undefined __thiscall ~TouchButton(TouchButton * this)

## decompile
```c

/* TouchButton::~TouchButton() */

TouchButton * __thiscall TouchButton::~TouchButton(TouchButton *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_001791ac + 0x179194);
  (*pcVar1)(this + 0x2c);
  (*pcVar1)(this + 0x18);
  (*pcVar1)(this + 0xc);
  return this;
}

```

## target disasm
```
  00179188: push {r4,r5,r7,lr}
  0017918a: add r7,sp,#0x8
  0017918c: mov r4,r0
  0017918e: ldr r0,[0x001791ac]
  00179190: add r0,pc
  00179192: ldr r5,[r0,#0x0]
  00179194: add.w r0,r4,#0x2c
  00179198: blx r5
  0017919a: add.w r0,r4,#0x18
  0017919e: blx r5
  001791a0: add.w r0,r4,#0xc
  001791a4: blx r5
  001791a6: mov r0,r4
  001791a8: pop {r4,r5,r7,pc}
```
