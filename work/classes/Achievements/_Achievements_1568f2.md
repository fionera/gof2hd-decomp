# Achievements::~Achievements
elf 0x1568f2 body 32
Sig: undefined __thiscall ~Achievements(Achievements * this)

## decompile
```c

/* Achievements::~Achievements() */

Achievements * __thiscall Achievements::~Achievements(Achievements *this)

{
  if (*(void **)this != (void *)0x0) {
    operator_delete__(*(void **)this);
  }
  *(undefined4 *)this = 0;
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  001668f2: push {r4,r5,r7,lr}
  001668f4: add r7,sp,#0x8
  001668f6: mov r4,r0
  001668f8: ldr r0,[r0,#0x0]
  001668fa: cbz r0,0x00166900
  001668fc: blx 0x0006ebfc
  00166900: ldr r0,[r4,#0x4]
  00166902: movs r5,#0x0
  00166904: str r5,[r4,#0x0]
  00166906: cbz r0,0x0016690c
  00166908: blx 0x0006ebfc
  0016690c: mov r0,r4
  0016690e: str r5,[r4,#0x4]
  00166910: pop {r4,r5,r7,pc}
```
