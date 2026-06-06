# Ship::setMods
elf 0x1756ce body 162
Sig: undefined __thiscall setMods(Ship * this, Array * param_1)

## decompile
```c

/* Ship::setMods(Array<int>*) */

void __thiscall Ship::setMods(Ship *this,Array *param_1)

{
  int iVar1;
  Item *this_00;
  void *pvVar2;
  undefined4 *puVar3;
  uint uVar4;
  bool bVar5;
  
  *(Array **)(this + 0x78) = param_1;
  if (param_1 != (Array *)0x0) {
    for (uVar4 = 0; uVar4 < *(uint *)param_1; uVar4 = uVar4 + 1) {
      iVar1 = *(int *)(*(int *)(param_1 + 4) + uVar4 * 4);
      bVar5 = iVar1 == 2;
      if (bVar5) {
        iVar1 = *(int *)(this + 0x7c);
      }
      if (bVar5 && iVar1 == 0) {
        *(int *)(*(int *)(this + 0x68) + 0xc) = *(int *)(*(int *)(this + 0x68) + 0xc) + 1;
        *(int *)(this + 0x7c) = *(int *)(this + 0x7c) + 1;
        this_00 = operator_new(0x48);
        Item::Item(this_00,(Array *)0x0,(Array *)0x0,(Array *)0x0);
        ArrayAdd<Item*>(this_00,*(Array **)(this + 0x6c));
        puVar3 = (undefined4 *)((*(int **)(this + 0x6c))[1] + **(int **)(this + 0x6c) * 4 + -4);
        if ((Item *)*puVar3 != (Item *)0x0) {
          pvVar2 = (void *)Item::~Item((Item *)*puVar3);
          operator_delete(pvVar2);
          puVar3 = (undefined4 *)((*(int **)(this + 0x6c))[1] + **(int **)(this + 0x6c) * 4 + -4);
        }
        *puVar3 = 0;
      }
    }
  }
  (*(code *)(DAT_001abf14 + 0x1abf18))(this);
  return;
}

```
## target disasm
```
  001856ce: push {r4,r5,r6,r7,lr}
  001856d0: add r7,sp,#0xc
  001856d2: push {r8,r9,r11}
  001856d6: mov r5,r1
  001856d8: mov r9,r0
  001856da: cmp r1,#0x0
  001856dc: str r1,[r0,#0x78]
  001856de: beq 0x00185760
  001856e0: mov.w r8,#0x0
  001856e4: movs r4,#0x0
  001856e6: ldr r0,[r5,#0x0]
  001856e8: cmp r4,r0
  001856ea: bcs 0x00185760
  001856ec: ldr r0,[r5,#0x4]
  001856ee: ldr.w r0,[r0,r4,lsl #0x2]
  001856f2: cmp r0,#0x2
  001856f4: itt eq
  001856f6: ldr.eq.w r0,[r9,#0x7c]
  001856fa: cmp.eq r0,#0x0
  001856fc: beq 0x00185702
  001856fe: adds r4,#0x1
  00185700: b 0x001856e6
  00185702: ldr.w r0,[r9,#0x68]
  00185706: ldr r1,[r0,#0xc]
  00185708: adds r1,#0x1
  0018570a: str r1,[r0,#0xc]
  0018570c: ldr.w r0,[r9,#0x7c]
  00185710: adds r0,#0x1
  00185712: str.w r0,[r9,#0x7c]
  00185716: movs r0,#0x48
  00185718: blx 0x0006eb24
  0018571c: mov r6,r0
  0018571e: movs r1,#0x0
  00185720: movs r2,#0x0
  00185722: movs r3,#0x0
  00185724: blx 0x0007636c
  00185728: ldr.w r1,[r9,#0x6c]
  0018572c: mov r0,r6
  0018572e: blx 0x000718b4
  00185732: ldr.w r0,[r9,#0x6c]
  00185736: ldrd r1,r0,[r0,#0x0]
  0018573a: add.w r2,r0,r1, lsl #0x2
  0018573e: ldr.w r0,[r2,#-0x4]!
  00185742: cbz r0,0x0018575a
  00185744: blx 0x00073474
  00185748: blx 0x0006eb48
  0018574c: ldr.w r0,[r9,#0x6c]
  00185750: ldrd r1,r0,[r0,#0x0]
  00185754: add.w r0,r0,r1, lsl #0x2
  00185758: subs r2,r0,#0x4
  0018575a: str.w r8,[r2,#0x0]
  0018575e: b 0x001856fe
  00185760: mov r0,r9
  00185762: pop.w {r8,r9,r11}
  00185766: pop.w {r4,r5,r6,r7,lr}
  0018576a: b.w 0x001abf08
  001abf08: bx pc
```
