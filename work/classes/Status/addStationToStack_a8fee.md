# Status::addStationToStack
elf 0xa8fee body 130
Sig: undefined __thiscall addStationToStack(Status * this, Station * param_1)

## decompile
```c

/* Status::addStationToStack(Station*) */

undefined4 __thiscall Status::addStationToStack(Status *this,Station *param_1)

{
  int iVar1;
  void *pvVar2;
  int *piVar3;
  int iVar4;
  
  iVar1 = isOnStack(this,param_1);
  if (iVar1 != 0) {
    setStation((Station *)this);
    return 0;
  }
  piVar3 = *(int **)(*(int *)(this + 0x1a0) + 4);
  if (*piVar3 == 0) {
    iVar1 = 0;
    do {
      iVar4 = iVar1;
      if (iVar4 + 2 < 0) {
        return 0;
      }
      iVar1 = iVar4 + -1;
    } while (piVar3[iVar4 + 2] != 0);
    piVar3[iVar4 + 2] = (int)param_1;
  }
  else {
    piVar3 = piVar3 + 2;
    if ((Station *)*piVar3 != (Station *)0x0) {
      pvVar2 = (void *)Station::~Station((Station *)*piVar3);
      operator_delete(pvVar2);
      piVar3 = (int *)(*(int *)(*(int *)(this + 0x1a0) + 4) + 8);
    }
    *piVar3 = 0;
    for (iVar1 = 0; iVar1 != -2; iVar1 = iVar1 + -1) {
      iVar4 = *(int *)(*(int *)(this + 0x1a0) + 4) + iVar1 * 4;
      *(undefined4 *)(iVar4 + 8) = *(undefined4 *)(iVar4 + 4);
    }
    **(undefined4 **)(*(int *)(this + 0x1a0) + 4) = param_1;
  }
  setStation((Station *)this);
  return 1;
}

```
## target disasm
```
  000b8fee: push {r4,r5,r7,lr}
  000b8ff0: add r7,sp,#0x8
  000b8ff2: mov r5,r1
  000b8ff4: mov r4,r0
  000b8ff6: blx 0x00073660
  000b8ffa: mov r1,r0
  000b8ffc: cbz r0,0x000b9008
  000b8ffe: mov r0,r4
  000b9000: blx 0x0007366c
  000b9004: movs r0,#0x0
  000b9006: pop {r4,r5,r7,pc}
  000b9008: ldr.w r0,[r4,#0x1a0]
  000b900c: ldr r1,[r0,#0x4]
  000b900e: ldr r0,[r1,#0x0]
  000b9010: cbz r0,0x000b904a
  000b9012: ldr.w r0,[r1,#0x8]!
  000b9016: cbz r0,0x000b902a
  000b9018: blx 0x0007360c
  000b901c: blx 0x0006eb48
  000b9020: ldr.w r0,[r4,#0x1a0]
  000b9024: ldr r0,[r0,#0x4]
  000b9026: add.w r1,r0,#0x8
  000b902a: movs r0,#0x0
  000b902c: str r0,[r1,#0x0]
  000b902e: b 0x000b903c
  000b9030: ldr r1,[r1,#0x4]
  000b9032: add.w r1,r1,r0, lsl #0x2
  000b9036: subs r0,#0x1
  000b9038: ldr r2,[r1,#0x4]
  000b903a: str r2,[r1,#0x8]
  000b903c: ldr.w r1,[r4,#0x1a0]
  000b9040: adds r2,r0,#0x2
  000b9042: bne 0x000b9030
  000b9044: ldr r0,[r1,#0x4]
  000b9046: str r5,[r0,#0x0]
  000b9048: b 0x000b9064
  000b904a: movs r0,#0x0
  000b904c: adds r2,r0,#0x2
  000b904e: cmp r2,#0x0
  000b9050: blt 0x000b9004
  000b9052: add.w r2,r1,r0, lsl #0x2
  000b9056: subs r0,#0x1
  000b9058: ldr r2,[r2,#0x8]
  000b905a: cmp r2,#0x0
  000b905c: bne 0x000b904c
  000b905e: add.w r0,r1,r0, lsl #0x2
  000b9062: str r5,[r0,#0xc]
  000b9064: mov r0,r4
  000b9066: mov r1,r5
  000b9068: blx 0x0007366c
  000b906c: movs r0,#0x1
  000b906e: pop {r4,r5,r7,pc}
```
