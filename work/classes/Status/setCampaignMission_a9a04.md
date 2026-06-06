# Status::setCampaignMission
elf 0xa9a04 body 64
Sig: undefined __thiscall setCampaignMission(Status * this, Mission * param_1)

## decompile
```c

/* Status::setCampaignMission(Mission*) */

void __thiscall Status::setCampaignMission(Status *this,Mission *param_1)

{
  int *piVar1;
  int *piVar2;
  
  Mission::setCampaignMission(SUB41(param_1,0));
  piVar2 = *(int **)(*(int *)(this + 0x198) + 4);
  piVar1 = (int *)*piVar2;
  if ((piVar1 != (int *)0x0) && (piVar1 != (int *)**(undefined4 **)(DAT_000b9a44 + 0xb9a24))) {
    (**(code **)(*piVar1 + 4))();
    **(undefined4 **)(*(int *)(this + 0x198) + 4) = 0;
    piVar2 = *(int **)(*(int *)(this + 0x198) + 4);
  }
  *piVar2 = (int)param_1;
  return;
}

```
## target disasm
```
  000b9a04: push {r4,r5,r7,lr}
  000b9a06: add r7,sp,#0x8
  000b9a08: mov r5,r0
  000b9a0a: mov r4,r1
  000b9a0c: mov r0,r1
  000b9a0e: movs r1,#0x1
  000b9a10: blx 0x00073828
  000b9a14: ldr.w r0,[r5,#0x198]
  000b9a18: ldr r1,[r0,#0x4]
  000b9a1a: ldr r0,[r1,#0x0]
  000b9a1c: cbz r0,0x000b9a40
  000b9a1e: ldr r2,[0x000b9a44]
  000b9a20: add r2,pc
  000b9a22: ldr r2,[r2,#0x0]
  000b9a24: ldr r2,[r2,#0x0]
  000b9a26: cmp r0,r2
  000b9a28: beq 0x000b9a40
  000b9a2a: ldr r1,[r0,#0x0]
  000b9a2c: ldr r1,[r1,#0x4]
  000b9a2e: blx r1
  000b9a30: ldr.w r0,[r5,#0x198]
  000b9a34: movs r1,#0x0
  000b9a36: ldr r0,[r0,#0x4]
  000b9a38: str r1,[r0,#0x0]
  000b9a3a: ldr.w r0,[r5,#0x198]
  000b9a3e: ldr r1,[r0,#0x4]
  000b9a40: str r4,[r1,#0x0]
  000b9a42: pop {r4,r5,r7,pc}
```
