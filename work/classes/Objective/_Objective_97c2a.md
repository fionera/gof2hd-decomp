# Objective::~Objective
elf 0x97c2a body 46
Sig: undefined __thiscall ~Objective(Objective * this)

## decompile
```c

/* Objective::~Objective() */

Objective * __thiscall Objective::~Objective(Objective *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x10) != (Array *)0x0) {
    ArrayReleaseClasses<Objective*>(*(Array **)(this + 0x10));
    if (*(Array<Objective*> **)(this + 0x10) != (Array<Objective*> *)0x0) {
      pvVar1 = (void *)Array<Objective*>::~Array(*(Array<Objective*> **)(this + 0x10));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x10) = 0;
  if (*(int **)(this + 0x14) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x14) + 4))();
  }
  *(undefined4 *)(this + 0x14) = 0;
  return this;
}

```

## target disasm
```
  000a7c2a: push {r4,r5,r7,lr}
  000a7c2c: add r7,sp,#0x8
  000a7c2e: mov r4,r0
  000a7c30: ldr r0,[r0,#0x10]
  000a7c32: cbz r0,0x000a7c44
  000a7c34: blx 0x00071e78
  000a7c38: ldr r0,[r4,#0x10]
  000a7c3a: cbz r0,0x000a7c44
  000a7c3c: blx 0x00071e84
  000a7c40: blx 0x0006eb48
  000a7c44: ldr r0,[r4,#0x14]
  000a7c46: movs r5,#0x0
  000a7c48: str r5,[r4,#0x10]
  000a7c4a: cbz r0,0x000a7c52
  000a7c4c: ldr r1,[r0,#0x0]
  000a7c4e: ldr r1,[r1,#0x4]
  000a7c50: blx r1
  000a7c52: mov r0,r4
  000a7c54: str r5,[r4,#0x14]
  000a7c56: pop {r4,r5,r7,pc}
```
