# BluePrint::addItem
elf 0x177008 body 270
Sig: undefined __thiscall addItem(BluePrint * this, Item * param_1, int param_2, int param_3)

## decompile
```c

/* BluePrint::addItem(Item*, int, int) */

void __thiscall BluePrint::addItem(BluePrint *this,Item *param_1,int param_2,int param_3)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  Station *pSVar4;
  void *pvVar5;
  uint uVar6;
  int iVar7;
  int *piVar8;
  
  piVar8 = *(int **)(DAT_00187128 + 0x187020);
  iVar1 = *piVar8;
  if (param_2 != 0) {
    Item::setBlueprintAmount(param_1,0);
    puVar2 = (uint *)getIngredientList(this);
    if (puVar2 != (uint *)0x0) {
      for (uVar6 = 0; uVar6 < *puVar2; uVar6 = uVar6 + 1) {
        iVar7 = *(int *)(puVar2[1] + uVar6 * 4);
        iVar3 = Item::getIndex(param_1);
        if (iVar7 == iVar3) {
          *(int *)(*(int *)(*(int *)this + 4) + uVar6 * 4) =
               *(int *)(*(int *)(*(int *)this + 4) + uVar6 * 4) - param_2;
          iVar3 = Item::getSinglePrice(param_1);
          *(int *)(this + 4) = iVar3 * param_2 + *(int *)(this + 4);
          if ((-1 < param_3) && (*(int *)(this + 0x10) < 0)) {
            *(int *)(this + 0x10) = param_3;
            pSVar4 = (Station *)Status::getStation();
            iVar3 = Station::getIndex(pSVar4);
            if (iVar3 == param_3) {
              Status::getStation();
              Station::getName();
              AbyssEngine::String::operator=((String *)(this + 0x14),&stack0xffffffcc);
              AbyssEngine::String::~String((String *)&stack0xffffffcc);
            }
            else {
              pSVar4 = (Station *)Galaxy::getStation(**(int **)(DAT_00187130 + 0x1870e4));
              Station::getName();
              AbyssEngine::String::operator=((String *)(this + 0x14),&stack0xffffffcc);
              AbyssEngine::String::~String((String *)&stack0xffffffcc);
              if (pSVar4 != (Station *)0x0) {
                pvVar5 = (void *)Station::~Station(pSVar4);
                operator_delete(pvVar5);
              }
            }
          }
          break;
        }
      }
    }
  }
  if (*piVar8 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00187008: push {r4,r5,r6,r7,lr}
  0018700a: add r7,sp,#0xc
  0018700c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00187010: str r3,[sp,#0x0]
  00187012: mov r9,r0
  00187014: ldr r0,[0x00187128]
  00187016: mov r11,r2
  00187018: mov r5,r1
  0018701a: cmp r2,#0x0
  0018701c: add r0,pc
  0018701e: ldr.w r10,[r0,#0x0]
  00187022: ldr.w r0,[r10,#0x0]
  00187026: str r0,[sp,#0x10]
  00187028: beq 0x00187058
  0018702a: mov r0,r5
  0018702c: movs r1,#0x0
  0018702e: blx 0x00076234
  00187032: mov r0,r9
  00187034: blx 0x00073954
  00187038: mov r4,r0
  0018703a: cbz r0,0x00187058
  0018703c: movs r6,#0x0
  0018703e: b 0x00187052
  00187040: ldr r0,[r4,#0x4]
  00187042: ldr.w r8,[r0,r6,lsl #0x2]
  00187046: mov r0,r5
  00187048: blx 0x000718d8
  0018704c: cmp r8,r0
  0018704e: beq 0x0018706e
  00187050: adds r6,#0x1
  00187052: ldr r0,[r4,#0x0]
  00187054: cmp r6,r0
  00187056: bcc 0x00187040
  00187058: ldr r0,[sp,#0x10]
  0018705a: ldr.w r1,[r10,#0x0]
  0018705e: subs r0,r1,r0
  00187060: ittt eq
  00187062: add.eq sp,#0x14
  00187064: pop.eq.w {r8,r9,r10,r11}
  00187068: pop.eq {r4,r5,r6,r7,pc}
  0018706a: blx 0x0006e824
  0018706e: ldr.w r0,[r9,#0x0]
  00187072: ldr r0,[r0,#0x4]
  00187074: ldr.w r1,[r0,r6,lsl #0x2]
  00187078: sub.w r1,r1,r11
  0018707c: str.w r1,[r0,r6,lsl #0x2]
  00187080: mov r0,r5
  00187082: blx 0x00071944
  00187086: ldr.w r2,[r9,#0x4]
  0018708a: mla r0,r0,r11,r2
  0018708e: ldr.w r1,[r9,#0x10]
  00187092: str.w r0,[r9,#0x4]
  00187096: ldr r0,[sp,#0x0]
  00187098: cmp r0,#0x0
  0018709a: blt 0x00187058
  0018709c: cmp r1,#0x0
  0018709e: bge 0x00187058
  001870a0: ldr r0,[0x0018712c]
  001870a2: ldr r5,[sp,#0x0]
  001870a4: add r0,pc
  001870a6: str.w r5,[r9,#0x10]
  001870aa: ldr r4,[r0,#0x0]
  001870ac: ldr r0,[r4,#0x0]
  001870ae: blx 0x00071c14
  001870b2: blx 0x00071824
  001870b6: cmp r0,r5
  001870b8: bne 0x001870dc
  001870ba: ldr r0,[r4,#0x0]
  001870bc: blx 0x00071c14
  001870c0: add r4,sp,#0x4
  001870c2: mov r1,r0
  001870c4: mov r0,r4
  001870c6: blx 0x000736a8
  001870ca: add.w r0,r9,#0x14
  001870ce: mov r1,r4
  001870d0: blx 0x0006f2b0
  001870d4: add r0,sp,#0x4
  001870d6: blx 0x0006ee30
  001870da: b 0x00187058
  001870dc: ldr r0,[0x00187130]
  001870de: mov r1,r5
  001870e0: add r0,pc
  001870e2: ldr r0,[r0,#0x0]
  001870e4: ldr r0,[r0,#0x0]
  001870e6: blx 0x00071c44
  001870ea: add r4,sp,#0x4
  001870ec: mov r5,r0
  001870ee: mov r1,r5
  001870f0: mov r0,r4
  001870f2: blx 0x000736a8
  001870f6: add.w r0,r9,#0x14
  001870fa: mov r1,r4
  001870fc: blx 0x0006f2b0
  00187100: add r0,sp,#0x4
  00187102: blx 0x0006ee30
  00187106: cmp r5,#0x0
  00187108: beq 0x00187058
  0018710a: mov r0,r5
  0018710c: blx 0x0007360c
  00187110: blx 0x0006eb48
  00187114: b 0x00187058
```
