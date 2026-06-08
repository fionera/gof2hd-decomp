# Array<SolarSystem*>::~Array
elf 0xad2ec body 22
Sig: undefined __thiscall ~Array(Array<SolarSystem*> * this)

## decompile
```c

/* Array<SolarSystem*>::~Array() */

Array<SolarSystem*> * __thiscall Array<SolarSystem*>::~Array(Array<SolarSystem*> *this)

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
  000bd2ec: push {r4,r6,r7,lr}
  000bd2ee: add r7,sp,#0x8
  000bd2f0: mov r4,r0
  000bd2f2: ldr r0,[r0,#0x4]
  000bd2f4: cbz r0,0x000bd2fa
  000bd2f6: blx 0x0006ebfc
  000bd2fa: movs r0,#0x0
  000bd2fc: str r0,[r4,#0x4]
  000bd2fe: mov r0,r4
  000bd300: pop {r4,r6,r7,pc}
```
