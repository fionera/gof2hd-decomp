# Globals::getRandomStation
elf 0xe4af0 body 54
Sig: undefined __stdcall getRandomStation(void)

## decompile
```c

/* Globals::getRandomStation() */

undefined4 Globals::getRandomStation(void)

{
  FileRead *this;
  undefined4 uVar1;
  void *pvVar2;
  
  this = operator_new(1);
  FileRead::FileRead(this);
  AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4b34 + 0xf4b08));
  uVar1 = FileRead::loadStation((int)this);
  pvVar2 = (void *)FileRead::~FileRead(this);
  operator_delete(pvVar2);
  return uVar1;
}

```

## target disasm
```
  000f4af0: push {r4,r5,r7,lr}
  000f4af2: add r7,sp,#0x8
  000f4af4: movs r0,#0x1
  000f4af6: blx 0x0006eb24
  000f4afa: mov r4,r0
  000f4afc: blx 0x00072124
  000f4b00: ldr r0,[0x000f4b34]
  000f4b02: movs r1,#0x87
  000f4b04: add r0,pc
  000f4b06: ldr r0,[r0,#0x0]
  000f4b08: ldr r0,[r0,#0x0]
  000f4b0a: blx 0x00071848
  000f4b0e: mov r1,r0
  000f4b10: mov r0,r4
  000f4b12: blx 0x000737a4
  000f4b16: mov r5,r0
  000f4b18: mov r0,r4
  000f4b1a: blx 0x0007213c
  000f4b1e: blx 0x0006eb48
  000f4b22: mov r0,r5
  000f4b24: pop {r4,r5,r7,pc}
```
