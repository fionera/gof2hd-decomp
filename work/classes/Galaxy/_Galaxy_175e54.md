# Galaxy::~Galaxy
elf 0x175e54 body 42
Sig: undefined __thiscall ~Galaxy(Galaxy * this)

## decompile
```c

/* Galaxy::~Galaxy() */

Galaxy * __thiscall Galaxy::~Galaxy(Galaxy *this)

{
  void *pvVar1;
  
  operator_delete__(this->stations);
  this->stations = (uint8_t *)0x0;
  ArrayReleaseClasses<SolarSystem*>((Array *)this->systems);
  if (this->systems != (AEArray *)0x0) {
    pvVar1 = (void *)Array<SolarSystem*>::~Array((Array<SolarSystem*> *)this->systems);
    operator_delete(pvVar1);
  }
  this->systems = (AEArray *)0x0;
  return this;
}

```

## target disasm
```
  00185e54: push {r4,r6,r7,lr}
  00185e56: add r7,sp,#0x8
  00185e58: mov r4,r0
  00185e5a: ldr r0,[r0,#0x0]
  00185e5c: blx 0x0006ebfc
  00185e60: ldr r0,[r4,#0x4]
  00185e62: movs r1,#0x0
  00185e64: str r1,[r4,#0x0]
  00185e66: blx 0x00073804
  00185e6a: ldr r0,[r4,#0x4]
  00185e6c: cbz r0,0x00185e76
  00185e6e: blx 0x00073810
  00185e72: blx 0x0006eb48
  00185e76: movs r0,#0x0
  00185e78: str r0,[r4,#0x4]
  00185e7a: mov r0,r4
  00185e7c: pop {r4,r6,r7,pc}
```
