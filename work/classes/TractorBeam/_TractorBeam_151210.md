# TractorBeam::~TractorBeam
elf 0x151210 body 26
Sig: undefined __thiscall ~TractorBeam(TractorBeam * this)

## decompile
```c

/* TractorBeam::~TractorBeam() */

TractorBeam * __thiscall TractorBeam::~TractorBeam(TractorBeam *this)

{
  void *pvVar1;
  
  if (*(AEGeometry **)(this + 0x14) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x14));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14) = 0;
  return this;
}

```

## target disasm
```
  00161210: push {r4,r6,r7,lr}
  00161212: add r7,sp,#0x8
  00161214: mov r4,r0
  00161216: ldr r0,[r0,#0x14]
  00161218: cbz r0,0x00161222
  0016121a: blx 0x00071fc8
  0016121e: blx 0x0006eb48
  00161222: movs r0,#0x0
  00161224: str r0,[r4,#0x14]
  00161226: mov r0,r4
  00161228: pop {r4,r6,r7,pc}
```
