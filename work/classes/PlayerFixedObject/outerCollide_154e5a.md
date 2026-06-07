# PlayerFixedObject::outerCollide
elf 0x154e5a body 144
Sig: undefined __thiscall outerCollide(PlayerFixedObject * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFixedObject::outerCollide(float, float, float) */

undefined4 __thiscall
PlayerFixedObject::outerCollide(PlayerFixedObject *this,float param_1,float param_2,float param_3)

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
          iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0xc))(param_1,param_2,param_3);
          if (iVar2 != 0) goto LAB_00164ee2;
          puVar1 = *(uint **)(this + 0x128);
          param_1 = extraout_s0;
          param_2 = extraout_s1;
          param_3 = extraout_s2;
        }
      }
    }
    else {
      for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
        iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0xc))();
        if (iVar2 != 0) {
LAB_00164ee2:
          *(uint *)(this + 0x16c) = uVar3;
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
  00164e5a: push {r4,r5,r6,r7,lr}
  00164e5c: add r7,sp,#0xc
  00164e5e: push {r8,r9,r10}
  00164e62: mov r5,r0
  00164e64: ldr.w r0,[r0,#0x12c]
  00164e68: mov r8,r3
  00164e6a: mov r9,r2
  00164e6c: mov r10,r1
  00164e6e: cbnz r0,0x00164e78
  00164e70: ldr.w r1,[r5,#0x88]
  00164e74: cmp r1,#0x4
  00164e76: beq 0x00164eda
  00164e78: ldrb.w r1,[r5,#0x8c]
  00164e7c: cbz r1,0x00164eda
  00164e7e: cbz r0,0x00164eb0
  00164e80: ldr.w r1,[r5,#0x88]
  00164e84: cmp r1,#0x4
  00164e86: bne 0x00164eb0
  00164e88: movs r6,#0x0
  00164e8a: b 0x00164e92
  00164e8c: ldr.w r0,[r5,#0x12c]
  00164e90: adds r6,#0x1
  00164e92: ldr r1,[r0,#0x0]
  00164e94: cmp r6,r1
  00164e96: bcs 0x00164eda
  00164e98: ldr r0,[r0,#0x4]
  00164e9a: mov r2,r9
  00164e9c: mov r3,r8
  00164e9e: ldr.w r0,[r0,r6,lsl #0x2]
  00164ea2: ldr r1,[r0,#0x0]
  00164ea4: ldr r4,[r1,#0xc]
  00164ea6: mov r1,r10
  00164ea8: blx r4
  00164eaa: cmp r0,#0x0
  00164eac: beq 0x00164e8c
  00164eae: b 0x00164ee2
  00164eb0: ldr.w r0,[r5,#0x128]
  00164eb4: cbz r0,0x00164eda
  00164eb6: movs r6,#0x0
  00164eb8: b 0x00164ed4
  00164eba: ldr r0,[r0,#0x4]
  00164ebc: mov r2,r9
  00164ebe: mov r3,r8
  00164ec0: ldr.w r0,[r0,r6,lsl #0x2]
  00164ec4: ldr r1,[r0,#0x0]
  00164ec6: ldr r4,[r1,#0xc]
  00164ec8: mov r1,r10
  00164eca: blx r4
  00164ecc: cbnz r0,0x00164ee2
  00164ece: ldr.w r0,[r5,#0x128]
  00164ed2: adds r6,#0x1
  00164ed4: ldr r1,[r0,#0x0]
  00164ed6: cmp r6,r1
  00164ed8: bcc 0x00164eba
  00164eda: movs r0,#0x0
  00164edc: pop.w {r8,r9,r10}
  00164ee0: pop {r4,r5,r6,r7,pc}
  00164ee2: movs r0,#0x1
  00164ee4: str.w r6,[r5,#0x16c]
  00164ee8: b 0x00164edc
```
