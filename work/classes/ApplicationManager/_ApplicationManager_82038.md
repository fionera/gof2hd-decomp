# ApplicationManager::~ApplicationManager
elf 0x82038 body 220
Sig: undefined __thiscall ~ApplicationManager(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::~ApplicationManager() */

ApplicationManager * __thiscall
AbyssEngine::ApplicationManager::~ApplicationManager(ApplicationManager *this)

{
  int *piVar1;
  undefined4 *puVar2;
  void *pvVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  Array *this_00;
  
  if (*(int **)(this + 0x18) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x18) + 0xc))();
  }
  this_00 = (Array *)(this + 0x44);
  iVar4 = 0;
  for (uVar6 = 0; uVar6 < *(uint *)this_00; uVar6 = uVar6 + 1) {
    piVar1 = *(int **)(*(int *)(this + 0x48) + iVar4);
    if (piVar1 == (int *)0x0) {
      puVar2 = (undefined4 *)(*(int *)(this + 0x48) + uVar6 * 4);
    }
    else {
      (**(code **)(*piVar1 + 4))();
      puVar2 = (undefined4 *)(*(int *)(this + 0x48) + iVar4);
    }
    *puVar2 = 0;
    iVar4 = iVar4 + 4;
  }
  ArrayRelease<AbyssEngine::IApplicationModule*>(this_00);
  ArrayRelease<unsigned_int>((Array *)(this + 0x50));
  if (*(PaintCanvas **)this != (PaintCanvas *)0x0) {
    pvVar3 = (void *)PaintCanvas::~PaintCanvas(*(PaintCanvas **)this);
    operator_delete(pvVar3);
  }
  *(undefined4 *)this = 0;
  if (*(AESoundRessource **)(this + 0xac) != (AESoundRessource *)0x0) {
    pvVar3 = (void *)AESoundRessource::~AESoundRessource(*(AESoundRessource **)(this + 0xac));
    operator_delete(pvVar3);
  }
  *(undefined4 *)(this + 0xac) = 0;
  if (*(CheatHandler **)(this + 0x30) != (CheatHandler *)0x0) {
    pvVar3 = (void *)CheatHandler::~CheatHandler(*(CheatHandler **)(this + 0x30));
    operator_delete(pvVar3);
  }
  *(undefined4 *)(this + 0x30) = 0;
  if (*(ConfigReader **)(this + 0x38) != (ConfigReader *)0x0) {
    pvVar3 = (void *)ConfigReader::~ConfigReader(*(ConfigReader **)(this + 0x38));
    operator_delete(pvVar3);
  }
  iVar4 = *(int *)(this + 0x10);
  *(undefined4 *)(this + 0x38) = 0;
  if (iVar4 != 0) {
    if (*(int *)(iVar4 + -4) != 0) {
      iVar5 = *(int *)(iVar4 + -4) << 4;
      do {
        String::~String((String *)(iVar4 + -0xc + iVar5));
        iVar5 = iVar5 + -0x10;
      } while (iVar5 != 0);
    }
    operator_delete__((void *)(iVar4 + -8));
  }
  *(undefined4 *)(this + 0x10) = 0;
  Array<long_long>::~Array((Array<long_long> *)(this + 0x88));
  Array<unsigned_int>::~Array((Array<unsigned_int> *)(this + 0x50));
  Array<AbyssEngine::IApplicationModule*>::~Array
            ((Array<AbyssEngine::IApplicationModule*> *)this_00);
  return this;
}

```

## target disasm
```
  00092038: push {r4,r5,r6,r7,lr}
  0009203a: add r7,sp,#0xc
  0009203c: push {r8,r9,r10}
  00092040: mov r4,r0
  00092042: ldr r0,[r0,#0x18]
  00092044: cbz r0,0x0009204c
  00092046: ldr r1,[r0,#0x0]
  00092048: ldr r1,[r1,#0xc]
  0009204a: blx r1
  0009204c: add.w r9,r4,#0x44
  00092050: mov.w r8,#0x0
  00092054: movs r5,#0x0
  00092056: movs r6,#0x0
  00092058: b 0x00092078
  0009205a: ldr r1,[r4,#0x48]
  0009205c: ldr r0,[r1,r5]
  0009205e: cbz r0,0x0009206c
  00092060: ldr r1,[r0,#0x0]
  00092062: ldr r1,[r1,#0x4]
  00092064: blx r1
  00092066: ldr r0,[r4,#0x48]
  00092068: add r0,r5
  0009206a: b 0x00092070
  0009206c: add.w r0,r1,r6, lsl #0x2
  00092070: str.w r8,[r0,#0x0]
  00092074: adds r5,#0x4
  00092076: adds r6,#0x1
  00092078: ldr.w r0,[r9,#0x0]
  0009207c: cmp r6,r0
  0009207e: bcc 0x0009205a
  00092080: mov r0,r9
  00092082: blx 0x000702b8
  00092086: add.w r8,r4,#0x50
  0009208a: mov r0,r8
  0009208c: blx 0x0006f97c
  00092090: ldr r0,[r4,#0x0]
  00092092: cbz r0,0x0009209c
  00092094: blx 0x000702c4
  00092098: blx 0x0006eb48
  0009209c: ldr.w r0,[r4,#0xac]
  000920a0: movs r5,#0x0
  000920a2: str r5,[r4,#0x0]
  000920a4: cbz r0,0x000920ae
  000920a6: blx 0x000702d0
  000920aa: blx 0x0006eb48
  000920ae: ldr r0,[r4,#0x30]
  000920b0: str.w r5,[r4,#0xac]
  000920b4: cbz r0,0x000920be
  000920b6: blx 0x000702dc
  000920ba: blx 0x0006eb48
  000920be: ldr r0,[r4,#0x38]
  000920c0: movs r5,#0x0
  000920c2: str r5,[r4,#0x30]
  000920c4: cbz r0,0x000920ce
  000920c6: blx 0x000702e8
  000920ca: blx 0x0006eb48
  000920ce: ldr r0,[r4,#0x10]
  000920d0: str r5,[r4,#0x38]
  000920d2: cbz r0,0x000920f4
  000920d4: ldr.w r1,[r0,#-0x4]
  000920d8: sub.w r10,r0,#0x8
  000920dc: cbz r1,0x000920ee
  000920de: sub.w r6,r0,#0xc
  000920e2: lsls r5,r1,#0x4
  000920e4: adds r0,r6,r5
  000920e6: blx 0x0006ee30
  000920ea: subs r5,#0x10
  000920ec: bne 0x000920e4
  000920ee: mov r0,r10
  000920f0: blx 0x0006ebfc
  000920f4: movs r0,#0x0
  000920f6: str r0,[r4,#0x10]
  000920f8: add.w r0,r4,#0x88
  000920fc: blx 0x000702a0
  00092100: mov r0,r8
  00092102: blx 0x0006f904
  00092106: mov r0,r9
  00092108: blx 0x000702ac
  0009210c: mov r0,r4
  0009210e: pop.w {r8,r9,r10}
  00092112: pop {r4,r5,r6,r7,pc}
```
