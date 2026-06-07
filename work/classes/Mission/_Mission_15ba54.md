# Mission::~Mission
elf 0x15ba54 body 60
Sig: undefined __thiscall ~Mission(Mission * this)

## decompile
```c

/* Mission::~Mission() */

Mission * __thiscall Mission::~Mission(Mission *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_0016ba94 + 0x16ba6a);
  *(int *)this = *(int *)(DAT_0016ba90 + 0x16ba68) + 8;
  (*pcVar1)(this + 0x4c);
  (*pcVar1)(this + 0x40);
  (*pcVar1)(this + 0x1c);
  (*pcVar1)(this + 0x10);
  return this;
}

```

## target disasm
```
  0016ba54: push {r4,r5,r6,r7,lr}
  0016ba56: add r7,sp,#0xc
  0016ba58: push.w r11
  0016ba5c: mov r4,r0
  0016ba5e: ldr r0,[0x0016ba90]
  0016ba60: ldr r1,[0x0016ba94]
  0016ba62: mov r5,r4
  0016ba64: add r0,pc
  0016ba66: add r1,pc
  0016ba68: ldr r0,[r0,#0x0]
  0016ba6a: ldr r6,[r1,#0x0]
  0016ba6c: adds r0,#0x8
  0016ba6e: str.w r0,[r5],#0x10
  0016ba72: add.w r0,r4,#0x4c
  0016ba76: blx r6
  0016ba78: add.w r0,r4,#0x40
  0016ba7c: blx r6
  0016ba7e: add.w r0,r4,#0x1c
  0016ba82: blx r6
  0016ba84: mov r0,r5
  0016ba86: blx r6
  0016ba88: mov r0,r4
  0016ba8a: pop.w r11
  0016ba8e: pop {r4,r5,r6,r7,pc}
```
