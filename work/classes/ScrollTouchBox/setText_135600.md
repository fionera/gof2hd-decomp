# ScrollTouchBox::setText
elf 0x135600 body 236
Sig: undefined __thiscall setText(ScrollTouchBox * this, String param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::setText(AbyssEngine::String, int) */

void __thiscall ScrollTouchBox::setText(ScrollTouchBox *this,int param_2,String *param_3)

{
  void *pvVar1;
  undefined4 *puVar2;
  Array *pAVar3;
  String *this_00;
  int iVar4;
  int iVar5;
  uint *puVar6;
  int *piVar7;
  
  if (*(Array **)this != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)this);
    if (*(Array<AbyssEngine::String*> **)this != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array(*(Array<AbyssEngine::String*> **)this);
      operator_delete(pvVar1);
    }
    *(undefined4 *)this = 0;
  }
  *(String **)(this + 0x3c) = param_3;
  pvVar1 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  puVar2 = (undefined4 *)(DAT_00145700 + 0x145642);
  *(void **)this = pvVar1;
  puVar6 = (uint *)*puVar2;
  Globals::getLineArray(*puVar6,param_3,param_2,*(Array **)(this + 0x14));
  piVar7 = *(int **)(DAT_00145704 + 0x145654);
  pAVar3 = *(Array **)this;
  iVar4 = *piVar7;
  iVar5 = *(int *)(iVar4 + 4) * *(int *)pAVar3;
  *(int *)(this + 0x18) = iVar5;
  if (iVar5 - *(int *)(this + 0x10) != 0 && *(int *)(this + 0x10) <= iVar5) {
    *(int *)(this + 0xc) = *(int *)(this + 0x14) - *(int *)(iVar4 + 0x48);
    if (pAVar3 != (Array *)0x0) {
      ArrayReleaseClasses<AbyssEngine::String*>(pAVar3);
      if (*(Array<AbyssEngine::String*> **)this != (Array<AbyssEngine::String*> *)0x0) {
        pvVar1 = (void *)Array<AbyssEngine::String*>::~Array(*(Array<AbyssEngine::String*> **)this);
        operator_delete(pvVar1);
      }
      *(undefined4 *)this = 0;
    }
    pvVar1 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    *(void **)this = pvVar1;
    Globals::getLineArray(*puVar6,param_3,param_2,*(Array **)(this + 0xc));
    this_00 = operator_new(0xc);
    AbyssEngine::String::String(this_00,(char *)(DAT_00145708 + 0x1456b4),false);
    ArrayAdd<AbyssEngine::String*>(this_00,*(Array **)this);
    *(int *)(this + 0x18) = *(int *)(*piVar7 + 4) * **(int **)this;
  }
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x2d) = 0;
  *(undefined4 *)(this + 0x29) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x28) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return;
}

```

## target disasm
```
  00145600: push {r4,r5,r6,r7,lr}
  00145602: add r7,sp,#0xc
  00145604: push {r6,r7,r8,r9,r10}
  00145608: mov r4,r0
  0014560a: ldr r0,[r0,#0x0]
  0014560c: mov r10,r2
  0014560e: mov r8,r1
  00145610: cbz r0,0x00145626
  00145612: blx 0x0006facc
  00145616: ldr r0,[r4,#0x0]
  00145618: cbz r0,0x00145622
  0014561a: blx 0x0006f64c
  0014561e: blx 0x0006eb48
  00145622: movs r0,#0x0
  00145624: str r0,[r4,#0x0]
  00145626: movs r0,#0xc
  00145628: str.w r10,[r4,#0x3c]
  0014562c: blx 0x0006eb24
  00145630: mov r5,r0
  00145632: blx 0x0006f628
  00145636: ldr r0,[0x00145700]
  00145638: mov r1,r10
  0014563a: ldr r3,[r4,#0x14]
  0014563c: mov r2,r8
  0014563e: add r0,pc
  00145640: str r5,[r4,#0x0]
  00145642: str r5,[sp,#0x0]
  00145644: ldr r6,[r0,#0x0]
  00145646: ldr r0,[r6,#0x0]
  00145648: blx 0x00076a44
  0014564c: ldr r0,[0x00145704]
  0014564e: ldr r2,[r4,#0x10]
  00145650: add r0,pc
  00145652: ldr.w r9,[r0,#0x0]
  00145656: ldr r0,[r4,#0x0]
  00145658: ldr.w r1,[r9,#0x0]
  0014565c: ldr r3,[r0,#0x0]
  0014565e: ldr r5,[r1,#0x4]
  00145660: muls r3,r5
  00145662: str r3,[r4,#0x18]
  00145664: cmp r3,r2
  00145666: ble 0x001456ce
  00145668: ldr r1,[r1,#0x48]
  0014566a: cmp r0,#0x0
  0014566c: ldr r2,[r4,#0x14]
  0014566e: sub.w r1,r2,r1
  00145672: str r1,[r4,#0xc]
  00145674: beq 0x0014568a
  00145676: blx 0x0006facc
  0014567a: ldr r0,[r4,#0x0]
  0014567c: cbz r0,0x00145686
  0014567e: blx 0x0006f64c
  00145682: blx 0x0006eb48
  00145686: movs r0,#0x0
  00145688: str r0,[r4,#0x0]
  0014568a: movs r0,#0xc
  0014568c: blx 0x0006eb24
  00145690: mov r5,r0
  00145692: blx 0x0006f628
  00145696: ldr r3,[r4,#0xc]
  00145698: mov r1,r10
  0014569a: str r5,[r4,#0x0]
  0014569c: mov r2,r8
  0014569e: ldr r0,[r6,#0x0]
  001456a0: str r5,[sp,#0x0]
  001456a2: blx 0x00076a44
  001456a6: movs r0,#0xc
  001456a8: blx 0x0006eb24
  001456ac: ldr r1,[0x00145708]
  001456ae: mov r5,r0
  001456b0: add r1,pc
  001456b2: movs r2,#0x0
  001456b4: blx 0x0006ee18
  001456b8: ldr r1,[r4,#0x0]
  001456ba: mov r0,r5
  001456bc: blx 0x0006f634
  001456c0: ldr r0,[r4,#0x0]
  001456c2: ldr.w r1,[r9,#0x0]
  001456c6: ldr r0,[r0,#0x0]
  001456c8: ldr r1,[r1,#0x4]
  001456ca: muls r0,r1
  001456cc: str r0,[r4,#0x18]
  001456ce: movs r0,#0x0
  001456d0: vmov.i32 q8,#0x0
  001456d4: str r0,[r4,#0x34]
  001456d6: str.w r0,[r4,#0x2d]
  001456da: str.w r0,[r4,#0x29]
  001456de: add.w r0,r4,#0x1c
  001456e2: vst1.32 {d16,d17},[r0]
  001456e6: pop.w {r2,r3,r8,r9,r10}
  001456ea: pop {r4,r5,r6,r7,pc}
```
