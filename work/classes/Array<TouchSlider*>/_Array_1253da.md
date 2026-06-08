# Array<TouchSlider*>::~Array
elf 0x1253da body 22
Sig: undefined __thiscall ~Array(Array<TouchSlider*> * this)

## decompile
```c

/* Array<TouchSlider*>::~Array() */

Array<TouchSlider*> * __thiscall Array<TouchSlider*>::~Array(Array<TouchSlider*> *this)

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
  001353da: push {r4,r6,r7,lr}
  001353dc: add r7,sp,#0x8
  001353de: mov r4,r0
  001353e0: ldr r0,[r0,#0x4]
  001353e2: cbz r0,0x001353e8
  001353e4: blx 0x0006ebfc
  001353e8: movs r0,#0x0
  001353ea: str r0,[r4,#0x4]
  001353ec: mov r0,r4
  001353ee: pop {r4,r6,r7,pc}
```
