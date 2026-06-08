# Array<Array<ListItem*>*>::~Array
elf 0x11db32 body 22
Sig: undefined __thiscall ~Array(Array<Array<ListItem*>*> * this)

## decompile
```c

/* Array<Array<ListItem*>*>::~Array() */

Array<Array<ListItem*>*> * __thiscall
Array<Array<ListItem*>*>::~Array(Array<Array<ListItem*>*> *this)

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
  0012db32: push {r4,r6,r7,lr}
  0012db34: add r7,sp,#0x8
  0012db36: mov r4,r0
  0012db38: ldr r0,[r0,#0x4]
  0012db3a: cbz r0,0x0012db40
  0012db3c: blx 0x0006ebfc
  0012db40: movs r0,#0x0
  0012db42: str r0,[r4,#0x4]
  0012db44: mov r0,r4
  0012db46: pop {r4,r6,r7,pc}
```
