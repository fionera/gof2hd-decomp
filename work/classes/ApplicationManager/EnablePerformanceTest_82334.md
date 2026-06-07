# ApplicationManager::EnablePerformanceTest
elf 0x82334 body 94
Sig: undefined __stdcall EnablePerformanceTest(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::EnablePerformanceTest(int) */

void AbyssEngine::ApplicationManager::EnablePerformanceTest(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int *piVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  
  iVar1 = DAT_000923a8;
  puVar3 = (undefined4 *)(DAT_00092398 + 0x9234a);
  puVar2 = *(undefined4 **)(DAT_00092394 + 0x92346);
  puVar4 = (undefined4 *)(DAT_000923a0 + 0x92350);
  puVar7 = *(undefined1 **)(DAT_0009239c + 0x9234c);
  puVar5 = (undefined4 *)(DAT_000923a4 + 0x9235c);
  *puVar2 = 0;
  puVar2[1] = 0;
  puVar8 = (undefined1 *)*puVar4;
  piVar6 = *(int **)(iVar1 + 0x92368);
  puVar3 = (undefined4 *)*puVar3;
  puVar5 = (undefined4 *)*puVar5;
  puVar2 = *(undefined4 **)(DAT_000923ac + 0x9236e);
  *piVar6 = param_1;
  piVar6[1] = param_1 >> 0x1f;
  *puVar2 = 0;
  puVar2[1] = 0;
  *puVar5 = 0;
  puVar5[1] = 0;
  *puVar3 = 0;
  puVar3[1] = 0;
  *puVar7 = 1;
  *puVar8 = 0;
  return;
}

```

## target disasm
```
  00092334: push {r4,r5,r6,r7,lr}
  00092336: add r7,sp,#0xc
  00092338: push.w r11
  0009233c: ldr r1,[0x00092394]
  0009233e: ldr r2,[0x00092398]
  00092340: ldr r3,[0x0009239c]
  00092342: add r1,pc
  00092344: ldr r4,[0x000923a0]
  00092346: add r2,pc
  00092348: add r3,pc
  0009234a: ldr r1,[r1,#0x0]
  0009234c: add r4,pc
  0009234e: ldr r5,[0x000923a4]
  00092350: ldr.w r12,[r3,#0x0]
  00092354: movs r3,#0x0
  00092356: ldr r6,[0x000923a8]
  00092358: add r5,pc
  0009235a: strd r3,r3,[r1,#0x0]
  0009235e: asrs r1,r0,#0x1f
  00092360: ldr.w lr,[r4,#0x0]
  00092364: add r6,pc
  00092366: ldr r4,[0x000923ac]
  00092368: ldr r6,[r6,#0x0]
  0009236a: add r4,pc
  0009236c: ldr r2,[r2,#0x0]
  0009236e: ldr r5,[r5,#0x0]
  00092370: ldr r4,[r4,#0x0]
  00092372: strd r0,r1,[r6,#0x0]
  00092376: movs r0,#0x1
  00092378: strd r3,r3,[r4,#0x0]
  0009237c: strd r3,r3,[r5,#0x0]
  00092380: strd r3,r3,[r2,#0x0]
  00092384: strb.w r0,[r12,#0x0]
  00092388: strb.w r3,[lr,#0x0]
  0009238c: pop.w r11
  00092390: pop {r4,r5,r6,r7,pc}
```
