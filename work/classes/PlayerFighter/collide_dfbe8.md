# PlayerFighter::collide
elf 0xdfbe8 body 80
Sig: undefined __thiscall collide(PlayerFighter * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFighter::collide(float, float, float) */

undefined4 __thiscall
PlayerFighter::collide(PlayerFighter *this,float param_1,float param_2,float param_3)

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
      iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 8))(param_1,param_2,param_3);
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
  000efbe8: push {r4,r5,r6,r7,lr}
  000efbea: add r7,sp,#0xc
  000efbec: push {r8,r9,r10}
  000efbf0: mov r4,r0
  000efbf2: ldr.w r0,[r0,#0x88]
  000efbf6: mov r8,r3
  000efbf8: mov r9,r2
  000efbfa: subs r0,#0x3
  000efbfc: mov r10,r1
  000efbfe: cmp r0,#0x2
  000efc00: bcc 0x000efc2c
  000efc02: ldr.w r0,[r4,#0x150]
  000efc06: cbz r0,0x000efc2c
  000efc08: movs r5,#0x0
  000efc0a: b 0x000efc26
  000efc0c: ldr r0,[r0,#0x4]
  000efc0e: mov r2,r9
  000efc10: mov r3,r8
  000efc12: ldr.w r0,[r0,r5,lsl #0x2]
  000efc16: ldr r1,[r0,#0x0]
  000efc18: ldr r6,[r1,#0x8]
  000efc1a: mov r1,r10
  000efc1c: blx r6
  000efc1e: cbnz r0,0x000efc34
  000efc20: ldr.w r0,[r4,#0x150]
  000efc24: adds r5,#0x1
  000efc26: ldr r1,[r0,#0x0]
  000efc28: cmp r5,r1
  000efc2a: bcc 0x000efc0c
  000efc2c: movs r0,#0x0
  000efc2e: pop.w {r8,r9,r10}
  000efc32: pop {r4,r5,r6,r7,pc}
  000efc34: movs r0,#0x1
  000efc36: b 0x000efc2e
```
