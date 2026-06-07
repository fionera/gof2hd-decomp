# Globals::startNewSoundResourceList
elf 0xe5554 body 64
Sig: undefined __stdcall startNewSoundResourceList(void)

## decompile
```c

/* Globals::startNewSoundResourceList() */

void Globals::startNewSoundResourceList(void)

{
  int in_r0;
  void *pvVar1;
  Array *pAVar2;
  
  if (*(Array **)(in_r0 + 4) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(in_r0 + 4));
    if (*(Array<int> **)(in_r0 + 4) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(in_r0 + 4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(in_r0 + 4) = 0;
  pAVar2 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(in_r0 + 4) = pAVar2;
  ArrayAdd<int>(0x7c,pAVar2);
  (*(code *)(DAT_001ac1b4 + 0x1ac1b8))(0x7b,*(undefined4 *)(in_r0 + 4));
  return;
}

```

## target disasm
```
  000f5554: push {r4,r5,r7,lr}
  000f5556: add r7,sp,#0x8
  000f5558: mov r4,r0
  000f555a: ldr r0,[r0,#0x4]
  000f555c: cbz r0,0x000f556e
  000f555e: blx 0x0007369c
  000f5562: ldr r0,[r4,#0x4]
  000f5564: cbz r0,0x000f556e
  000f5566: blx 0x00070204
  000f556a: blx 0x0006eb48
  000f556e: movs r0,#0x0
  000f5570: str r0,[r4,#0x4]
  000f5572: movs r0,#0xc
  000f5574: blx 0x0006eb24
  000f5578: mov r5,r0
  000f557a: blx 0x000701f8
  000f557e: movs r0,#0x7c
  000f5580: mov r1,r5
  000f5582: str r5,[r4,#0x4]
  000f5584: blx 0x0007021c
  000f5588: ldr r1,[r4,#0x4]
  000f558a: movs r0,#0x7b
  000f558c: pop.w {r4,r5,r7,lr}
  000f5590: b.w 0x001ac1a8
```
