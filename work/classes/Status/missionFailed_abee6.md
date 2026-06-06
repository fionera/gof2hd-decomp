# Status::missionFailed
elf 0xabee6 body 80
Sig: undefined __thiscall missionFailed(Status * this, bool param_1, longlong param_2)

## decompile
```c

/* Status::missionFailed(bool, long long) */

int __thiscall Status::missionFailed(Status *this,bool param_1,longlong param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = 0;
  while( true ) {
    if (**(uint **)(this + 0x198) <= uVar3) {
      return 0;
    }
    iVar2 = *(int *)((*(uint **)(this + 0x198))[1] + uVar3 * 4);
    iVar1 = Mission::hasFailed();
    if (iVar1 != 0) break;
    if (((iVar2 != 0) && (iVar1 = Mission::getType(), iVar1 == 0xd && param_1)) &&
       (this[0xf1] != (Status)0x0)) {
      return iVar2;
    }
    uVar3 = uVar3 + 1;
  }
  return 0;
}

```
## target disasm
```
  000bbee6: push {r4,r5,r6,r7,lr}
  000bbee8: add r7,sp,#0xc
  000bbeea: push.w r8
  000bbeee: eor r8,r1,#0x1
  000bbef2: mov r4,r0
  000bbef4: movs r6,#0x0
  000bbef6: b 0x000bbf22
  000bbef8: ldr r0,[r0,#0x4]
  000bbefa: ldr.w r5,[r0,r6,lsl #0x2]
  000bbefe: mov r0,r5
  000bbf00: blx 0x00073924
  000bbf04: cbnz r0,0x000bbf2c
  000bbf06: cbz r5,0x000bbf20
  000bbf08: mov r0,r5
  000bbf0a: blx 0x00072874
  000bbf0e: subs r0,#0xd
  000bbf10: it ne
  000bbf12: mov.ne r0,#0x1
  000bbf14: orrs.w r0,r0,r8
  000bbf18: bne 0x000bbf20
  000bbf1a: ldrb.w r0,[r4,#0xf1]
  000bbf1e: cbnz r0,0x000bbf2e
  000bbf20: adds r6,#0x1
  000bbf22: ldr.w r0,[r4,#0x198]
  000bbf26: ldr r1,[r0,#0x0]
  000bbf28: cmp r6,r1
  000bbf2a: bcc 0x000bbef8
  000bbf2c: movs r5,#0x0
  000bbf2e: mov r0,r5
  000bbf30: pop.w r8
  000bbf34: pop {r4,r5,r6,r7,pc}
```
