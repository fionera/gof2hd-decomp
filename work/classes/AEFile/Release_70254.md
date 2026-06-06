# AEFile::Release
elf 0x70254 body 134
Sig: undefined __stdcall Release(void)

## decompile
```c

/* AEFile::Release() */

void AEFile::Release(void)

{
  void *pvVar1;
  Array<AEPakFileEntry*> *this;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  
  puVar2 = *(undefined4 **)(DAT_000802dc + 0x80262);
  if ((Array *)*puVar2 != (Array *)0x0) {
    ArrayReleaseClasses<AELowLevelFile*>((Array *)*puVar2);
    if ((Array<AELowLevelFile*> *)*puVar2 != (Array<AELowLevelFile*> *)0x0) {
      pvVar1 = (void *)Array<AELowLevelFile*>::~Array((Array<AELowLevelFile*> *)*puVar2);
      operator_delete(pvVar1);
    }
    *puVar2 = 0;
  }
  piVar5 = *(int **)(DAT_000802e0 + 0x80280);
  this = (Array<AEPakFileEntry*> *)*piVar5;
  if (this != (Array<AEPakFileEntry*> *)0x0) {
    iVar4 = 0;
    for (uVar3 = 0; uVar3 < *(uint *)this; uVar3 = uVar3 + 1) {
      pvVar1 = *(void **)(*(int *)(this + 4) + iVar4);
      if (pvVar1 == (void *)0x0) {
        puVar2 = (undefined4 *)(*(int *)(this + 4) + uVar3 * 4);
      }
      else {
        AbyssEngine::String::~String((String *)((int)pvVar1 + 4));
        operator_delete(pvVar1);
        puVar2 = (undefined4 *)(*(int *)(*piVar5 + 4) + iVar4);
      }
      *puVar2 = 0;
      iVar4 = iVar4 + 4;
      this = (Array<AEPakFileEntry*> *)*piVar5;
    }
    if (this != (Array<AEPakFileEntry*> *)0x0) {
      pvVar1 = (void *)Array<AEPakFileEntry*>::~Array(this);
      operator_delete(pvVar1);
    }
    *piVar5 = 0;
  }
  return;
}

```
## target disasm
```
  00080254: push {r4,r5,r6,r7,lr}
  00080256: add r7,sp,#0xc
  00080258: push {r8,r9,r11}
  0008025c: ldr r0,[0x000802dc]
  0008025e: add r0,pc
  00080260: ldr r4,[r0,#0x0]
  00080262: ldr r0,[r4,#0x0]
  00080264: cbz r0,0x0008027a
  00080266: blx 0x0006f328
  0008026a: ldr r0,[r4,#0x0]
  0008026c: cbz r0,0x00080276
  0008026e: blx 0x0006f334
  00080272: blx 0x0006eb48
  00080276: movs r0,#0x0
  00080278: str r0,[r4,#0x0]
  0008027a: ldr r0,[0x000802e0]
  0008027c: add r0,pc
  0008027e: ldr.w r9,[r0,#0x0]
  00080282: ldr.w r0,[r9,#0x0]
  00080286: cbz r0,0x000802d4
  00080288: mov.w r8,#0x0
  0008028c: movs r6,#0x0
  0008028e: movs r5,#0x0
  00080290: b 0x000802be
  00080292: ldr r0,[r0,#0x4]
  00080294: ldr r4,[r0,r6]
  00080296: cbz r4,0x000802ae
  00080298: adds r0,r4,#0x4
  0008029a: blx 0x0006ee30
  0008029e: mov r0,r4
  000802a0: blx 0x0006eb48
  000802a4: ldr.w r0,[r9,#0x0]
  000802a8: ldr r0,[r0,#0x4]
  000802aa: add r0,r6
  000802ac: b 0x000802b2
  000802ae: add.w r0,r0,r5, lsl #0x2
  000802b2: str.w r8,[r0,#0x0]
  000802b6: adds r6,#0x4
  000802b8: ldr.w r0,[r9,#0x0]
  000802bc: adds r5,#0x1
  000802be: ldr r1,[r0,#0x0]
  000802c0: cmp r5,r1
  000802c2: bcc 0x00080292
  000802c4: cbz r0,0x000802ce
  000802c6: blx 0x0006f340
  000802ca: blx 0x0006eb48
  000802ce: movs r0,#0x0
  000802d0: str.w r0,[r9,#0x0]
  000802d4: pop.w {r8,r9,r11}
  000802d8: pop {r4,r5,r6,r7,pc}
```
