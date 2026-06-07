# Station::~Station
elf 0xa69f4 body 222
Sig: undefined __thiscall ~Station(Station * this)

## decompile
```c

/* Station::~Station() */

void __thiscall Station::~Station(Station *this)

{
  void *pvVar1;
  Array<Agent*> *this_00;
  int iVar2;
  Mission *pMVar3;
  Agent *pAVar4;
  Agent *pAVar5;
  uint uVar6;
  Agent *this_01;
  
  if (*(Array **)(this + 0x2c) != (Array *)0x0) {
    ArrayReleaseClasses<Ship*>(*(Array **)(this + 0x2c));
    if (*(Array<Ship*> **)(this + 0x2c) != (Array<Ship*> *)0x0) {
      pvVar1 = (void *)Array<Ship*>::~Array(*(Array<Ship*> **)(this + 0x2c));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x2c) = 0;
  }
  if (*(Array **)(this + 0x28) != (Array *)0x0) {
    ArrayReleaseClasses<Item*>(*(Array **)(this + 0x28));
    if (*(Array<Item*> **)(this + 0x28) != (Array<Item*> *)0x0) {
      pvVar1 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x28));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x28) = 0;
  }
  this_00 = *(Array<Agent*> **)(this + 0x30);
  if (this_00 != (Array<Agent*> *)0x0) {
    for (uVar6 = 0; uVar6 < *(uint *)this_00; uVar6 = uVar6 + 1) {
      this_01 = *(Agent **)(*(int *)(this_00 + 4) + uVar6 * 4);
      iVar2 = Status::getCampaignMission();
      if (iVar2 == 0) {
        pAVar4 = (Agent *)0x0;
      }
      else {
        pMVar3 = (Mission *)Status::getCampaignMission();
        pAVar4 = (Agent *)Mission::getAgent(pMVar3);
      }
      iVar2 = Status::getFreelanceMission();
      if (iVar2 == 0) {
        pAVar5 = (Agent *)0x0;
      }
      else {
        pMVar3 = (Mission *)Status::getFreelanceMission();
        pAVar5 = (Agent *)Mission::getAgent(pMVar3);
      }
      if (((this_01 != (Agent *)0x0) && (this_01 != pAVar4 && this_01 != pAVar5)) &&
         (iVar2 = Agent::isStoryAgent(this_01), iVar2 == 0)) {
        pvVar1 = (void *)Agent::~Agent(this_01);
        operator_delete(pvVar1);
      }
      this_00 = *(Array<Agent*> **)(this + 0x30);
    }
    if (this_00 != (Array<Agent*> *)0x0) {
      pvVar1 = (void *)Array<Agent*>::~Array(this_00);
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x30) = 0;
  }
  (*(code *)(DAT_001abe94 + 0x1abe98))(this);
  return;
}

```

## target disasm
```
  000b69f4: push {r4,r5,r6,r7,lr}
  000b69f6: add r7,sp,#0xc
  000b69f8: push {r8,r9,r11}
  000b69fc: mov r8,r0
  000b69fe: ldr r0,[r0,#0x2c]
  000b6a00: cbz r0,0x000b6a1a
  000b6a02: blx 0x00073408
  000b6a06: ldr.w r0,[r8,#0x2c]
  000b6a0a: cbz r0,0x000b6a14
  000b6a0c: blx 0x00073414
  000b6a10: blx 0x0006eb48
  000b6a14: movs r0,#0x0
  000b6a16: str.w r0,[r8,#0x2c]
  000b6a1a: ldr.w r0,[r8,#0x28]
  000b6a1e: cbz r0,0x000b6a38
  000b6a20: blx 0x00073420
  000b6a24: ldr.w r0,[r8,#0x28]
  000b6a28: cbz r0,0x000b6a32
  000b6a2a: blx 0x0007342c
  000b6a2e: blx 0x0006eb48
  000b6a32: movs r0,#0x0
  000b6a34: str.w r0,[r8,#0x28]
  000b6a38: ldr.w r0,[r8,#0x30]
  000b6a3c: cmp r0,#0x0
  000b6a3e: beq 0x000b6ac4
  000b6a40: ldr r1,[0x000b6ae4]
  000b6a42: movs r4,#0x0
  000b6a44: add r1,pc
  000b6a46: ldr.w r9,[r1,#0x0]
  000b6a4a: ldr r1,[r0,#0x0]
  000b6a4c: cmp r4,r1
  000b6a4e: bcs 0x000b6ab4
  000b6a50: ldr r0,[r0,#0x4]
  000b6a52: ldr.w r5,[r0,r4,lsl #0x2]
  000b6a56: ldr.w r0,[r9,#0x0]
  000b6a5a: blx 0x0007285c
  000b6a5e: cbz r0,0x000b6a70
  000b6a60: ldr.w r0,[r9,#0x0]
  000b6a64: blx 0x0007285c
  000b6a68: blx 0x00073438
  000b6a6c: mov r6,r0
  000b6a6e: b 0x000b6a72
  000b6a70: movs r6,#0x0
  000b6a72: ldr.w r0,[r9,#0x0]
  000b6a76: blx 0x00073444
  000b6a7a: cbz r0,0x000b6a8a
  000b6a7c: ldr.w r0,[r9,#0x0]
  000b6a80: blx 0x00073444
  000b6a84: blx 0x00073438
  000b6a88: b 0x000b6a8c
  000b6a8a: movs r0,#0x0
  000b6a8c: cbz r5,0x000b6a96
  000b6a8e: cmp r5,r6
  000b6a90: it ne
  000b6a92: cmp.ne r5,r0
  000b6a94: bne 0x000b6a9e
  000b6a96: ldr.w r0,[r8,#0x30]
  000b6a9a: adds r4,#0x1
  000b6a9c: b 0x000b6a4a
  000b6a9e: mov r0,r5
  000b6aa0: blx 0x00071ba8
  000b6aa4: cmp r0,#0x0
  000b6aa6: bne 0x000b6a96
  000b6aa8: mov r0,r5
  000b6aaa: blx 0x00073450
  000b6aae: blx 0x0006eb48
  000b6ab2: b 0x000b6a96
  000b6ab4: cbz r0,0x000b6abe
  000b6ab6: blx 0x0007345c
  000b6aba: blx 0x0006eb48
  000b6abe: movs r0,#0x0
  000b6ac0: str.w r0,[r8,#0x30]
  000b6ac4: mov r0,r8
  000b6ac6: pop.w {r8,r9,r11}
  000b6aca: pop.w {r4,r5,r6,r7,lr}
  000b6ace: b.w 0x001abe88
```
