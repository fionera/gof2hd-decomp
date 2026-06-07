# Engine::DrawQuad
elf 0x868f0 body 118
Sig: undefined __stdcall DrawQuad(int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::Engine::DrawQuad(int, int, int, int) */

void AbyssEngine::Engine::DrawQuad(int param_1,int param_2,int param_3,int param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined8 *puVar7;
  uint in_fpscr;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int in_stack_00000000;
  
  uVar4 = DAT_00096980;
  uVar3 = DAT_00096978;
  uVar2 = DAT_00096970;
  uVar1 = DAT_00096968;
  uVar8 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  uVar9 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
  iVar5 = *(int *)(param_1 + 0x380);
  uVar10 = VectorSignedToFloat(param_2 + param_4,(byte)(in_fpscr >> 0x16) & 3);
  puVar6 = *(undefined4 **)(iVar5 + 4);
  uVar11 = VectorSignedToFloat(in_stack_00000000 + param_3,(byte)(in_fpscr >> 0x16) & 3);
  *puVar6 = uVar8;
  puVar6[1] = uVar9;
  puVar6[3] = uVar10;
  puVar6[4] = uVar9;
  puVar6[6] = uVar10;
  puVar6[7] = uVar11;
  puVar6[9] = uVar8;
  puVar6[10] = uVar11;
  puVar7 = *(undefined8 **)(iVar5 + 8);
  *puVar7 = uVar1;
  puVar7[1] = uVar2;
  puVar7[2] = uVar3;
  puVar7[3] = uVar4;
  (*(code *)(DAT_001ab6e4 + 0x1ab6e8))
            (4,*(undefined2 *)(iVar5 + 0x28),0x1403,*(undefined4 *)(iVar5 + 0x2c));
  return;
}

```

## target disasm
```
  000968f0: vmov s0,r1
  000968f4: add r1,r3
  000968f6: ldr.w r12,[sp,#0x0]
  000968fa: vmov s2,r2
  000968fe: vmov s4,r1
  00096902: adr r3,[0x96968]
  00096904: add.w r1,r12,r2
  00096908: vcvt.f32.s32 s0,s0
  0009690c: vcvt.f32.s32 s2,s2
  00096910: ldr.w r0,[r0,#0x380]
  00096914: vmov s6,r1
  00096918: vld1.64 {d16,d17},[r3]
  0009691c: vcvt.f32.s32 s4,s4
  00096920: adr r2,[0x96978]
  00096922: ldr r1,[r0,#0x4]
  00096924: vcvt.f32.s32 s6,s6
  00096928: vld1.64 {d18,d19},[r2]
  0009692c: movw r2,#0x1403
  00096930: vstr.32 s0,[r1]
  00096934: vstr.32 s2,[r1,#0x4]
  00096938: vstr.32 s4,[r1,#0xc]
  0009693c: vstr.32 s2,[r1,#0x10]
  00096940: vstr.32 s4,[r1,#0x18]
  00096944: vstr.32 s6,[r1,#0x1c]
  00096948: vstr.32 s0,[r1,#0x24]
  0009694c: vstr.32 s6,[r1,#0x28]
  00096950: ldr r1,[r0,#0x8]
  00096952: vst1.32 {d16,d17},[r1]!
  00096956: vst1.32 {d18,d19},[r1]
  0009695a: ldr r3,[r0,#0x2c]
  0009695c: ldrh r1,[r0,#0x28]
  0009695e: movs r0,#0x4
  00096960: b.w 0x001ab6d8
  001ab6d8: bx pc
```
