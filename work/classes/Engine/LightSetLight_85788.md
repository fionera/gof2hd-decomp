# Engine::LightSetLight
elf 0x85788 body 148
Sig: undefined __thiscall LightSetLight(Engine * this, uint param_1)

## decompile
```c

/* AbyssEngine::Engine::LightSetLight(unsigned int) */

void __thiscall AbyssEngine::Engine::LightSetLight(Engine *this,uint param_1)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 local_30;
  undefined8 uStack_28;
  int local_1c;
  
  piVar2 = *(int **)(DAT_0009581c + 0x95798);
  local_1c = *piVar2;
  local_30 = *(undefined8 *)(DAT_00095820 + 0x9579e);
  uStack_28 = *(undefined8 *)(DAT_00095820 + 0x957a6);
  uVar1 = param_1 - 0x4000;
  if (uVar1 < 8) {
    puVar4 = (undefined4 *)(DAT_00095824 + 0x957c4);
    iVar3 = param_1 - 0x3fff;
    if ((int)(param_1 - 0x3fff) < *(int *)(this + 0x32c)) {
      iVar3 = *(int *)(this + 0x32c);
    }
    *(int *)(this + 0x32c) = iVar3;
    local_30 = *(undefined8 *)(this + uVar1 * 0xc + 0x468);
    uStack_28 = CONCAT44(*(undefined4 *)(this + uVar1 * 4 + 0x378),
                         *(undefined4 *)(this + uVar1 * 0xc + 0x470));
    if (*(char *)*puVar4 == '\0') {
      glLightfv(param_1,0x1203,&local_30);
    }
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00095788: push {r4,r5,r6,r7,lr}
  0009578a: add r7,sp,#0xc
  0009578c: push.w r11
  00095790: sub sp,#0x18
  00095792: ldr r2,[0x0009581c]
  00095794: add r2,pc
  00095796: ldr r4,[r2,#0x0]
  00095798: ldr r2,[0x00095820]
  0009579a: add r2,pc
  0009579c: ldr r3,[r4,#0x0]
  0009579e: vld1.64 {d16,d17},[r2]
  000957a2: mov r2,sp
  000957a4: str r3,[sp,#0x14]
  000957a6: vst1.64 {d16,d17},[r2]
  000957aa: sub.w r2,r1,#0x4000
  000957ae: cmp r2,#0x7
  000957b0: bhi 0x00095808
  000957b2: ldr r6,[0x00095824]
  000957b4: movw r12,#0x3fff
  000957b8: ldr.w r3,[r0,#0x32c]
  000957bc: sub.w r5,r1,r12
  000957c0: add r6,pc
  000957c2: cmp r3,r5
  000957c4: it gt
  000957c6: mov.gt r5,r3
  000957c8: add.w r3,r2,r2, lsl #0x1
  000957cc: str.w r5,[r0,#0x32c]
  000957d0: add.w r3,r0,r3, lsl #0x2
  000957d4: ldr.w r12,[r6,#0x0]
  000957d8: add.w r0,r0,r2, lsl #0x2
  000957dc: ldr.w r5,[r3,#0x468]
  000957e0: ldr.w r6,[r3,#0x46c]
  000957e4: ldr.w r3,[r3,#0x470]
  000957e8: str r5,[sp,#0x0]
  000957ea: ldrb.w r5,[r12,#0x0]
  000957ee: strd r6,r3,[sp,#0x4]
  000957f2: ldr.w r0,[r0,#0x378]
  000957f6: cmp r5,#0x0
  000957f8: str r0,[sp,#0xc]
  000957fa: bne 0x00095808
  000957fc: mov r0,r1
  000957fe: mov r2,sp
  00095800: movw r1,#0x1203
  00095804: blx 0x000707a4
  00095808: ldr r0,[sp,#0x14]
  0009580a: ldr r1,[r4,#0x0]
  0009580c: subs r0,r1,r0
  0009580e: ittt eq
  00095810: add.eq sp,#0x18
  00095812: pop.eq.w r11
  00095816: pop.eq {r4,r5,r6,r7,pc}
  00095818: blx 0x0006e824
```
