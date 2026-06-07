# PlayerFixedObject::collide
elf 0x154d4a body 140
Sig: undefined __thiscall collide(PlayerFixedObject * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFixedObject::collide(float, float, float) */

undefined4 __thiscall
PlayerFixedObject::collide(PlayerFixedObject *this,float param_1,float param_2,float param_3)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  
  puVar1 = *(uint **)(this + 300);
  if (((puVar1 != (uint *)0x0) || (*(int *)(this + 0x88) != 4)) &&
     (this[0x8c] != (PlayerFixedObject)0x0)) {
    if ((puVar1 == (uint *)0x0) || (*(int *)(this + 0x88) != 4)) {
      puVar1 = *(uint **)(this + 0x128);
      if (puVar1 != (uint *)0x0) {
        for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
          iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 8))(param_1,param_2,param_3);
          if (iVar2 != 0) {
            return 1;
          }
          puVar1 = *(uint **)(this + 0x128);
          param_1 = extraout_s0;
          param_2 = extraout_s1;
          param_3 = extraout_s2;
        }
      }
    }
    else {
      for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
        iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 8))();
        if (iVar2 != 0) {
          return 1;
        }
        puVar1 = *(uint **)(this + 300);
      }
    }
  }
  return 0;
}

```

## target disasm
```
  00164d4a: push {r4,r5,r6,r7,lr}
  00164d4c: add r7,sp,#0xc
  00164d4e: push {r8,r9,r10}
  00164d52: mov r4,r0
  00164d54: ldr.w r0,[r0,#0x12c]
  00164d58: mov r8,r3
  00164d5a: mov r9,r2
  00164d5c: mov r10,r1
  00164d5e: cbnz r0,0x00164d68
  00164d60: ldr.w r1,[r4,#0x88]
  00164d64: cmp r1,#0x4
  00164d66: beq 0x00164dca
  00164d68: ldrb.w r1,[r4,#0x8c]
  00164d6c: cbz r1,0x00164dca
  00164d6e: cbz r0,0x00164da0
  00164d70: ldr.w r1,[r4,#0x88]
  00164d74: cmp r1,#0x4
  00164d76: bne 0x00164da0
  00164d78: movs r5,#0x0
  00164d7a: b 0x00164d82
  00164d7c: ldr.w r0,[r4,#0x12c]
  00164d80: adds r5,#0x1
  00164d82: ldr r1,[r0,#0x0]
  00164d84: cmp r5,r1
  00164d86: bcs 0x00164dca
  00164d88: ldr r0,[r0,#0x4]
  00164d8a: mov r2,r9
  00164d8c: mov r3,r8
  00164d8e: ldr.w r0,[r0,r5,lsl #0x2]
  00164d92: ldr r1,[r0,#0x0]
  00164d94: ldr r6,[r1,#0x8]
  00164d96: mov r1,r10
  00164d98: blx r6
  00164d9a: cmp r0,#0x0
  00164d9c: beq 0x00164d7c
  00164d9e: b 0x00164dd2
  00164da0: ldr.w r0,[r4,#0x128]
  00164da4: cbz r0,0x00164dca
  00164da6: movs r5,#0x0
  00164da8: b 0x00164dc4
  00164daa: ldr r0,[r0,#0x4]
  00164dac: mov r2,r9
  00164dae: mov r3,r8
  00164db0: ldr.w r0,[r0,r5,lsl #0x2]
  00164db4: ldr r1,[r0,#0x0]
  00164db6: ldr r6,[r1,#0x8]
  00164db8: mov r1,r10
  00164dba: blx r6
  00164dbc: cbnz r0,0x00164dd2
  00164dbe: ldr.w r0,[r4,#0x128]
  00164dc2: adds r5,#0x1
  00164dc4: ldr r1,[r0,#0x0]
  00164dc6: cmp r5,r1
  00164dc8: bcc 0x00164daa
  00164dca: movs r0,#0x0
  00164dcc: pop.w {r8,r9,r10}
  00164dd0: pop {r4,r5,r6,r7,pc}
  00164dd2: movs r0,#0x1
  00164dd4: b 0x00164dcc
```
