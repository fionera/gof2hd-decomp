# CheatHandler::~CheatHandler
elf 0x7449c body 94
Sig: undefined __thiscall ~CheatHandler(CheatHandler * this)

## decompile
```c

/* AbyssEngine::CheatHandler::~CheatHandler() */

CheatHandler * __thiscall AbyssEngine::CheatHandler::~CheatHandler(CheatHandler *this)

{
  Array *pAVar1;
  CheatCode *this_00;
  undefined4 *puVar2;
  void *pvVar3;
  uint uVar4;
  int iVar5;
  
  pAVar1 = *(Array **)(this + 8);
  if (pAVar1 != (Array *)0x0) {
    iVar5 = 0;
    for (uVar4 = 0; uVar4 < *(uint *)pAVar1; uVar4 = uVar4 + 1) {
      this_00 = *(CheatCode **)(*(int *)(pAVar1 + 4) + iVar5);
      if (this_00 == (CheatCode *)0x0) {
        puVar2 = (undefined4 *)(*(int *)(pAVar1 + 4) + uVar4 * 4);
      }
      else {
        pvVar3 = (void *)CheatCode::~CheatCode(this_00);
        operator_delete(pvVar3);
        puVar2 = (undefined4 *)(*(int *)(*(int *)(this + 8) + 4) + iVar5);
      }
      *puVar2 = 0;
      iVar5 = iVar5 + 4;
      pAVar1 = *(Array **)(this + 8);
    }
    ArrayRemoveAll<AbyssEngine::CheatCode*>(pAVar1);
    if (*(Array<AbyssEngine::CheatCode*> **)(this + 8) != (Array<AbyssEngine::CheatCode*> *)0x0) {
      pvVar3 = (void *)Array<AbyssEngine::CheatCode*>::~Array
                                 (*(Array<AbyssEngine::CheatCode*> **)(this + 8));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(this + 8) = 0;
  }
  return this;
}

```

## target disasm
```
  0008449c: push {r4,r5,r6,r7,lr}
  0008449e: add r7,sp,#0xc
  000844a0: push.w r8
  000844a4: mov r4,r0
  000844a6: ldr r0,[r0,#0x8]
  000844a8: cbz r0,0x000844f2
  000844aa: mov.w r8,#0x0
  000844ae: movs r6,#0x0
  000844b0: movs r5,#0x0
  000844b2: b 0x000844d8
  000844b4: ldr r1,[r0,#0x4]
  000844b6: ldr r0,[r1,r6]
  000844b8: cbz r0,0x000844ca
  000844ba: blx 0x0006f6d0
  000844be: blx 0x0006eb48
  000844c2: ldr r0,[r4,#0x8]
  000844c4: ldr r0,[r0,#0x4]
  000844c6: add r0,r6
  000844c8: b 0x000844ce
  000844ca: add.w r0,r1,r5, lsl #0x2
  000844ce: str.w r8,[r0,#0x0]
  000844d2: adds r6,#0x4
  000844d4: ldr r0,[r4,#0x8]
  000844d6: adds r5,#0x1
  000844d8: ldr r1,[r0,#0x0]
  000844da: cmp r5,r1
  000844dc: bcc 0x000844b4
  000844de: blx 0x0006f6dc
  000844e2: ldr r0,[r4,#0x8]
  000844e4: cbz r0,0x000844ee
  000844e6: blx 0x0006f6e8
  000844ea: blx 0x0006eb48
  000844ee: movs r0,#0x0
  000844f0: str r0,[r4,#0x8]
  000844f2: mov r0,r4
  000844f4: pop.w r8
  000844f8: pop {r4,r5,r6,r7,pc}
```
