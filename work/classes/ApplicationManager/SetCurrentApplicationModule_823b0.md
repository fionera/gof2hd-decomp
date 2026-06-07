# ApplicationManager::SetCurrentApplicationModule
elf 0x823b0 body 126
Sig: undefined __stdcall SetCurrentApplicationModule(uint param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SetCurrentApplicationModule(unsigned int) */

void AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uint param_1)

{
  int in_r1;
  uint uVar1;
  undefined4 uVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  bool bVar8;
  
  pcVar7 = *(char **)(DAT_00092430 + 0x923be);
  if (*pcVar7 != '\0') {
    puVar3 = *(uint **)(DAT_00092434 + 0x923ce);
    uVar4 = **(uint **)(DAT_00092438 + 0x923d0);
    uVar6 = (*(uint **)(DAT_00092438 + 0x923d0))[1];
    uVar1 = *puVar3 + 1;
    uVar5 = puVar3[1] + (uint)(0xfffffffe < *puVar3);
    *puVar3 = uVar1;
    puVar3[1] = uVar5;
    bVar8 = uVar1 < uVar4;
    if ((int)((uVar5 - uVar6) - (uint)bVar8) < 0 ==
        (SBORROW4(uVar5,uVar6) != SBORROW4(uVar5 - uVar6,(uint)bVar8))) {
      **(undefined1 **)(DAT_0009243c + 0x923f4) = 1;
      *pcVar7 = '\0';
    }
  }
  uVar1 = 0;
  while( true ) {
    if (*(uint *)(param_1 + 0x50) <= uVar1) {
      return;
    }
    if (*(int *)(*(int *)(param_1 + 0x54) + uVar1 * 4) == in_r1) break;
    uVar1 = uVar1 + 1;
  }
  uVar2 = *(undefined4 *)(*(int *)(param_1 + 0x48) + uVar1 * 4);
  *(uint *)(param_1 + 0x3c) = (uint)(*(int *)(param_1 + 0x18) != 0);
  *(int *)(param_1 + 0x5c) = in_r1;
  *(undefined4 *)(param_1 + 0x60) = uVar2;
  return;
}

```

## target disasm
```
  000923b0: push {r4,r5,r6,r7,lr}
  000923b2: add r7,sp,#0xc
  000923b4: push.w r11
  000923b8: ldr r2,[0x00092430]
  000923ba: add r2,pc
  000923bc: ldr.w r12,[r2,#0x0]
  000923c0: ldrb.w r3,[r12,#0x0]
  000923c4: cbz r3,0x000923fc
  000923c6: ldr r3,[0x00092434]
  000923c8: ldr r4,[0x00092438]
  000923ca: add r3,pc
  000923cc: add r4,pc
  000923ce: ldr r3,[r3,#0x0]
  000923d0: ldr r4,[r4,#0x0]
  000923d2: ldrd r2,lr,[r3,#0x0]
  000923d6: ldrd r4,r6,[r4,#0x0]
  000923da: adds r2,#0x1
  000923dc: adc r5,lr,#0x0
  000923e0: strd r2,r5,[r3,#0x0]
  000923e4: subs r2,r2,r4
  000923e6: sbcs.w r2,r5,r6
  000923ea: blt 0x000923fc
  000923ec: ldr r2,[0x0009243c]
  000923ee: movs r3,#0x1
  000923f0: add r2,pc
  000923f2: ldr r2,[r2,#0x0]
  000923f4: strb r3,[r2,#0x0]
  000923f6: movs r2,#0x0
  000923f8: strb.w r2,[r12,#0x0]
  000923fc: ldr r3,[r0,#0x50]
  000923fe: movs r2,#0x0
  00092400: b 0x0009240e
  00092402: ldr r6,[r0,#0x54]
  00092404: ldr.w r6,[r6,r2,lsl #0x2]
  00092408: cmp r6,r1
  0009240a: beq 0x00092414
  0009240c: adds r2,#0x1
  0009240e: cmp r2,r3
  00092410: bcc 0x00092402
  00092412: b 0x00092428
  00092414: ldr r6,[r0,#0x48]
  00092416: ldr r3,[r0,#0x18]
  00092418: ldr.w r2,[r6,r2,lsl #0x2]
  0009241c: cmp r3,#0x0
  0009241e: it ne
  00092420: mov.ne r3,#0x1
  00092422: str r3,[r0,#0x3c]
  00092424: strd r1,r2,[r0,#0x5c]
  00092428: pop.w r11
  0009242c: pop {r4,r5,r6,r7,pc}
```
