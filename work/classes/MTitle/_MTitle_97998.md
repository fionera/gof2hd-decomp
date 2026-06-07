# MTitle::~MTitle
elf 0x97998 body 26
Sig: undefined __thiscall ~MTitle(MTitle * this)

## decompile
```c

/* MTitle::~MTitle() */

MTitle * __thiscall MTitle::~MTitle(MTitle *this)

{
  *(int *)this = *(int *)(DAT_000a79b8 + 0xa79a4) + 8;
  OnRelease();
  return this;
}

```

## target disasm
```
  000a7998: push {r4,r6,r7,lr}
  000a799a: add r7,sp,#0x8
  000a799c: mov r4,r0
  000a799e: ldr r0,[0x000a79b8]
  000a79a0: add r0,pc
  000a79a2: ldr r0,[r0,#0x0]
  000a79a4: adds r0,#0x8
  000a79a6: str r0,[r4,#0x0]
  000a79a8: mov r0,r4
  000a79aa: blx 0x00071ce0
  000a79ae: mov r0,r4
  000a79b0: pop {r4,r6,r7,pc}
```
