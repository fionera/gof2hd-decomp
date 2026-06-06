# Ship::addMod
elf 0x1755bc body 182
Sig: undefined __thiscall addMod(Ship * this, int param_1)

## decompile
```c

/* Ship::addMod(int) */

void __thiscall Ship::addMod(Ship *this,int param_1)

{
  bool bVar1;
  int extraout_r0;
  int iVar2;
  Item *this_00;
  void *pvVar3;
  uint uVar4;
  undefined4 *puVar5;
  Array *pAVar6;
  
  pAVar6 = *(Array **)(this + 0x78);
  if (pAVar6 == (Array *)0x0) {
    pAVar6 = operator_new(0xc);
    Array<int>::Array();
    *(Array **)(this + 0x78) = pAVar6;
  }
  bVar1 = false;
  for (uVar4 = 0; (!bVar1 && (uVar4 < *(uint *)pAVar6)); uVar4 = uVar4 + 1) {
    bVar1 = *(int *)(*(int *)(pAVar6 + 4) + uVar4 * 4) == param_1;
  }
  if (bVar1) {
    return;
  }
  ArrayAdd<int>(param_1,pAVar6);
  iVar2 = extraout_r0;
  if (param_1 == 2) {
    iVar2 = *(int *)(this + 0x7c);
  }
  if (param_1 == 2 && iVar2 == 0) {
    *(int *)(*(int *)(this + 0x68) + 0xc) = *(int *)(*(int *)(this + 0x68) + 0xc) + 1;
    *(int *)(this + 0x7c) = *(int *)(this + 0x7c) + 1;
    this_00 = operator_new(0x48);
    Item::Item(this_00,(Array *)0x0,(Array *)0x0,(Array *)0x0);
    ArrayAdd<Item*>(this_00,*(Array **)(this + 0x6c));
    puVar5 = (undefined4 *)((*(int **)(this + 0x6c))[1] + **(int **)(this + 0x6c) * 4 + -4);
    if ((Item *)*puVar5 != (Item *)0x0) {
      pvVar3 = (void *)Item::~Item((Item *)*puVar5);
      operator_delete(pvVar3);
      puVar5 = (undefined4 *)((*(int **)(this + 0x6c))[1] + **(int **)(this + 0x6c) * 4 + -4);
    }
    *puVar5 = 0;
  }
  (*(code *)(DAT_001abf14 + 0x1abf18))(this);
  return;
}

```
## target disasm
```
  001855bc: push {r4,r5,r6,r7,lr}
  001855be: add r7,sp,#0xc
  001855c0: push.w r11
  001855c4: ldr r6,[r0,#0x78]
  001855c6: mov r5,r1
  001855c8: mov r4,r0
  001855ca: cbnz r6,0x001855da
  001855cc: movs r0,#0xc
  001855ce: blx 0x0006eb24
  001855d2: mov r6,r0
  001855d4: blx 0x000701f8
  001855d8: str r6,[r4,#0x78]
  001855da: ldr r0,[r6,#0x0]
  001855dc: movs r2,#0x0
  001855de: movs r1,#0x0
  001855e0: b 0x001855f2
  001855e2: ldr r2,[r6,#0x4]
  001855e4: ldr.w r2,[r2,r1,lsl #0x2]
  001855e8: adds r1,#0x1
  001855ea: subs r2,r2,r5
  001855ec: clz r2,r2
  001855f0: lsrs r2,r2,#0x5
  001855f2: cbnz r2,0x001855f8
  001855f4: cmp r1,r0
  001855f6: bcc 0x001855e2
  001855f8: cbz r2,0x00185600
  001855fa: pop.w r11
  001855fe: pop {r4,r5,r6,r7,pc}
  00185600: mov r0,r5
  00185602: mov r1,r6
  00185604: blx 0x0007021c
  00185608: cmp r5,#0x2
  0018560a: itt eq
  0018560c: ldr.eq r0,[r4,#0x7c]
  0018560e: cmp.eq r0,#0x0
  00185610: beq 0x00185620
  00185612: mov r0,r4
  00185614: pop.w r11
  00185618: pop.w {r4,r5,r6,r7,lr}
  0018561c: b.w 0x001abf08
  00185620: ldr r0,[r4,#0x68]
  00185622: ldr r1,[r0,#0xc]
  00185624: adds r1,#0x1
  00185626: str r1,[r0,#0xc]
  00185628: ldr r0,[r4,#0x7c]
  0018562a: adds r0,#0x1
  0018562c: str r0,[r4,#0x7c]
  0018562e: movs r0,#0x48
  00185630: blx 0x0006eb24
  00185634: mov r5,r0
  00185636: movs r1,#0x0
  00185638: movs r2,#0x0
  0018563a: movs r3,#0x0
  0018563c: blx 0x0007636c
  00185640: ldr r1,[r4,#0x6c]
  00185642: mov r0,r5
  00185644: blx 0x000718b4
  00185648: ldr r0,[r4,#0x6c]
  0018564a: ldrd r1,r0,[r0,#0x0]
  0018564e: add.w r2,r0,r1, lsl #0x2
  00185652: ldr.w r0,[r2,#-0x4]!
  00185656: cbz r0,0x0018566c
  00185658: blx 0x00073474
  0018565c: blx 0x0006eb48
  00185660: ldr r0,[r4,#0x6c]
  00185662: ldrd r1,r0,[r0,#0x0]
  00185666: add.w r0,r0,r1, lsl #0x2
  0018566a: subs r2,r0,#0x4
  0018566c: movs r0,#0x0
  0018566e: str r0,[r2,#0x0]
  00185670: b 0x00185612
```
