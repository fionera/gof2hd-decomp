# PlayerEgo::shake
elf 0xa2134 body 158
Sig: undefined __thiscall shake(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::shake(int) */

void __thiscall PlayerEgo::shake(PlayerEgo *this,int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int *piVar4;
  code *pcVar5;
  uint in_fpscr;
  float fVar6;
  int iVar7;
  undefined4 uVar8;
  float fVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  
  fVar6 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  piVar4 = this->field_8;
  fVar9 = (float)VectorSignedToFloat((int)this[8].field_14 << 1,(byte)(in_fpscr >> 0x16) & 3);
  puVar2 = *(undefined4 **)(DAT_000b21d8 + 0xb216a);
  pcVar5 = *(code **)(DAT_000b21dc + 0xb216c);
  iVar7 = (int)((fVar6 / DAT_000b21d4) * fVar9);
  if (iVar7 < 2) {
    iVar7 = 1;
  }
  iVar3 = iVar7 << 1;
  iVar1 = (*pcVar5)(*puVar2,iVar3);
  uVar10 = VectorSignedToFloat(iVar1 - iVar7,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = (*pcVar5)(*puVar2,iVar3);
  uVar11 = VectorSignedToFloat(iVar1 - iVar7,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = (*pcVar5)(*puVar2,iVar3);
  uVar8 = VectorSignedToFloat(iVar1 - iVar7,(byte)(in_fpscr >> 0x16) & 3);
  (*(code *)(DAT_001abc84 + 0x1abc88))(piVar4,uVar10,uVar11,uVar8);
  return;
}

```

## target disasm
```
  000b2134: push {r4,r5,r6,r7,lr}
  000b2136: add r7,sp,#0xc
  000b2138: push {r8,r9,r11}
  000b213c: vpush {d8,d9}
  000b2140: vmov s0,r1
  000b2144: vldr.32 s2,[pc,#0x8c]
  000b2148: vcvt.f32.s32 s0,s0
  000b214c: ldr.w r8,[r0,#0x8]
  000b2150: ldr.w r0,[r0,#0x134]
  000b2154: lsls r0,r0,#0x1
  000b2156: vdiv.f32 s0,s0,s2
  000b215a: vmov s2,r0
  000b215e: vcvt.f32.s32 s2,s2
  000b2162: ldr r0,[0x000b21d8]
  000b2164: ldr r1,[0x000b21dc]
  000b2166: add r0,pc
  000b2168: add r1,pc
  000b216a: ldr r4,[r0,#0x0]
  000b216c: ldr.w r9,[r1,#0x0]
  000b2170: vmul.f32 s0,s0,s2
  000b2174: vcvt.s32.f32 s0,s0
  000b2178: ldr r0,[r4,#0x0]
  000b217a: vmov r6,s0
  000b217e: cmp r6,#0x1
  000b2180: it le
  000b2182: mov.le r6,#0x1
  000b2184: lsls r5,r6,#0x1
  000b2186: mov r1,r5
  000b2188: blx r9
  000b218a: subs r1,r0,r6
  000b218c: ldr r0,[r4,#0x0]
  000b218e: vmov s0,r1
  000b2192: mov r1,r5
  000b2194: vcvt.f32.s32 s16,s0
  000b2198: blx r9
  000b219a: subs r1,r0,r6
  000b219c: ldr r0,[r4,#0x0]
  000b219e: vmov s0,r1
  000b21a2: mov r1,r5
  000b21a4: vcvt.f32.s32 s18,s0
  000b21a8: blx r9
  000b21aa: subs r0,r0,r6
  000b21ac: vmov r1,s16
  000b21b0: vmov r2,s18
  000b21b4: vmov s0,r0
  000b21b8: mov r0,r8
  000b21ba: vcvt.f32.s32 s0,s0
  000b21be: vmov r3,s0
  000b21c2: vpop {d8,d9}
  000b21c6: pop.w {r8,r9,r11}
  000b21ca: pop.w {r4,r5,r6,r7,lr}
  000b21ce: b.w 0x001abc78
```
