# PlayerFighter::outerCollide
elf 0xdfc38 body 80
Sig: undefined __thiscall outerCollide(PlayerFighter * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFighter::outerCollide(float, float, float) */

undefined4 __thiscall
PlayerFighter::outerCollide(PlayerFighter *this,float param_1,float param_2,float param_3)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  
  if ((1 < *(int *)(this + 0x88) - 3U) && (puVar1 = *(uint **)(this + 0x150), puVar1 != (uint *)0x0)
     ) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0xc))(param_1,param_2,param_3);
      if (iVar2 != 0) {
        return 1;
      }
      puVar1 = *(uint **)(this + 0x150);
      param_1 = extraout_s0;
      param_2 = extraout_s1;
      param_3 = extraout_s2;
    }
  }
  return 0;
}

```

## target disasm
```
  000efc38: push {r4,r5,r6,r7,lr}
  000efc3a: add r7,sp,#0xc
  000efc3c: push {r8,r9,r10}
  000efc40: mov r4,r0
  000efc42: ldr.w r0,[r0,#0x88]
  000efc46: mov r8,r3
  000efc48: mov r9,r2
  000efc4a: subs r0,#0x3
  000efc4c: mov r10,r1
  000efc4e: cmp r0,#0x2
  000efc50: bcc 0x000efc7c
  000efc52: ldr.w r0,[r4,#0x150]
  000efc56: cbz r0,0x000efc7c
  000efc58: movs r5,#0x0
  000efc5a: b 0x000efc76
  000efc5c: ldr r0,[r0,#0x4]
  000efc5e: mov r2,r9
  000efc60: mov r3,r8
  000efc62: ldr.w r0,[r0,r5,lsl #0x2]
  000efc66: ldr r1,[r0,#0x0]
  000efc68: ldr r6,[r1,#0xc]
  000efc6a: mov r1,r10
  000efc6c: blx r6
  000efc6e: cbnz r0,0x000efc84
  000efc70: ldr.w r0,[r4,#0x150]
  000efc74: adds r5,#0x1
  000efc76: ldr r1,[r0,#0x0]
  000efc78: cmp r5,r1
  000efc7a: bcc 0x000efc5c
  000efc7c: movs r0,#0x0
  000efc7e: pop.w {r8,r9,r10}
  000efc82: pop {r4,r5,r6,r7,pc}
  000efc84: movs r0,#0x1
  000efc86: b 0x000efc7e
```
