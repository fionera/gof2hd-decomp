# Objective::achieved
elf 0x97d40 body 832
Sig: undefined __stdcall achieved(int param_1)

## decompile
```c

/* Objective::achieved(int) */

uint Objective::achieved(int param_1)

{
  char cVar1;
  uint *puVar2;
  uint uVar3;
  Route *this;
  int *piVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int in_r1;
  int iVar8;
  uint uVar9;
  bool bVar10;
  bool bVar11;
  
  puVar2 = (uint *)Level::getEnemies();
  uVar3 = 0;
  switch(*(undefined4 *)param_1) {
  case 0:
    iVar7 = Level::getEnemiesLeft(*(Level **)(param_1 + 0xc));
    goto LAB_000a7f94;
  case 1:
    iVar7 = Level::getEnemies();
    uVar3 = (*(code *)(DAT_001ab934 + 0x1ab938))
                      (*(undefined4 *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4));
    return uVar3;
  case 2:
    this = (Route *)Level::getPlayerRoute();
    iVar7 = Route::getLastWaypoint(this);
    cVar1 = *(char *)(iVar7 + 0x130);
    goto LAB_000a7fec;
  case 3:
    bVar11 = SBORROW4(*(int *)(param_1 + 4),in_r1);
    bVar10 = *(int *)(param_1 + 4) - in_r1 < 0;
    goto LAB_000a8052;
  case 4:
    iVar7 = Level::getMessages();
    uVar5 = *(undefined4 *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4);
    goto LAB_000a7fca;
  case 5:
    iVar7 = Level::getFriendsLeft(*(Level **)(param_1 + 0xc));
    goto LAB_000a7f94;
  case 7:
    iVar7 = 0;
    for (iVar8 = 0; iVar6 = *(int *)(param_1 + 4), iVar8 < iVar6; iVar8 = iVar8 + 1) {
      iVar6 = KIPlayer::isDead();
      iVar7 = iVar7 + iVar6;
    }
    goto LAB_000a7f92;
  case 8:
    puVar2 = (uint *)Level::getAsteroids(*(Level **)(param_1 + 0xc));
    iVar6 = 0;
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      iVar7 = KIPlayer::isDead();
      iVar6 = iVar6 + iVar7;
    }
    iVar8 = *(int *)(param_1 + 4);
    bVar11 = SBORROW4(iVar6,iVar8);
    iVar7 = iVar6 - iVar8;
    bVar10 = iVar6 == iVar8;
    goto LAB_000a7e9a;
  case 9:
    puVar2 = (uint *)Level::getAsteroids(*(Level **)(param_1 + 0xc));
    uVar3 = 0xffffffff;
    do {
      uVar3 = uVar3 + 1;
      if (*puVar2 <= uVar3) goto LAB_000a7ec8;
      iVar7 = *(int *)(param_1 + 4);
    } while ((int)uVar3 < iVar7);
    bVar11 = SBORROW4(iVar7,1);
    bVar10 = iVar7 + -1 < 0;
    goto LAB_000a8052;
  case 10:
LAB_000a7ec8:
    puVar2 = (uint *)Level::getAsteroids(*(Level **)(param_1 + 0xc));
    uVar3 = 0xffffffff;
    do {
      uVar3 = uVar3 + 1;
      if (*puVar2 <= uVar3) goto switchD_000a7d60_caseD_b;
    } while ((int)uVar3 < *(int *)(param_1 + 4));
LAB_000a803c:
    uVar3 = 0;
    break;
  case 0xb:
switchD_000a7d60_caseD_b:
    iVar7 = Level::getEnemies();
    cVar1 = *(char *)(*(int *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4) + 0x69);
    goto LAB_000a7fec;
  case 0xc:
    iVar7 = Level::getEnemies();
    cVar1 = *(char *)(*(int *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4) + 0x68);
    goto LAB_000a7fec;
  case 0xf:
    iVar7 = Level::getEnemies();
    uVar3 = (*(code *)(DAT_001ab944 + 0x1ab948))
                      (*(undefined4 *)
                        (*(int *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4) + 4));
    return uVar3;
  case 0x10:
    uVar9 = 0;
    do {
      if (*puVar2 <= uVar9) {
        return 1;
      }
      iVar7 = uVar9 * 4;
      uVar9 = uVar9 + 1;
      uVar3 = 0;
    } while (*(char *)(*(int *)(puVar2[1] + iVar7) + 0x69) != '\0');
    break;
  case 0x11:
    uVar9 = 0;
    do {
      if (*puVar2 <= uVar9) goto LAB_000a803c;
      iVar7 = uVar9 * 4;
      uVar9 = uVar9 + 1;
      uVar3 = 1;
    } while (*(char *)(*(int *)(puVar2[1] + iVar7) + 0x68) == '\0');
    break;
  case 0x12:
    iVar7 = 0;
    for (iVar6 = *(int *)(param_1 + 4); iVar6 < *(int *)(param_1 + 8); iVar6 = iVar6 + 1) {
      iVar8 = KIPlayer::isDead();
      iVar7 = iVar7 + iVar8;
    }
    iVar6 = *(int *)(param_1 + 8) - *(int *)(param_1 + 4);
LAB_000a7f92:
    iVar7 = iVar7 - iVar6;
LAB_000a7f94:
    uVar3 = (uint)(iVar7 == 0);
    break;
  case 0x13:
    uVar3 = (*(code *)(DAT_001ab954 + 0x1ab958))(*(undefined4 *)(param_1 + 0xc));
    return uVar3;
  case 0x14:
  case 0x15:
    iVar6 = 0;
    for (iVar7 = *(int *)(param_1 + 4); iVar7 < *(int *)(param_1 + 8); iVar7 = iVar7 + 1) {
      iVar8 = KIPlayer::isDead();
      if ((iVar8 != 0) && (*(int *)(*(int *)(puVar2[1] + iVar7 * 4) + 0x28) == 8)) {
        iVar6 = iVar6 + 1;
      }
    }
    iVar7 = *(int *)(param_1 + 8) - *(int *)(param_1 + 4);
    if (*(int *)param_1 != 0x14) {
      if (iVar6 != iVar7) {
        return 0;
      }
      return (uint)(*(int *)(*(int *)(param_1 + 0xc) + 0x24) <=
                   *(int *)(*(int *)(param_1 + 0xc) + 0x20));
    }
    if (iVar6 != iVar7) {
      return 0;
    }
    iVar8 = *(int *)(*(int *)(param_1 + 0xc) + 0x20);
    iVar6 = *(int *)(*(int *)(param_1 + 0xc) + 0x24);
    bVar11 = SBORROW4(iVar6,iVar8);
    iVar7 = iVar6 - iVar8;
    bVar10 = iVar6 == iVar8;
LAB_000a7e9a:
    uVar3 = (uint)(!bVar10 && iVar7 < 0 == bVar11);
    break;
  case 0x16:
    iVar7 = Level::getMessages();
    piVar4 = (int *)Level::getMessages();
    uVar5 = *(undefined4 *)(*(int *)(iVar7 + 4) + *piVar4 * 4 + -4);
LAB_000a7fca:
    uVar3 = (*(code *)(DAT_001ab964 + 0x1ab968))(uVar5);
    return uVar3;
  case 0x17:
    iVar7 = Level::getEnemies();
    cVar1 = *(char *)(*(int *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4) + 0x24);
LAB_000a7fec:
    uVar3 = (uint)(cVar1 != '\0');
    break;
  case 0x19:
    iVar7 = Level::getEnemies();
    uVar3 = (uint)(*(float *)(*(int *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4) + 100) ==
                  0.0);
    break;
  case 0x1a:
    iVar7 = *(int *)(param_1 + 4);
    do {
      if (*(int *)(param_1 + 8) <= iVar7) goto LAB_000a803c;
      iVar6 = KIPlayer::isDead();
      iVar7 = iVar7 + 1;
      uVar3 = 1;
    } while (iVar6 == 0);
    break;
  case 0x1b:
    iVar7 = Level::getEnemiesLeft(*(Level **)(param_1 + 0xc));
    bVar11 = SBORROW4(iVar7,*(int *)(param_1 + 4));
    bVar10 = iVar7 - *(int *)(param_1 + 4) < 0;
LAB_000a8052:
    uVar3 = (uint)(bVar10 != bVar11);
    break;
  case 0x1c:
    iVar7 = Level::getNumDeliveredOre(*(Level **)(param_1 + 0xc));
    goto LAB_000a806a;
  case 0x1d:
    iVar7 = Level::getNumDeliveredPassengers();
LAB_000a806a:
    uVar3 = (uint)(*(int *)(param_1 + 4) <= iVar7);
    break;
  case 0x1e:
    iVar7 = Level::getEnemies();
    uVar3 = (*(code *)(DAT_001ab974 + 0x1ab978))
                      (*(undefined4 *)(*(int *)(iVar7 + 4) + *(int *)(param_1 + 4) * 4));
    return uVar3;
  }
  return uVar3;
}

```

