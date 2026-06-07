# Transform::SetAnimationRangeInTime
elf 0x74e90 body 292
Sig: undefined __stdcall SetAnimationRangeInTime(longlong param_1, longlong param_2)

## decompile
```c

/* AbyssEngine::Transform::SetAnimationRangeInTime(long long, long long) */

void AbyssEngine::Transform::SetAnimationRangeInTime(longlong param_1,longlong param_2)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint extraout_r1;
  uint extraout_r1_00;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint in_stack_00000000;
  int in_stack_00000004;
  
  iVar7 = (int)((ulonglong)param_2 >> 0x20);
  uVar9 = (uint)param_2;
  iVar3 = (int)param_1;
  uVar5 = *(uint *)(iVar3 + 0xf8);
  iVar4 = *(int *)(iVar3 + 0xfc);
  if (uVar5 != 0 || iVar4 != 0) {
    uVar8 = 0;
    bVar1 = (int)((iVar4 - in_stack_00000004) - (uint)(uVar5 < in_stack_00000000)) < 0 !=
            (SBORROW4(iVar4,in_stack_00000004) !=
            SBORROW4(iVar4 - in_stack_00000004,(uint)(uVar5 < in_stack_00000000)));
    iVar10 = in_stack_00000004;
    if (in_stack_00000004 < 0 || bVar1) {
      iVar10 = iVar4;
    }
    *(int *)(iVar3 + 0x10c) = iVar10;
    uVar6 = in_stack_00000000;
    if (in_stack_00000004 < 0 || bVar1) {
      uVar6 = uVar5;
    }
    *(uint *)(iVar3 + 0x108) = uVar6;
    bVar1 = (int)((iVar4 - iVar7) - (uint)(uVar5 < uVar9)) < 0 !=
            (SBORROW4(iVar4,iVar7) != SBORROW4(iVar4 - iVar7,(uint)(uVar5 < uVar9)));
    iVar4 = iVar7;
    if (bVar1) {
      iVar4 = 0;
    }
    if (0x7fffffffffffffff < (ulonglong)param_2) {
      iVar4 = 0;
    }
    *(int *)(iVar3 + 0x104) = iVar4;
    uVar5 = uVar9;
    if (bVar1) {
      uVar5 = 0;
    }
    if (0x7fffffffffffffff < (ulonglong)param_2) {
      uVar5 = 0;
    }
    *(uint *)(iVar3 + 0x100) = uVar5;
    uVar5 = *(uint *)(iVar3 + 0x110);
    iVar10 = *(int *)(iVar3 + 0x114);
    bVar1 = (int)((in_stack_00000004 - iVar10) - (uint)(in_stack_00000000 < uVar5)) < 0 !=
            (SBORROW4(in_stack_00000004,iVar10) !=
            SBORROW4(in_stack_00000004 - iVar10,(uint)(in_stack_00000000 < uVar5)));
    iVar4 = iVar10;
    if (bVar1) {
      iVar4 = in_stack_00000004;
    }
    bVar2 = (int)((iVar10 - iVar7) - (uint)(uVar5 < uVar9)) < 0 !=
            (SBORROW4(iVar10,iVar7) != SBORROW4(iVar10 - iVar7,(uint)(uVar5 < uVar9)));
    if (bVar2) {
      iVar4 = iVar7;
    }
    *(int *)(iVar3 + 0x114) = iVar4;
    if (bVar1) {
      uVar5 = in_stack_00000000;
    }
    if (bVar2) {
      uVar5 = uVar9;
    }
    *(uint *)(iVar3 + 0x110) = uVar5;
    for (; uVar8 < *(uint *)(iVar3 + 0x3c); uVar8 = uVar8 + 1) {
      iVar4 = *(int *)(*(int *)(iVar3 + 0x40) + uVar8 * 4);
      if ((iVar4 != 0) && (iVar4 = *(int *)(iVar4 + 0x34), iVar4 != 0)) {
        SetAnimationRangeInTime(CONCAT44(uVar5,iVar4),param_2);
        uVar5 = extraout_r1;
      }
    }
    for (uVar9 = 0; uVar9 < *(uint *)(iVar3 + 0x4c); uVar9 = uVar9 + 1) {
      SetAnimationRangeInTime
                (CONCAT44(uVar5,*(undefined4 *)(*(int *)(iVar3 + 0x50) + uVar9 * 4)),param_2);
      uVar5 = extraout_r1_00;
    }
    Update(CONCAT44(uVar5,iVar3),false);
  }
  return;
}

```

