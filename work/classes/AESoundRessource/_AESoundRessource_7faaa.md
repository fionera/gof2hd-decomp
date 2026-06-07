# AESoundRessource::~AESoundRessource
elf 0x7faaa body 84
Sig: undefined __thiscall ~AESoundRessource(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::~AESoundRessource() */

AESoundRessource * __thiscall
AbyssEngine::AESoundRessource::~AESoundRessource(AESoundRessource *this)

{
  void *pvVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  Array *this_00;
  
  freeAllRessources(this);
  this_00 = (Array *)(this + 4);
  iVar4 = 0;
  for (uVar3 = 0; uVar3 < *(uint *)this_00; uVar3 = uVar3 + 1) {
    pvVar1 = *(void **)(*(int *)(this + 8) + iVar4);
    if (pvVar1 == (void *)0x0) {
      puVar2 = (undefined4 *)(*(int *)(this + 8) + uVar3 * 4);
    }
    else {
      operator_delete(pvVar1);
      puVar2 = (undefined4 *)(*(int *)(this + 8) + iVar4);
    }
    *puVar2 = 0;
    iVar4 = iVar4 + 4;
  }
  ArrayRemoveAll<AbyssEngine::AESoundInterface*>(this_00);
  Array<AbyssEngine::AESoundInterface*>::~Array((Array<AbyssEngine::AESoundInterface*> *)this_00);
  return this;
}

```

## target disasm
```
  0008faaa: push {r4,r5,r6,r7,lr}
  0008faac: add r7,sp,#0xc
  0008faae: push {r8,r9,r11}
  0008fab2: mov r4,r0
  0008fab4: blx 0x00070090
  0008fab8: add.w r9,r4,#0x4
  0008fabc: mov.w r8,#0x0
  0008fac0: movs r6,#0x0
  0008fac2: movs r5,#0x0
  0008fac4: b 0x0008fae2
  0008fac6: ldr r1,[r4,#0x8]
  0008fac8: ldr r0,[r1,r6]
  0008faca: cbz r0,0x0008fad6
  0008facc: blx 0x0006eb48
  0008fad0: ldr r0,[r4,#0x8]
  0008fad2: add r0,r6
  0008fad4: b 0x0008fada
  0008fad6: add.w r0,r1,r5, lsl #0x2
  0008fada: str.w r8,[r0,#0x0]
  0008fade: adds r6,#0x4
  0008fae0: adds r5,#0x1
  0008fae2: ldr.w r0,[r9,#0x0]
  0008fae6: cmp r5,r0
  0008fae8: bcc 0x0008fac6
  0008faea: mov r0,r9
  0008faec: blx 0x0007009c
  0008faf0: mov r0,r9
  0008faf2: blx 0x000700a8
  0008faf6: mov r0,r4
  0008faf8: pop.w {r8,r9,r11}
  0008fafc: pop {r4,r5,r6,r7,pc}
```
