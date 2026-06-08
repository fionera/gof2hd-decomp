# Array<Waypoint*>::~Array
elf 0x11bd7e body 22
Sig: undefined __thiscall ~Array(Array<Waypoint*> * this)

## decompile
```c

/* Array<Waypoint*>::~Array() */

Array<Waypoint*> * __thiscall Array<Waypoint*>::~Array(Array<Waypoint*> *this)

{
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  0012bd7e: push {r4,r6,r7,lr}
  0012bd80: add r7,sp,#0x8
  0012bd82: mov r4,r0
  0012bd84: ldr r0,[r0,#0x4]
  0012bd86: cbz r0,0x0012bd8c
  0012bd88: blx 0x0006ebfc
  0012bd8c: movs r0,#0x0
  0012bd8e: str r0,[r4,#0x4]
  0012bd90: mov r0,r4
  0012bd92: pop {r4,r6,r7,pc}
```