## target disasm
```
  00084e90: push {r4,r5,r6,r7,lr}
  00084e92: add r7,sp,#0xc
  00084e94: push {r6,r7,r8,r9,r10}
  00084e98: mov r4,r0
  00084e9a: ldrd r1,r0,[r0,#0xf8]
  00084e9e: mov r10,r2
  00084ea0: orrs.w r2,r1,r0
  00084ea4: beq.w 0x00084fae
  00084ea8: ldrd r9,r8,[r7,#0x8]
  00084eac: cmp.w r8,#0x0
  00084eb0: mov.w r2,#0x0
  00084eb4: mov r5,r3
  00084eb6: it lt
  00084eb8: mov.lt r2,#0x1
  00084eba: subs.w r3,r1,r9
  00084ebe: sbcs.w r3,r0,r8
  00084ec2: mov.w r6,#0x0
  00084ec6: mov.w r3,#0x0
  00084eca: it lt
  00084ecc: mov.lt r3,#0x1
  00084ece: orrs r2,r3
  00084ed0: mov r2,r8
  00084ed2: it ne
  00084ed4: mov.ne r2,r0
  00084ed6: str.w r2,[r4,#0x10c]
  00084eda: mov r2,r9
  00084edc: it ne
  00084ede: mov.ne r2,r1
  00084ee0: subs.w r1,r1,r10
  00084ee4: sbcs r0,r5
  00084ee6: str.w r2,[r4,#0x108]
  00084eea: mov.w r0,#0x0
  00084eee: mov r1,r5
  00084ef0: it lt
  00084ef2: mov.lt r0,#0x1
  00084ef4: cmp r0,#0x0
  00084ef6: it ne
  00084ef8: mov.ne r1,#0x0
  00084efa: cmp.w r5,#0xffffffff
  00084efe: it le
  00084f00: mov.le r1,r6
  00084f02: cmp r0,#0x0
  00084f04: mov r0,r10
  00084f06: str.w r1,[r4,#0x104]
  00084f0a: it ne
  00084f0c: mov.ne r0,#0x0
  00084f0e: cmp.w r5,#0xffffffff
  00084f12: it le
  00084f14: mov.le r0,r6
  00084f16: str.w r0,[r4,#0x100]
  00084f1a: ldrd r1,r12,[r4,#0x110]
  00084f1e: mov r3,r12
  00084f20: subs.w r2,r9,r1
  00084f24: sbcs.w r2,r8,r12
  00084f28: mov.w r2,#0x0
  00084f2c: it lt
  00084f2e: mov.lt r2,#0x1
  00084f30: cmp r2,#0x0
  00084f32: it ne
  00084f34: mov.ne r3,r8
  00084f36: subs.w r0,r1,r10
  00084f3a: sbcs.w r0,r12,r5
  00084f3e: mov.w r0,#0x0
  00084f42: it lt
  00084f44: mov.lt r0,#0x1
  00084f46: cmp r0,#0x0
  00084f48: it ne
  00084f4a: mov.ne r3,r5
  00084f4c: str.w r3,[r4,#0x114]
  00084f50: cmp r2,#0x0
  00084f52: it ne
  00084f54: mov.ne r1,r9
  00084f56: cmp r0,#0x0
  00084f58: it ne
  00084f5a: mov.ne r1,r10
  00084f5c: str.w r1,[r4,#0x110]
  00084f60: b 0x00084f7c
  00084f62: ldr r0,[r4,#0x40]
  00084f64: ldr.w r0,[r0,r6,lsl #0x2]
  00084f68: cbz r0,0x00084f7a
  00084f6a: ldr r0,[r0,#0x34]
  00084f6c: cbz r0,0x00084f7a
  00084f6e: mov r2,r10
  00084f70: mov r3,r5
  00084f72: strd r9,r8,[sp,#0x0]
  00084f76: blx 0x0006ebc0
  00084f7a: adds r6,#0x1
  00084f7c: ldr r0,[r4,#0x3c]
  00084f7e: cmp r6,r0
  00084f80: bcc 0x00084f62
  00084f82: movs r6,#0x0
  00084f84: b 0x00084f9a
  00084f86: ldr r0,[r4,#0x50]
  00084f88: mov r2,r10
  00084f8a: mov r3,r5
  00084f8c: ldr.w r0,[r0,r6,lsl #0x2]
  00084f90: strd r9,r8,[sp,#0x0]
  00084f94: blx 0x0006ebc0
  00084f98: adds r6,#0x1
  00084f9a: ldr r0,[r4,#0x4c]
  00084f9c: cmp r6,r0
  00084f9e: bcc 0x00084f86
  00084fa0: movs r0,#0x0
  00084fa2: movs r2,#0x0
  00084fa4: str r0,[sp,#0x0]
  00084fa6: mov r0,r4
  00084fa8: movs r3,#0x0
  00084faa: blx 0x0006f7cc
  00084fae: pop.w {r2,r3,r8,r9,r10}
  00084fb2: pop {r4,r5,r6,r7,pc}
```
