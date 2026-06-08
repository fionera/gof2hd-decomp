# Array<Node*>::~Array
elf 0x11ba80 body 22
Sig: undefined __thiscall ~Array(Array<Node*> * this)

## decompile
```c

/* Array<Node*>::~Array() */

Array<Node*> * __thiscall Array<Node*>::~Array(Array<Node*> *this)

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
  0012ba80: push {r4,r6,r7,lr}
  0012ba82: add r7,sp,#0x8
  0012ba84: mov r4,r0
  0012ba86: ldr r0,[r0,#0x4]
  0012ba88: cbz r0,0x0012ba8e
  0012ba8a: blx 0x0006ebfc
  0012ba8e: movs r0,#0x0
  0012ba90: str r0,[r4,#0x4]
  0012ba92: mov r0,r4
  0012ba94: pop {r4,r6,r7,pc}
```