## target disasm
```
  000a7d40: push {r4,r5,r6,r7,lr}
  000a7d42: add r7,sp,#0xc
  000a7d44: push.w r8
  000a7d48: mov r8,r0
  000a7d4a: ldr r0,[r0,#0xc]
  000a7d4c: mov r6,r1
  000a7d4e: blx 0x00071eb4
  000a7d52: ldr.w r1,[r8,#0x0]
  000a7d56: mov r5,r0
  000a7d58: movs r0,#0x0
  000a7d5a: cmp r1,#0x1e
  000a7d5c: bhi.w 0x000a80ae
  000a7d60: tbh [pc,r1]
  000a7da2: ldr.w r4,[r8,#0x4]
  000a7da6: movs r6,#0x0
  000a7da8: b 0x000a7dc6
  000a7daa: ldr r0,[r5,#0x4]
  000a7dac: ldr.w r0,[r0,r4,lsl #0x2]
  000a7db0: blx 0x00071ec0
  000a7db4: cbz r0,0x000a7dc4
  000a7db6: ldr r0,[r5,#0x4]
  000a7db8: ldr.w r0,[r0,r4,lsl #0x2]
  000a7dbc: ldr r0,[r0,#0x28]
  000a7dbe: cmp r0,#0x8
  000a7dc0: it eq
  000a7dc2: add.eq r6,#0x1
  000a7dc4: adds r4,#0x1
  000a7dc6: ldr.w r0,[r8,#0x8]
  000a7dca: cmp r4,r0
  000a7dcc: blt 0x000a7daa
  000a7dce: ldrd r2,r1,[r8,#0x0]
  000a7dd2: subs r1,r0,r1
  000a7dd4: movs r0,#0x0
  000a7dd6: cmp r2,#0x14
  000a7dd8: bne.w 0x000a8098
  000a7ddc: cmp r6,r1
  000a7dde: bne.w 0x000a80ae
  000a7de2: ldr.w r1,[r8,#0xc]
  000a7de6: ldrd r2,r1,[r1,#0x20]
  000a7dea: cmp r1,r2
  000a7dec: b 0x000a7e9a
  000a7dee: ldr.w r0,[r8,#0xc]
  000a7df2: blx 0x00071ecc
  000a7df6: b 0x000a7f94
  000a7df8: ldr.w r0,[r8,#0xc]
  000a7dfc: blx 0x00071eb4
  000a7e00: ldr.w r1,[r8,#0x4]
  000a7e04: ldr r0,[r0,#0x4]
  000a7e06: ldr.w r0,[r0,r1,lsl #0x2]
  000a7e0a: pop.w r8
  000a7e0e: pop.w {r4,r5,r6,r7,lr}
  000a7e12: b.w 0x001ab928
  000a7e16: ldr.w r0,[r8,#0xc]
  000a7e1a: blx 0x00071ed8
  000a7e1e: blx 0x00071ee4
  000a7e22: ldrb.w r0,[r0,#0x130]
  000a7e26: b 0x000a7fec
  000a7e28: ldr.w r1,[r8,#0x4]
  000a7e2c: movs r0,#0x0
  000a7e2e: cmp r1,r6
  000a7e30: b 0x000a8052
  000a7e32: ldr.w r0,[r8,#0xc]
  000a7e36: blx 0x00071ef0
  000a7e3a: ldr.w r1,[r8,#0x4]
  000a7e3e: ldr r0,[r0,#0x4]
  000a7e40: ldr.w r0,[r0,r1,lsl #0x2]
  000a7e44: b 0x000a7fca
  000a7e46: ldr.w r0,[r8,#0xc]
  000a7e4a: blx 0x00071efc
  000a7e4e: b 0x000a7f94
  000a7e50: movs r4,#0x0
  000a7e52: movs r6,#0x0
  000a7e54: b 0x000a7e64
  000a7e56: ldr r0,[r5,#0x4]
  000a7e58: ldr.w r0,[r0,r6,lsl #0x2]
  000a7e5c: blx 0x00071ec0
  000a7e60: add r4,r0
  000a7e62: adds r6,#0x1
  000a7e64: ldr.w r0,[r8,#0x4]
  000a7e68: cmp r6,r0
  000a7e6a: blt 0x000a7e56
  000a7e6c: b 0x000a7f92
  000a7e6e: ldr.w r0,[r8,#0xc]
  000a7e72: blx 0x00071f08
  000a7e76: mov r5,r0
  000a7e78: movs r4,#0x0
  000a7e7a: movs r6,#0x0
  000a7e7c: b 0x000a7e8c
  000a7e7e: ldr r0,[r5,#0x4]
  000a7e80: ldr.w r0,[r0,r6,lsl #0x2]
  000a7e84: blx 0x00071ec0
  000a7e88: add r4,r0
  000a7e8a: adds r6,#0x1
  000a7e8c: ldr r0,[r5,#0x0]
  000a7e8e: cmp r6,r0
  000a7e90: bcc 0x000a7e7e
  000a7e92: ldr.w r1,[r8,#0x4]
  000a7e96: movs r0,#0x0
  000a7e98: cmp r4,r1
  000a7e9a: it gt
  000a7e9c: mov.gt r0,#0x1
  000a7e9e: b 0x000a80ae
  000a7ea0: ldr.w r0,[r8,#0xc]
  000a7ea4: blx 0x00071f08
  000a7ea8: ldr r0,[r0,#0x0]
  000a7eaa: add.w r5,r8,#0x4
  000a7eae: mov.w r1,#0xffffffff
  000a7eb2: adds r1,#0x1
  000a7eb4: cmp r1,r0
  000a7eb6: bcs 0x000a7ec8
  000a7eb8: ldr r2,[r5,#0x0]
  000a7eba: cmp r1,r2
  000a7ebc: blt 0x000a7eb2
  000a7ebe: movs r0,#0x0
  000a7ec0: cmp r2,#0x1
  000a7ec2: b 0x000a8052
  000a7ec4: add.w r5,r8,#0x4
  000a7ec8: ldr.w r0,[r8,#0xc]
  000a7ecc: blx 0x00071f08
  000a7ed0: ldr r0,[r0,#0x0]
  000a7ed2: mov.w r1,#0xffffffff
  000a7ed6: adds r1,#0x1
  000a7ed8: cmp r1,r0
  000a7eda: bcs 0x000a7ee4
  000a7edc: ldr r2,[r5,#0x0]
  000a7ede: cmp r1,r2
  000a7ee0: blt 0x000a7ed6
  000a7ee2: b 0x000a803c
  000a7ee4: ldr.w r0,[r8,#0xc]
  000a7ee8: blx 0x00071eb4
  000a7eec: ldr.w r1,[r8,#0x4]
  000a7ef0: ldr r0,[r0,#0x4]
  000a7ef2: ldr.w r0,[r0,r1,lsl #0x2]
  000a7ef6: ldrb.w r0,[r0,#0x69]
  000a7efa: b 0x000a7fec
  000a7efc: ldr.w r0,[r8,#0xc]
  000a7f00: blx 0x00071eb4
  000a7f04: ldr.w r1,[r8,#0x4]
  000a7f08: ldr r0,[r0,#0x4]
  000a7f0a: ldr.w r0,[r0,r1,lsl #0x2]
  000a7f0e: ldrb.w r0,[r0,#0x68]
  000a7f12: b 0x000a7fec
  000a7f14: ldr.w r0,[r8,#0xc]
  000a7f18: blx 0x00071eb4
  000a7f1c: ldr.w r1,[r8,#0x4]
  000a7f20: ldr r0,[r0,#0x4]
  000a7f22: ldr.w r0,[r0,r1,lsl #0x2]
  000a7f26: ldr r0,[r0,#0x4]
  000a7f28: pop.w r8
  000a7f2c: pop.w {r4,r5,r6,r7,lr}
  000a7f30: b.w 0x001ab938
  000a7f34: ldr r1,[r5,#0x0]
  000a7f36: movs r2,#0x0
  000a7f38: cmp r2,r1
  000a7f3a: bcs.w 0x000a80ac
  000a7f3e: ldr r0,[r5,#0x4]
  000a7f40: ldr.w r0,[r0,r2,lsl #0x2]
  000a7f44: adds r2,#0x1
  000a7f46: ldrb.w r3,[r0,#0x69]
  000a7f4a: movs r0,#0x0
  000a7f4c: cmp r3,#0x0
  000a7f4e: bne 0x000a7f38
  000a7f50: b 0x000a80ae
  000a7f52: ldr r1,[r5,#0x0]
  000a7f54: movs r2,#0x0
  000a7f56: cmp r2,r1
  000a7f58: bcs 0x000a803c
  000a7f5a: ldr r0,[r5,#0x4]
  000a7f5c: ldr.w r0,[r0,r2,lsl #0x2]
  000a7f60: adds r2,#0x1
  000a7f62: ldrb.w r3,[r0,#0x68]
  000a7f66: movs r0,#0x1
  000a7f68: cmp r3,#0x0
  000a7f6a: beq 0x000a7f56
  000a7f6c: b 0x000a80ae
  000a7f6e: ldr.w r6,[r8,#0x4]
  000a7f72: movs r4,#0x0
  000a7f74: b 0x000a7f84
  000a7f76: ldr r0,[r5,#0x4]
  000a7f78: ldr.w r0,[r0,r6,lsl #0x2]
  000a7f7c: blx 0x00071ec0
  000a7f80: add r4,r0
  000a7f82: adds r6,#0x1
  000a7f84: ldr.w r0,[r8,#0x8]
  000a7f88: cmp r6,r0
  000a7f8a: blt 0x000a7f76
  000a7f8c: ldr.w r1,[r8,#0x4]
  000a7f90: subs r0,r0,r1
  000a7f92: subs r0,r4,r0
  000a7f94: clz r0,r0
  000a7f98: lsrs r0,r0,#0x5
  000a7f9a: b 0x000a80ae
  000a7f9c: ldr.w r0,[r8,#0xc]
  000a7fa0: pop.w r8
  000a7fa4: pop.w {r4,r5,r6,r7,lr}
  000a7fa8: b.w 0x001ab948
  000a7fac: ldr.w r0,[r8,#0xc]
  000a7fb0: blx 0x00071ef0
  000a7fb4: mov r5,r0
  000a7fb6: ldr.w r0,[r8,#0xc]
  000a7fba: blx 0x00071ef0
  000a7fbe: ldr r0,[r0,#0x0]
  000a7fc0: ldr r1,[r5,#0x4]
  000a7fc2: add.w r0,r1,r0, lsl #0x2
  000a7fc6: ldr.w r0,[r0,#-0x4]
  000a7fca: pop.w r8
  000a7fce: pop.w {r4,r5,r6,r7,lr}
  000a7fd2: b.w 0x001ab958
  000a7fd6: ldr.w r0,[r8,#0xc]
  000a7fda: blx 0x00071eb4
  000a7fde: ldr.w r1,[r8,#0x4]
  000a7fe2: ldr r0,[r0,#0x4]
  000a7fe4: ldr.w r0,[r0,r1,lsl #0x2]
  000a7fe8: ldrb.w r0,[r0,#0x24]
  000a7fec: cmp r0,#0x0
  000a7fee: it ne
  000a7ff0: mov.ne r0,#0x1
  000a7ff2: b 0x000a80ae
  000a7ff4: ldr.w r0,[r8,#0xc]
  000a7ff8: blx 0x00071eb4
  000a7ffc: ldr.w r1,[r8,#0x4]
  000a8000: ldr r0,[r0,#0x4]
  000a8002: ldr.w r0,[r0,r1,lsl #0x2]
  000a8006: vldr.32 s0,[r0,#0x64]
  000a800a: movs r0,#0x0
  000a800c: vcmpe.f32 s0,#0
  000a8010: vmrs apsr,fpscr
  000a8014: it eq
  000a8016: mov.eq r0,#0x1
  000a8018: b 0x000a80ae
  000a801a: ldr.w r4,[r8,#0x4]
  000a801e: ldr.w r0,[r8,#0x8]
  000a8022: cmp r4,r0
  000a8024: bge 0x000a803c
  000a8026: ldr r0,[r5,#0x4]
  000a8028: ldr.w r0,[r0,r4,lsl #0x2]
  000a802c: blx 0x00071ec0
  000a8030: adds r4,#0x1
  000a8032: mov r1,r0
  000a8034: movs r0,#0x1
  000a8036: cmp r1,#0x0
  000a8038: beq 0x000a801e
  000a803a: b 0x000a80ae
  000a803c: movs r0,#0x0
  000a803e: b 0x000a80ae
  000a8040: ldr.w r0,[r8,#0xc]
  000a8044: blx 0x00071ecc
  000a8048: ldr.w r2,[r8,#0x4]
  000a804c: mov r1,r0
  000a804e: movs r0,#0x0
  000a8050: cmp r1,r2
  000a8052: it lt
  000a8054: mov.lt r0,#0x1
  000a8056: b 0x000a80ae
  000a8058: ldr.w r0,[r8,#0xc]
  000a805c: blx 0x00071f38
  000a8060: b 0x000a806a
  000a8062: ldr.w r0,[r8,#0xc]
  000a8066: blx 0x00071f44
  000a806a: ldr.w r2,[r8,#0x4]
  000a806e: mov r1,r0
  000a8070: movs r0,#0x0
  000a8072: cmp r1,r2
  000a8074: it ge
  000a8076: mov.ge r0,#0x1
  000a8078: b 0x000a80ae
  000a807a: ldr.w r0,[r8,#0xc]
  000a807e: blx 0x00071eb4
  000a8082: ldr.w r1,[r8,#0x4]
  000a8086: ldr r0,[r0,#0x4]
  000a8088: ldr.w r0,[r0,r1,lsl #0x2]
  000a808c: pop.w r8
  000a8090: pop.w {r4,r5,r6,r7,lr}
  000a8094: b.w 0x001ab968
  000a8098: cmp r6,r1
  000a809a: bne 0x000a80ae
  000a809c: ldr.w r1,[r8,#0xc]
  000a80a0: ldrd r2,r1,[r1,#0x20]
  000a80a4: cmp r1,r2
  000a80a6: it le
  000a80a8: mov.le r0,#0x1
  000a80aa: b 0x000a80ae
  000a80ac: movs r0,#0x1
  000a80ae: pop.w r8
  000a80b2: pop {r4,r5,r6,r7,pc}
  001ab928: bx pc
  001ab938: bx pc
  001ab948: bx pc
  001ab958: bx pc
  001ab968: bx pc
```
