# Ship::removeAllCargo
elf 0x174eda body 24
Sig: undefined __thiscall removeAllCargo(Ship * this)

## decompile
```c

/* Ship::removeAllCargo() */

void __thiscall Ship::removeAllCargo(Ship *this)

{
  void *pvVar1;
  
  if (*(Array<Item*> **)(this + 0x70) != (Array<Item*> *)0x0) {
    pvVar1 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x70));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x70) = 0;
  return;
}

```
## target disasm
```
  00184eda: push {r4,r6,r7,lr}
  00184edc: add r7,sp,#0x8
  00184ede: mov r4,r0
  00184ee0: ldr r0,[r0,#0x70]
  00184ee2: cbz r0,0x00184eec
  00184ee4: blx 0x0007342c
  00184ee8: blx 0x0006eb48
  00184eec: movs r0,#0x0
  00184eee: str r0,[r4,#0x70]
  00184ef0: pop {r4,r6,r7,pc}
```
