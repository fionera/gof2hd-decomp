# Vector>::~Array
elf 0x134da8 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::AEMath::Vector> * this)

## decompile
```c

/* Array<AbyssEngine::AEMath::Vector>::~Array() */

Array<AbyssEngine::AEMath::Vector> * __thiscall
Array<AbyssEngine::AEMath::Vector>::~Array(Array<AbyssEngine::AEMath::Vector> *this)

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
  00144da8: push {r4,r6,r7,lr}
  00144daa: add r7,sp,#0x8
  00144dac: mov r4,r0
  00144dae: ldr r0,[r0,#0x4]
  00144db0: cbz r0,0x00144db6
  00144db2: blx 0x0006ebfc
  00144db6: movs r0,#0x0
  00144db8: str r0,[r4,#0x4]
  00144dba: mov r0,r4
  00144dbc: pop {r4,r6,r7,pc}
```
