# GameText::getText
elf 0x81818 body 224
Sig: undefined __stdcall getText(int param_1)

## decompile
```c

/* GameText::getText(int) */

String * GameText::getText(int param_1)

{
  int iVar1;
  int in_r1;
  char *pcVar2;
  uint uVar3;
  String *pSVar4;
  byte *pbVar5;
  
  if (in_r1 == 5000) {
    pbVar5 = (byte *)(DAT_00091910 + 0x91872);
    pSVar4 = (String *)(DAT_00091910 + 0x9187a);
    DataMemoryBarrier(0x1b);
    if ((*pbVar5 & 1) != 0) {
      return pSVar4;
    }
    iVar1 = __cxa_guard_acquire(pbVar5);
    if (iVar1 == 0) {
      return pSVar4;
    }
    pcVar2 = (char *)(DAT_0009191c + 0x91898);
    if (**(short **)(DAT_00091914 + 0x91890) != 1) {
      pcVar2 = (char *)(DAT_00091918 + 0x91894);
    }
    AbyssEngine::String::String(pSVar4,pcVar2,false);
  }
  else {
    if (in_r1 != 0x1389) {
      uVar3 = 0;
      do {
        if (*(uint *)param_1 <= uVar3) {
LAB_000918dc:
          if ((((-1 < in_r1) && (in_r1 < *(int *)(param_1 + 0x1c))) &&
              (*(int *)(param_1 + 0xc) != 0)) &&
             (pSVar4 = *(String **)(*(int *)(param_1 + 0xc) + in_r1 * 4), pSVar4 != (String *)0x0))
          {
            return pSVar4;
          }
          return (String *)(param_1 + 0x10);
        }
        if (*(int *)(*(int *)(param_1 + 4) + uVar3 * 4) == in_r1) {
          in_r1 = *(int *)(*(int *)(param_1 + 4) + uVar3 * 4 + 4);
          goto LAB_000918dc;
        }
        uVar3 = uVar3 + 2;
      } while( true );
    }
    pSVar4 = (String *)(DAT_00091928 + 0x91846);
    DataMemoryBarrier(0x1b);
    if ((*(byte *)(DAT_00091928 + 0x91836) & 1) != 0) {
      return pSVar4;
    }
    pbVar5 = (byte *)(DAT_00091928 + 0x91836);
    iVar1 = __cxa_guard_acquire(pbVar5);
    if (iVar1 == 0) {
      return pSVar4;
    }
    pcVar2 = (char *)(DAT_00091934 + 0x9185c);
    if (**(short **)(DAT_0009192c + 0x91854) != 1) {
      pcVar2 = (char *)(DAT_00091930 + 0x91858);
    }
    AbyssEngine::String::String(pSVar4,pcVar2,false);
  }
  __cxa_atexit(*(undefined4 *)(DAT_00091920 + 0x918b0),pSVar4,DAT_00091924 + 0x918b2);
  __cxa_guard_release(pbVar5);
  return pSVar4;
}

```

## target disasm
```
  00091818: push {r4,r5,r7,lr}
  0009181a: add r7,sp,#0x8
  0009181c: movw r2,#0x1388
  00091820: cmp r1,r2
  00091822: beq 0x0009186c
  00091824: movw r2,#0x1389
  00091828: cmp r1,r2
  0009182a: bne 0x000918be
  0009182c: ldr r0,[0x00091928]
  0009182e: add r0,pc
  00091830: add.w r4,r0,#0x14
  00091834: ldrb r1,[r0,#0x4]
  00091836: dmb #0x1b
  0009183a: tst r1,#0x1
  0009183e: bne 0x000918f4
  00091840: adds r5,r0,#0x4
  00091842: mov r0,r5
  00091844: blx 0x00070228
  00091848: cmp r0,#0x0
  0009184a: beq 0x000918f4
  0009184c: ldr r0,[0x0009192c]
  0009184e: ldr r2,[0x00091930]
  00091850: add r0,pc
  00091852: ldr r1,[0x00091934]
  00091854: add r2,pc
  00091856: ldr r0,[r0,#0x0]
  00091858: add r1,pc
  0009185a: ldrh r0,[r0,#0x0]
  0009185c: cmp r0,#0x1
  0009185e: it ne
  00091860: mov.ne r1,r2
  00091862: mov r0,r4
  00091864: movs r2,#0x0
  00091866: blx 0x0006ee18
  0009186a: b 0x000918a6
  0009186c: ldr r5,[0x00091910]
  0009186e: add r5,pc
  00091870: add.w r4,r5,#0x8
  00091874: ldrb r0,[r5,#0x0]
  00091876: dmb #0x1b
  0009187a: tst r0,#0x1
  0009187e: bne 0x000918f4
  00091880: mov r0,r5
  00091882: blx 0x00070228
  00091886: cbz r0,0x000918f4
  00091888: ldr r0,[0x00091914]
  0009188a: ldr r2,[0x00091918]
  0009188c: add r0,pc
  0009188e: ldr r1,[0x0009191c]
  00091890: add r2,pc
  00091892: ldr r0,[r0,#0x0]
  00091894: add r1,pc
  00091896: ldrh r0,[r0,#0x0]
  00091898: cmp r0,#0x1
  0009189a: it ne
  0009189c: mov.ne r1,r2
  0009189e: mov r0,r4
  000918a0: movs r2,#0x0
  000918a2: blx 0x0006ee18
  000918a6: ldr r0,[0x00091920]
  000918a8: mov r1,r4
  000918aa: ldr r2,[0x00091924]
  000918ac: add r0,pc
  000918ae: add r2,pc
  000918b0: ldr r0,[r0,#0x0]
  000918b2: blx 0x0006e80c
  000918b6: mov r0,r5
  000918b8: blx 0x00070234
  000918bc: b 0x000918f4
  000918be: ldr r3,[r0,#0x0]
  000918c0: movs r2,#0x0
  000918c2: b 0x000918d0
  000918c4: ldr r5,[r0,#0x4]
  000918c6: ldr.w r4,[r5,r2,lsl #0x2]
  000918ca: cmp r4,r1
  000918cc: beq 0x000918d6
  000918ce: adds r2,#0x2
  000918d0: cmp r2,r3
  000918d2: bcc 0x000918c4
  000918d4: b 0x000918dc
  000918d6: add.w r1,r5,r2, lsl #0x2
  000918da: ldr r1,[r1,#0x4]
  000918dc: cmp r1,#0x0
  000918de: blt 0x000918f0
  000918e0: ldr r2,[r0,#0x1c]
  000918e2: cmp r1,r2
  000918e4: bge 0x000918f0
  000918e6: ldr r2,[r0,#0xc]
  000918e8: cbz r2,0x000918f0
  000918ea: ldr.w r4,[r2,r1,lsl #0x2]
  000918ee: cbnz r4,0x000918f4
  000918f0: add.w r4,r0,#0x10
  000918f4: mov r0,r4
  000918f6: pop {r4,r5,r7,pc}
```
