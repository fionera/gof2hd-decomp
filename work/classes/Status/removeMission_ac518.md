# Status::removeMission
elf 0xac518 body 150
Sig: undefined __stdcall removeMission(Mission * param_1)

## decompile
```c

/* Status::removeMission(Mission*) */

void Status::removeMission(Mission *param_1)

{
  uint uVar1;
  int iVar2;
  Agent *this;
  int *piVar3;
  undefined4 *puVar4;
  Mission *in_r1;
  Mission *this_00;
  int iVar5;
  uint uVar6;
  undefined4 *puVar7;
  
  iVar5 = 0;
  puVar7 = *(undefined4 **)(DAT_000bc5b0 + 0xbc52e);
  for (uVar6 = 0; uVar6 < **(uint **)(param_1 + 0x198); uVar6 = uVar6 + 1) {
    uVar1 = (*(uint **)(param_1 + 0x198))[1];
    if (*(Mission **)(uVar1 + iVar5) == in_r1) {
      this_00 = in_r1;
      if (*(Mission **)(param_1 + 0x194) == in_r1) {
        in_r1 = (Mission *)0x0;
        *(undefined4 *)(param_1 + 0x194) = 0;
        this_00 = *(Mission **)(uVar1 + iVar5);
      }
      iVar2 = Mission::getAgent(this_00);
      if (iVar2 != 0) {
        this = (Agent *)Mission::getAgent(*(Mission **)
                                           (*(int *)(*(int *)(param_1 + 0x198) + 4) + iVar5));
        Agent::setMission(this,(Mission *)0x0);
      }
      piVar3 = *(int **)(*(int *)(*(int *)(param_1 + 0x198) + 4) + iVar5);
      if (piVar3 == (int *)0x0) {
        puVar4 = (undefined4 *)(*(int *)(*(int *)(param_1 + 0x198) + 4) + uVar6 * 4);
      }
      else {
        (**(code **)(*piVar3 + 4))();
        puVar4 = (undefined4 *)(*(int *)(*(int *)(param_1 + 0x198) + 4) + iVar5);
      }
      *puVar4 = 0;
      *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x198) + 4) + iVar5) = *puVar7;
    }
    iVar5 = iVar5 + 4;
  }
  return;
}

```
## target disasm
```
  000bc518: push {r4,r5,r6,r7,lr}
  000bc51a: add r7,sp,#0xc
  000bc51c: push {r8,r9,r10}
  000bc520: mov r4,r0
  000bc522: ldr r0,[0x000bc5b0]
  000bc524: mov.w r8,#0x0
  000bc528: movs r5,#0x0
  000bc52a: add r0,pc
  000bc52c: movs r6,#0x0
  000bc52e: ldr.w r9,[r0,#0x0]
  000bc532: b 0x000bc59e
  000bc534: ldr r0,[r0,#0x4]
  000bc536: ldr r2,[r0,r5]
  000bc538: cmp r2,r1
  000bc53a: bne 0x000bc59a
  000bc53c: ldr.w r2,[r4,#0x194]
  000bc540: mov r10,r1
  000bc542: cmp r2,r1
  000bc544: bne 0x000bc550
  000bc546: mov.w r10,#0x0
  000bc54a: str.w r10,[r4,#0x194]
  000bc54e: ldr r1,[r0,r5]
  000bc550: mov r0,r1
  000bc552: blx 0x00073438
  000bc556: cbz r0,0x000bc56a
  000bc558: ldr.w r0,[r4,#0x198]
  000bc55c: ldr r0,[r0,#0x4]
  000bc55e: ldr r0,[r0,r5]
  000bc560: blx 0x00073438
  000bc564: movs r1,#0x0
  000bc566: blx 0x00071b78
  000bc56a: ldr.w r0,[r4,#0x198]
  000bc56e: ldr r1,[r0,#0x4]
  000bc570: ldr r0,[r1,r5]
  000bc572: cbz r0,0x000bc584
  000bc574: ldr r1,[r0,#0x0]
  000bc576: ldr r1,[r1,#0x4]
  000bc578: blx r1
  000bc57a: ldr.w r0,[r4,#0x198]
  000bc57e: ldr r0,[r0,#0x4]
  000bc580: add r0,r5
  000bc582: b 0x000bc588
  000bc584: add.w r0,r1,r6, lsl #0x2
  000bc588: str.w r8,[r0,#0x0]
  000bc58c: ldr.w r0,[r4,#0x198]
  000bc590: ldr.w r1,[r9,#0x0]
  000bc594: ldr r0,[r0,#0x4]
  000bc596: str r1,[r0,r5]
  000bc598: mov r1,r10
  000bc59a: adds r5,#0x4
  000bc59c: adds r6,#0x1
  000bc59e: ldr.w r0,[r4,#0x198]
  000bc5a2: ldr r2,[r0,#0x0]
  000bc5a4: cmp r6,r2
  000bc5a6: bcc 0x000bc534
  000bc5a8: pop.w {r8,r9,r10}
  000bc5ac: pop {r4,r5,r6,r7,pc}
```
