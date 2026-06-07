# Station::removeShips
elf 0xa6c74 body 32
Sig: undefined __thiscall removeShips(Station * this)

## decompile
```c

/* Station::removeShips() */

void __thiscall Station::removeShips(Station *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x2c) != (Array *)0x0) {
    ArrayReleaseClasses<Ship*>(*(Array **)(this + 0x2c));
    if (*(Array<Ship*> **)(this + 0x2c) != (Array<Ship*> *)0x0) {
      pvVar1 = (void *)Array<Ship*>::~Array(*(Array<Ship*> **)(this + 0x2c));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x2c) = 0;
  return;
}

```

## target disasm
```
  000b6c74: push {r4,r6,r7,lr}
  000b6c76: add r7,sp,#0x8
  000b6c78: mov r4,r0
  000b6c7a: ldr r0,[r0,#0x2c]
  000b6c7c: cbz r0,0x000b6c8e
  000b6c7e: blx 0x00073408
  000b6c82: ldr r0,[r4,#0x2c]
  000b6c84: cbz r0,0x000b6c8e
  000b6c86: blx 0x00073414
  000b6c8a: blx 0x0006eb48
  000b6c8e: movs r0,#0x0
  000b6c90: str r0,[r4,#0x2c]
  000b6c92: pop {r4,r6,r7,pc}
```
