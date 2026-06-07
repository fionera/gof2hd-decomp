# Galaxy::Galaxy
elf 0x175e08 body 62
Sig: undefined __thiscall Galaxy(Galaxy * this)

## decompile
```c

/* Galaxy::Galaxy() */

Galaxy * __thiscall Galaxy::Galaxy(Galaxy *this)

{
  uint8_t *puVar1;
  FileRead *this_00;
  AEArray *pAVar2;
  void *pvVar3;
  int iVar4;
  
  puVar1 = operator_new__(0x87);
  this->stations = puVar1;
  for (iVar4 = 0; iVar4 != 0x87; iVar4 = iVar4 + 1) {
    puVar1[iVar4] = '\0';
  }
  this_00 = operator_new(1);
  FileRead::FileRead(this_00);
  pAVar2 = (AEArray *)FileRead::loadSystemsBinary();
  this->systems = pAVar2;
  pvVar3 = (void *)FileRead::~FileRead(this_00);
  operator_delete(pvVar3);
  return this;
}

```

## target disasm
```
  00185e08: push {r4,r5,r7,lr}
  00185e0a: add r7,sp,#0x8
  00185e0c: mov r4,r0
  00185e0e: movs r0,#0x87
  00185e10: blx 0x0006ec08
  00185e14: movs r1,#0x0
  00185e16: movs r2,#0x0
  00185e18: str r0,[r4,#0x0]
  00185e1a: b 0x00185e20
  00185e1c: strb r1,[r0,r2]
  00185e1e: adds r2,#0x1
  00185e20: cmp r2,#0x87
  00185e22: bne 0x00185e1c
  00185e24: movs r0,#0x1
  00185e26: blx 0x0006eb24
  00185e2a: mov r5,r0
  00185e2c: blx 0x00072124
  00185e30: mov r0,r5
  00185e32: blx 0x0007378c
  00185e36: str r0,[r4,#0x4]
  00185e38: mov r0,r5
  00185e3a: blx 0x0007213c
  00185e3e: blx 0x0006eb48
  00185e42: mov r0,r4
  00185e44: pop {r4,r5,r7,pc}
```
