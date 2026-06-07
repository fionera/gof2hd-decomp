# Trail::~Trail
elf 0x15bf8c body 32
Sig: undefined __thiscall ~Trail(Trail * this)

## decompile
```c

/* Trail::~Trail() */

Trail * __thiscall Trail::~Trail(Trail *this)

{
  if (*(void **)(this + 0x18) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x18));
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x1c));
  }
  *(undefined4 *)(this + 0x1c) = 0;
  return this;
}

```

## target disasm
```
  0016bf8c: push {r4,r5,r7,lr}
  0016bf8e: add r7,sp,#0x8
  0016bf90: mov r4,r0
  0016bf92: ldr r0,[r0,#0x18]
  0016bf94: cbz r0,0x0016bf9a
  0016bf96: blx 0x0006ebfc
  0016bf9a: ldr r0,[r4,#0x1c]
  0016bf9c: movs r5,#0x0
  0016bf9e: str r5,[r4,#0x18]
  0016bfa0: cbz r0,0x0016bfa6
  0016bfa2: blx 0x0006ebfc
  0016bfa6: mov r0,r4
  0016bfa8: str r5,[r4,#0x1c]
  0016bfaa: pop {r4,r5,r7,pc}
```
