# Sparks::explode
elf 0x15e48c body 246
Sig: undefined __stdcall explode(int param_1, int param_2, int param_3)

## decompile
```c

/* Sparks::explode(int, int, int) */

void Sparks::explode(int param_1,int param_2,int param_3)

{
  uint *puVar1;
  undefined4 uVar2;
  int iVar3;
  int in_r3;
  uint uVar4;
  uint uVar5;
  code *pcVar6;
  undefined4 *puVar7;
  uint in_fpscr;
  float fVar8;
  float in_s1;
  float extraout_s1;
  float extraout_s2;
  float fVar9;
  
  if (*(char *)(param_1 + 0x10) == '\0') {
    if (*(int *)(param_1 + 0x14) == 1) {
      puVar1 = *(uint **)(DAT_0016e584 + 0x16e4ba);
      puVar7 = *(undefined4 **)(DAT_0016e588 + 0x16e4bc);
      pcVar6 = *(code **)(DAT_0016e58c + 0x16e4c8);
      for (uVar5 = 0; uVar5 < *(uint *)(param_1 + 0x18); uVar5 = uVar5 + 1) {
        uVar2 = *(undefined4 *)(param_1 + 4);
        uVar4 = *puVar1;
        iVar3 = (*pcVar6)(*puVar7,400);
        VectorSignedToFloat(iVar3 + param_2,(byte)(in_fpscr >> 0x16) & 3);
        iVar3 = (*pcVar6)(iVar3 + param_2,*puVar7,400);
        VectorSignedToFloat(param_3 + iVar3,(byte)(in_fpscr >> 0x16) & 3);
        iVar3 = (*pcVar6)(*puVar7,400);
        fVar8 = (float)VectorSignedToFloat(iVar3 + in_r3,(byte)(in_fpscr >> 0x16) & 3);
        AbyssEngine::PaintCanvas::SpriteSystemSetPosition
                  (uVar4,(ushort)uVar2,fVar8,extraout_s1,extraout_s2);
      }
    }
    else {
      VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      fVar8 = (float)VectorSignedToFloat(in_r3,(byte)(in_fpscr >> 0x16) & 3);
      fVar9 = (float)VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::PaintCanvas::SpriteSystemSetPosition
                (**(uint **)(DAT_0016e590 + 0x16e552),(ushort)*(undefined4 *)(param_1 + 4),fVar8,
                 in_s1,fVar9);
    }
    *(undefined1 *)(param_1 + 0x10) = 1;
  }
  return;
}

```

## target disasm
```
  0016e48c: push {r4,r5,r6,r7,lr}
  0016e48e: add r7,sp,#0xc
  0016e490: push {r7,r8,r9,r10,r11}
  0016e494: vpush {d5,d6,d7,d8}
  0016e498: mov r5,r0
  0016e49a: ldrb r0,[r0,#0x10]
  0016e49c: mov r6,r3
  0016e49e: mov r11,r1
  0016e4a0: cmp r0,#0x0
  0016e4a2: bne 0x0016e576
  0016e4a4: ldr r0,[r5,#0x14]
  0016e4a6: cmp r0,#0x1
  0016e4a8: bne 0x0016e53e
  0016e4aa: strd r6,r2,[sp,#0xc]
  0016e4ae: mov.w r8,#0x0
  0016e4b2: ldr r0,[0x0016e584]
  0016e4b4: ldr r1,[0x0016e588]
  0016e4b6: add r0,pc
  0016e4b8: add r1,pc
  0016e4ba: ldr r0,[r0,#0x0]
  0016e4bc: str r0,[sp,#0x8]
  0016e4be: ldr r0,[0x0016e58c]
  0016e4c0: ldr.w r10,[r1,#0x0]
  0016e4c4: add r0,pc
  0016e4c6: ldr.w r9,[r0,#0x0]
  0016e4ca: b 0x0016e536
  0016e4cc: ldr r0,[r5,#0x4]
  0016e4ce: mov r6,r5
  0016e4d0: str r0,[sp,#0x14]
  0016e4d2: mov.w r1,#0x190
  0016e4d6: ldr r0,[sp,#0x8]
  0016e4d8: ldr r5,[r0,#0x0]
  0016e4da: ldr.w r0,[r10,#0x0]
  0016e4de: blx r9
  0016e4e0: add r0,r11
  0016e4e2: mov.w r1,#0x190
  0016e4e6: vmov s0,r0
  0016e4ea: vcvt.f32.s32 s16,s0
  0016e4ee: ldr.w r0,[r10,#0x0]
  0016e4f2: blx r9
  0016e4f4: ldr r1,[sp,#0x10]
  0016e4f6: vmov r4,s16
  0016e4fa: add r1,r0
  0016e4fc: ldr.w r0,[r10,#0x0]
  0016e500: vmov s0,r1
  0016e504: mov.w r1,#0x190
  0016e508: vcvt.f32.s32 s16,s0
  0016e50c: blx r9
  0016e50e: ldr r1,[sp,#0xc]
  0016e510: uxth.w r2,r8
  0016e514: mov r3,r4
  0016e516: add r0,r1
  0016e518: vmov s0,r0
  0016e51c: mov r0,r5
  0016e51e: mov r5,r6
  0016e520: vcvt.f32.s32 s0,s0
  0016e524: ldr r1,[sp,#0x14]
  0016e526: vstr.32 s16,[sp]
  0016e52a: vstr.32 s0,[sp,#0x4]
  0016e52e: blx 0x000778d8
  0016e532: add.w r8,r8,#0x1
  0016e536: ldr r0,[r5,#0x18]
  0016e538: cmp r8,r0
  0016e53a: bcc 0x0016e4cc
  0016e53c: b 0x0016e572
  0016e53e: vmov s0,r11
  0016e542: vmov s2,r2
  0016e546: movs r2,#0x0
  0016e548: vcvt.f32.s32 s0,s0
  0016e54c: ldr r0,[0x0016e590]
  0016e54e: add r0,pc
  0016e550: ldr r0,[r0,#0x0]
  0016e552: vmov r3,s0
  0016e556: vmov s0,r6
  0016e55a: vcvt.f32.s32 s0,s0
  0016e55e: vcvt.f32.s32 s2,s2
  0016e562: ldr r1,[r5,#0x4]
  0016e564: ldr r0,[r0,#0x0]
  0016e566: vstr.32 s0,[sp,#0x4]
  0016e56a: vstr.32 s2,[sp]
  0016e56e: blx 0x000778d8
  0016e572: movs r0,#0x1
  0016e574: strb r0,[r5,#0x10]
  0016e576: vpop {d5,d6,d7,d8}
  0016e57a: add sp,#0x4
  0016e57c: pop.w {r8,r9,r10,r11}
  0016e580: pop {r4,r5,r6,r7,pc}
```
