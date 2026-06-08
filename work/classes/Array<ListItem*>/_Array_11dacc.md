# Array<ListItem*>::~Array
elf 0x11dacc body 22
Sig: undefined __thiscall ~Array(Array<ListItem*> * this)

## decompile
```c

/* Array<ListItem*>::~Array() */

Array<ListItem*> * __thiscall Array<ListItem*>::~Array(Array<ListItem*> *this)

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
  0012dacc: push {r4,r6,r7,lr}
  0012dace: add r7,sp,#0x8
  0012dad0: mov r4,r0
  0012dad2: ldr r0,[r0,#0x4]
  0012dad4: cbz r0,0x0012dada
  0012dad6: blx 0x0006ebfc
  0012dada: movs r0,#0x0
  0012dadc: str r0,[r4,#0x4]
  0012dade: mov r0,r4
  0012dae0: pop {r4,r6,r7,pc}
```
