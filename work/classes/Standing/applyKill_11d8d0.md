# Standing::applyKill
elf 0x11d8d0 body 98
Sig: undefined __thiscall applyKill(Standing * this, int param_1)

## decompile
```c

/* Standing::applyKill(int) */

void __thiscall Standing::applyKill(Standing *this,int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  iVar1 = Status::inAlienOrbit();
  if (iVar1 == 0) {
    Status::getSystem();
    uVar2 = SolarSystem::getRace();
  }
  else {
    uVar2 = 9;
  }
  if (param_1 == 8) {
    if (-1 < *(int *)(this + 4)) {
      return;
    }
    if (uVar2 < 4) {
      uVar3 = 1;
      param_1 = *(int *)(DAT_0012d938 + 0x12d916 + uVar2 * 4);
    }
    else {
      uVar3 = 1;
      param_1 = 8;
    }
  }
  else {
    uVar3 = 5;
  }
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,uVar3);
  return;
}

```

## target disasm
```
  0012d8d0: push {r4,r5,r6,r7,lr}
  0012d8d2: add r7,sp,#0xc
  0012d8d4: push.w r11
  0012d8d8: mov r4,r0
  0012d8da: ldr r0,[0x0012d934]
  0012d8dc: mov r5,r1
  0012d8de: add r0,pc
  0012d8e0: ldr r6,[r0,#0x0]
  0012d8e2: ldr r0,[r6,#0x0]
  0012d8e4: blx 0x000723d0
  0012d8e8: cbz r0,0x0012d8ee
  0012d8ea: movs r0,#0x9
  0012d8ec: b 0x0012d8f8
  0012d8ee: ldr r0,[r6,#0x0]
  0012d8f0: blx 0x00071a10
  0012d8f4: blx 0x00071950
  0012d8f8: cmp r5,#0x8
  0012d8fa: bne 0x0012d91a
  0012d8fc: ldr r1,[r4,#0x4]
  0012d8fe: cmp.w r1,#0xffffffff
  0012d902: itt gt
  0012d904: pop.gt.w r11
  0012d908: pop.gt {r4,r5,r6,r7,pc}
  0012d90a: cmp r0,#0x3
  0012d90c: bhi 0x0012d91e
  0012d90e: ldr r1,[0x0012d938]
  0012d910: movs r2,#0x1
  0012d912: add r1,pc
  0012d914: ldr.w r5,[r1,r0,lsl #0x2]
  0012d918: b 0x0012d922
  0012d91a: movs r2,#0x5
  0012d91c: b 0x0012d922
  0012d91e: movs r2,#0x1
  0012d920: movs r5,#0x8
  0012d922: mov r0,r4
  0012d924: mov r1,r5
  0012d926: pop.w r11
  0012d92a: pop.w {r4,r5,r6,r7,lr}
  0012d92e: b.w 0x001ac3b8
```
