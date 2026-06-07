# DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat
elf 0x1680e0 body 166
Sig: undefined __thiscall pickGermanGenericTextBecauseWeSaved100EurosWithThat(DialogueWindow * this, int param_1, Agent * param_2)

## decompile
```c

/* DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat(int, Agent*) */

undefined4 __thiscall
DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat
          (DialogueWindow *this,int param_1,Agent *param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar1 = Agent::getRace(param_2);
  if (iVar1 < 10) {
    iVar1 = Agent::getRace(param_2);
    iVar2 = Agent::isMale(param_2);
    if (iVar1 == 3) {
      iVar1 = Agent::getImageParts(param_2);
      if (iVar1 == 0) {
        iVar1 = 3;
      }
      else {
        piVar3 = (int *)Agent::getImageParts(param_2);
        iVar1 = 0;
        if (*piVar3 == 2) {
          iVar1 = 3;
        }
      }
    }
  }
  else {
    iVar2 = Agent::isMale(param_2);
    iVar1 = 10;
  }
  piVar3 = *(int **)(DAT_00178188 + 0x178136);
  if (param_1 == 2) {
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar3);
    iVar6 = DAT_00178190 + 0x178156;
  }
  else if (param_1 == 0) {
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar3);
    iVar6 = DAT_0017818c + 0x178148;
  }
  else {
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar3);
    iVar6 = DAT_00178194 + 0x178164;
  }
  puVar5 = (undefined4 *)(iVar6 + iVar4 * 4 + 0x48);
  if (iVar2 != 0) {
    puVar5 = (undefined4 *)(iVar6 + iVar1 * 8 + iVar4 * 4);
  }
  return *puVar5;
}

```

## target disasm
```
  001780e0: push {r4,r5,r6,r7,lr}
  001780e2: add r7,sp,#0xc
  001780e4: push.w r8
  001780e8: mov r0,r2
  001780ea: mov r4,r2
  001780ec: mov r6,r1
  001780ee: blx 0x00071bd8
  001780f2: cmp r0,#0xa
  001780f4: bge 0x00178124
  001780f6: mov r0,r4
  001780f8: blx 0x00071bd8
  001780fc: mov r5,r0
  001780fe: mov r0,r4
  00178100: blx 0x00074110
  00178104: mov r8,r0
  00178106: cmp r5,#0x3
  00178108: bne 0x0017812e
  0017810a: mov r0,r4
  0017810c: blx 0x00071c74
  00178110: cbz r0,0x00178182
  00178112: mov r0,r4
  00178114: blx 0x00071c74
  00178118: ldr r0,[r0,#0x0]
  0017811a: movs r5,#0x0
  0017811c: cmp r0,#0x2
  0017811e: it eq
  00178120: mov.eq r5,#0x3
  00178122: b 0x0017812e
  00178124: mov r0,r4
  00178126: blx 0x00074110
  0017812a: mov r8,r0
  0017812c: movs r5,#0xa
  0017812e: ldr r0,[0x00178188]
  00178130: cmp r6,#0x2
  00178132: add r0,pc
  00178134: ldr r0,[r0,#0x0]
  00178136: beq 0x00178148
  00178138: cbnz r6,0x00178156
  0017813a: ldr r0,[r0,#0x0]
  0017813c: movs r1,#0x2
  0017813e: blx 0x00071848
  00178142: ldr r2,[0x0017818c]
  00178144: add r2,pc
  00178146: b 0x00178162
  00178148: ldr r0,[r0,#0x0]
  0017814a: movs r1,#0x2
  0017814c: blx 0x00071848
  00178150: ldr r2,[0x00178190]
  00178152: add r2,pc
  00178154: b 0x00178162
  00178156: ldr r0,[r0,#0x0]
  00178158: movs r1,#0x2
  0017815a: blx 0x00071848
  0017815e: ldr r2,[0x00178194]
  00178160: add r2,pc
  00178162: add.w r1,r2,r5, lsl #0x3
  00178166: cmp.w r8,#0x0
  0017816a: add.w r1,r1,r0, lsl #0x2
  0017816e: add.w r0,r2,r0, lsl #0x2
  00178172: add.w r0,r0,#0x48
  00178176: it ne
  00178178: mov.ne r0,r1
  0017817a: ldr r0,[r0,#0x0]
  0017817c: pop.w r8
  00178180: pop {r4,r5,r6,r7,pc}
  00178182: movs r5,#0x3
  00178184: b 0x0017812e
```
