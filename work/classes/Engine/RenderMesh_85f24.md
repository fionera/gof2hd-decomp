# Engine::RenderMesh
elf 0x85f24 body 918
Sig: undefined __stdcall RenderMesh(Mesh * param_1)

## decompile
```c

/* AbyssEngine::Engine::RenderMesh(AbyssEngine::Mesh*) */

void AbyssEngine::Engine::RenderMesh(Mesh *param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  Mesh *in_r1;
  int *piVar4;
  undefined2 uVar5;
  char *pcVar6;
  code *pcVar7;
  bool bVar8;
  undefined1 auStack_20 [4];
  int local_1c;
  
  piVar4 = *(int **)(DAT_000962ac + 0x95f3a);
  local_1c = *piVar4;
  if ((in_r1 == (Mesh *)0x0) || (*(short *)(in_r1 + 0x28) == 0)) goto LAB_0009626e;
  if (**(char **)(DAT_000962bc + 0x95f50) == '\0') {
    glVertexPointer(3,0x1406,0,*(undefined4 *)(in_r1 + 4));
    AEClientState((Engine *)param_1,0x8074,true);
    if ((int)((uint)(byte)*in_r1 << 0x1e) < 0) {
      if ((*(int *)(in_r1 + 0x30) == 0) || (*(int *)(*(int *)(in_r1 + 0x30) + 4) == -1)) {
        glTexCoordPointer(2,0x1406,0,*(undefined4 *)(in_r1 + 8));
        bVar8 = true;
        goto LAB_00096036;
      }
    }
    else {
      bVar8 = false;
LAB_00096036:
      AEClientState((Engine *)param_1,0x8078,bVar8);
    }
    bVar8 = (int)((uint)(byte)*in_r1 << 0x1d) < 0;
    if (bVar8) {
      glNormalPointer(0x1406,0,*(undefined4 *)(in_r1 + 0x10));
    }
    AEClientState((Engine *)param_1,0x8075,bVar8);
    bVar8 = (int)((uint)(byte)*in_r1 << 0x1c) < 0;
    if (bVar8) {
      glColorPointer(4,0x1406,0,*(undefined4 *)(in_r1 + 0xc));
    }
    AEClientState((Engine *)param_1,0x8076,bVar8);
    if ((int)((uint)(byte)*in_r1 << 0x1b) < 0) {
      if (param_1[0xfe] == (Mesh)0x0) {
        uVar5 = *(undefined2 *)(in_r1 + 0x28);
      }
      else {
        uVar5 = *(undefined2 *)(in_r1 + 0x28);
        iVar3 = __aeabi_uidiv(uVar5,3);
        if (param_1[0xfd] == (Mesh)0x0) {
          *(int *)(param_1 + 0x58) = iVar3 + *(int *)(param_1 + 0x58);
          *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + 1;
        }
        else {
          *(int *)(param_1 + 0x5c) = iVar3 + *(int *)(param_1 + 0x5c);
          *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        }
      }
      glDrawElements(4,uVar5,0x1403,*(undefined4 *)(in_r1 + 0x2c));
    }
    else {
      glDrawArrays(4,0,*(undefined2 *)(in_r1 + 2));
    }
    if ((((int)((uint)(byte)*in_r1 << 0x1e) < 0) && (*(int *)(in_r1 + 0x30) != 0)) &&
       (*(int *)(*(int *)(in_r1 + 0x30) + 4) != -1)) {
      AEClientState((Engine *)param_1,0x8078,false);
      if (*piVar4 - local_1c == 0) {
        (*(code *)(DAT_001ab6d4 + 0x1ab6d8))();
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*piVar4 - local_1c);
    }
    goto LAB_0009626e;
  }
  pcVar6 = *(char **)(DAT_000962c0 + 0x95f5c);
  *pcVar6 = '\0';
  uVar1 = *(uint *)(param_1 + 0x420);
  if ((uVar1 & 0x1000) == 0) {
    if ((int)(uVar1 << 0x12) < 0) {
      puVar2 = (undefined4 *)(DAT_000962c8 + 0x960ca);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0x11) < 0) {
      puVar2 = (undefined4 *)(DAT_000962cc + 0x960d0);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0x15) < 0) {
      puVar2 = (undefined4 *)(DAT_000962d0 + 0x9613a);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0x1d) < 0) {
      puVar2 = (undefined4 *)(DAT_000962d4 + 0x96140);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0xf) < 0) {
      puVar2 = (undefined4 *)(DAT_000962d8 + 0x96146);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 7) < 0) {
      puVar2 = (undefined4 *)(DAT_000962dc + 0x9614c);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0xd) < 0) {
      puVar2 = (undefined4 *)(DAT_000962e0 + 0x96152);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 4) < 0) {
      puVar2 = (undefined4 *)(DAT_000962e4 + 0x96158);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 10) < 0) {
      puVar2 = (undefined4 *)(DAT_000962e8 + 0x9615e);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 9) < 0) {
      puVar2 = (undefined4 *)(DAT_000962ec + 0x96164);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0xb) < 0) {
      puVar2 = (undefined4 *)(DAT_000962f0 + 0x9616a);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 5) < 0) {
      puVar2 = (undefined4 *)(DAT_000962f4 + 0x96170);
      goto LAB_0009619c;
    }
    if ((int)(uVar1 << 0xe) < 0) {
      if (**(char **)(DAT_000962f8 + 0x96176) == '\0') {
        puVar2 = (undefined4 *)(DAT_00096300 + 0x96186);
      }
      else {
        puVar2 = (undefined4 *)(DAT_000962fc + 0x96180);
      }
      goto LAB_0009619c;
    }
    if (-1 < (int)(uVar1 << 6)) {
      if ((int)(uVar1 << 0x1c) < 0) {
        puVar2 = (undefined4 *)(DAT_00096304 + 0x9618c);
      }
      else if ((int)(uVar1 << 0xc) < 0) {
        puVar2 = (undefined4 *)(DAT_00096308 + 0x96192);
      }
      else if ((int)(uVar1 << 0x10) < 0) {
        puVar2 = (undefined4 *)(DAT_0009630c + 0x96198);
      }
      else if ((int)(uVar1 << 0x1e) < 0) {
        puVar2 = (undefined4 *)(DAT_00096310 + 0x9619e);
      }
      else if ((uVar1 & 1) == 0) {
        puVar2 = (undefined4 *)(DAT_00096328 + 0x95fe6);
      }
      else if (((byte)*in_r1 & 10) == 8) {
        puVar2 = (undefined4 *)(DAT_00096314 + 0x96292);
      }
      else if (param_1[0x424] == (Mesh)0x0) {
        if ((int)((uint)(byte)*in_r1 << 0x1c) < 0) {
          puVar2 = (undefined4 *)(DAT_0009631c + 0x962b6);
        }
        else if ((int)(uVar1 << 8) < 0) {
          puVar2 = (undefined4 *)((int)&DAT_000962bc + DAT_00096320);
        }
        else {
          puVar2 = (undefined4 *)((int)&DAT_000962ac + DAT_00096324);
        }
      }
      else {
        puVar2 = (undefined4 *)(DAT_00096318 + 0x9629e);
      }
      goto LAB_0009619c;
    }
  }
  else {
    puVar2 = (undefined4 *)(DAT_000962c4 + 0x96016);
LAB_0009619c:
    ShaderSetActive((Engine *)param_1,*(int *)*puVar2,in_r1);
  }
  if (*pcVar6 != '\0') {
    glGetIntegerv(0x8ca6,auStack_20);
    if ((int)((uint)(byte)*in_r1 << 0x1b) < 0) {
      uVar5 = *(undefined2 *)(in_r1 + 0x28);
      iVar3 = __aeabi_uidiv(uVar5,3);
      if (in_r1[0x5c] == (Mesh)0x0) {
        if (param_1[0xfd] == (Mesh)0x0) {
          *(int *)(param_1 + 0x58) = iVar3 + *(int *)(param_1 + 0x58);
          *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + 1;
        }
        else {
          *(int *)(param_1 + 0x5c) = iVar3 + *(int *)(param_1 + 0x5c);
          *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        }
        glDrawElements(4,uVar5,0x1403,*(undefined4 *)(in_r1 + 0x2c));
      }
      else {
        if (param_1[0xfd] == (Mesh)0x0) {
          *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + 1;
          *(int *)(param_1 + 0x68) = iVar3 + *(int *)(param_1 + 0x68);
        }
        else {
          *(int *)(param_1 + 0x6c) = iVar3 + *(int *)(param_1 + 0x6c);
          *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        }
        pcVar7 = *(code **)(DAT_0009632c + 0x96216);
        (*pcVar7)(0x8893,*(undefined4 *)(in_r1 + 100));
        glDrawElements(4,*(undefined2 *)(in_r1 + 0x28),0x1403,0);
        (*pcVar7)(0x8892,0);
        (*pcVar7)(0x8893,0);
      }
    }
    else {
      glDrawArrays(4,0,*(undefined2 *)(in_r1 + 2));
    }
    (**(code **)(**(int **)(*(int *)(param_1 + 0x514) + **(int **)(DAT_00096330 + 0x96262) * 4) +
                0x10))();
  }
LAB_0009626e:
  if (*piVar4 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00095f24: push {r4,r5,r6,r7,lr}
  00095f26: add r7,sp,#0xc
  00095f28: push.w r8
  00095f2c: sub sp,#0x8
  00095f2e: mov r4,r0
  00095f30: ldr r0,[0x000962ac]
  00095f32: mov r5,r1
  00095f34: cmp r1,#0x0
  00095f36: add r0,pc
  00095f38: ldr r6,[r0,#0x0]
  00095f3a: ldr r0,[r6,#0x0]
  00095f3c: str r0,[sp,#0x4]
  00095f3e: beq.w 0x0009626e
  00095f42: ldrh r0,[r5,#0x28]
  00095f44: cmp r0,#0x0
  00095f46: beq.w 0x0009626e
  00095f4a: ldr r0,[0x000962bc]
  00095f4c: add r0,pc
  00095f4e: ldr r0,[r0,#0x0]
  00095f50: ldrb r0,[r0,#0x0]
  00095f52: cmp r0,#0x0
  00095f54: beq 0x00095fe6
  00095f56: ldr r0,[0x000962c0]
  00095f58: add r0,pc
  00095f5a: ldr.w r8,[r0,#0x0]
  00095f5e: movs r0,#0x0
  00095f60: strb.w r0,[r8,#0x0]
  00095f64: ldr.w r0,[r4,#0x420]
  00095f68: tst r0,#0x1000
  00095f6c: bne 0x00096010
  00095f6e: lsls r1,r0,#0x12
  00095f70: bmi.w 0x000960c4
  00095f74: lsls r1,r0,#0x11
  00095f76: bmi.w 0x000960ca
  00095f7a: lsls r1,r0,#0x15
  00095f7c: bmi.w 0x00096134
  00095f80: lsls r1,r0,#0x1d
  00095f82: bmi.w 0x0009613a
  00095f86: lsls r1,r0,#0xf
  00095f88: bmi.w 0x00096140
  00095f8c: lsls r1,r0,#0x7
  00095f8e: bmi.w 0x00096146
  00095f92: lsls r1,r0,#0xd
  00095f94: bmi.w 0x0009614c
  00095f98: lsls r1,r0,#0x4
  00095f9a: bmi.w 0x00096152
  00095f9e: lsls r1,r0,#0xa
  00095fa0: bmi.w 0x00096158
  00095fa4: lsls r1,r0,#0x9
  00095fa6: bmi.w 0x0009615e
  00095faa: lsls r1,r0,#0xb
  00095fac: bmi.w 0x00096164
  00095fb0: lsls r1,r0,#0x5
  00095fb2: bmi.w 0x0009616a
  00095fb6: lsls r1,r0,#0xe
  00095fb8: bmi.w 0x00096170
  00095fbc: lsls r1,r0,#0x6
  00095fbe: bmi.w 0x000961a8
  00095fc2: lsls r1,r0,#0x1c
  00095fc4: bmi.w 0x00096186
  00095fc8: lsls r1,r0,#0xc
  00095fca: bmi.w 0x0009618c
  00095fce: lsls r1,r0,#0x10
  00095fd0: bmi.w 0x00096192
  00095fd4: lsls r1,r0,#0x1e
  00095fd6: bmi.w 0x00096198
  00095fda: lsls r1,r0,#0x1f
  00095fdc: bne.w 0x00096282
  00095fe0: ldr r0,[0x00096328]
  00095fe2: add r0,pc
  00095fe4: b 0x0009619c
  00095fe6: ldr r3,[r5,#0x4]
  00095fe8: movs r0,#0x3
  00095fea: movw r1,#0x1406
  00095fee: movs r2,#0x0
  00095ff0: blx 0x0006ed1c
  00095ff4: mov r0,r4
  00095ff6: movw r1,#0x8074
  00095ffa: movs r2,#0x1
  00095ffc: blx 0x0006ed10
  00096000: ldrb r0,[r5,#0x0]
  00096002: lsls r0,r0,#0x1e
  00096004: bmi 0x00096016
  00096006: mov r0,r4
  00096008: movw r1,#0x8078
  0009600c: movs r2,#0x0
  0009600e: b 0x00096036
  00096010: ldr r0,[0x000962c4]
  00096012: add r0,pc
  00096014: b 0x0009619c
  00096016: ldr r0,[r5,#0x30]
  00096018: cbz r0,0x00096020
  0009601a: ldr r0,[r0,#0x4]
  0009601c: adds r0,#0x1
  0009601e: bne 0x0009603a
  00096020: ldr r3,[r5,#0x8]
  00096022: movs r0,#0x2
  00096024: movw r1,#0x1406
  00096028: movs r2,#0x0
  0009602a: blx 0x0006ed28
  0009602e: mov r0,r4
  00096030: movw r1,#0x8078
  00096034: movs r2,#0x1
  00096036: blx 0x0006ed10
  0009603a: ldrb r0,[r5,#0x0]
  0009603c: lsls r0,r0,#0x1d
  0009603e: bmi 0x0009604a
  00096040: mov r0,r4
  00096042: movw r1,#0x8075
  00096046: movs r2,#0x0
  00096048: b 0x0009605e
  0009604a: ldr r2,[r5,#0x10]
  0009604c: movw r0,#0x1406
  00096050: movs r1,#0x0
  00096052: blx 0x0006ed34
  00096056: mov r0,r4
  00096058: movw r1,#0x8075
  0009605c: movs r2,#0x1
  0009605e: blx 0x0006ed10
  00096062: ldrb r0,[r5,#0x0]
  00096064: lsls r0,r0,#0x1c
  00096066: bmi 0x00096072
  00096068: mov r0,r4
  0009606a: movw r1,#0x8076
  0009606e: movs r2,#0x0
  00096070: b 0x00096088
  00096072: ldr r3,[r5,#0xc]
  00096074: movs r0,#0x4
  00096076: movw r1,#0x1406
  0009607a: movs r2,#0x0
  0009607c: blx 0x0006ed40
  00096080: mov r0,r4
  00096082: movw r1,#0x8076
  00096086: movs r2,#0x1
  00096088: blx 0x0006ed10
  0009608c: ldrb r0,[r5,#0x0]
  0009608e: lsls r0,r0,#0x1b
  00096090: bmi 0x0009609e
  00096092: ldrh r2,[r5,#0x2]
  00096094: movs r0,#0x4
  00096096: movs r1,#0x0
  00096098: blx 0x0006fb20
  0009609c: b 0x000960f0
  0009609e: ldrb.w r0,[r4,#0xfe]
  000960a2: cbz r0,0x000960d0
  000960a4: ldrh.w r8,[r5,#0x28]
  000960a8: movs r1,#0x3
  000960aa: mov r0,r8
  000960ac: blx 0x0006ec2c
  000960b0: ldrb.w r1,[r4,#0xfd]
  000960b4: cbz r1,0x000960d6
  000960b6: ldrd r1,r2,[r4,#0x5c]
  000960ba: add r0,r1
  000960bc: adds r1,r2,#0x1
  000960be: strd r0,r1,[r4,#0x5c]
  000960c2: b 0x000960e2
  000960c4: ldr r0,[0x000962c8]
  000960c6: add r0,pc
  000960c8: b 0x0009619c
  000960ca: ldr r0,[0x000962cc]
  000960cc: add r0,pc
  000960ce: b 0x0009619c
  000960d0: ldrh.w r8,[r5,#0x28]
  000960d4: b 0x000960e2
  000960d6: ldr r1,[r4,#0x58]
  000960d8: ldr r2,[r4,#0x64]
  000960da: add r0,r1
  000960dc: str r0,[r4,#0x58]
  000960de: adds r0,r2,#0x1
  000960e0: str r0,[r4,#0x64]
  000960e2: ldr r3,[r5,#0x2c]
  000960e4: movs r0,#0x4
  000960e6: mov r1,r8
  000960e8: movw r2,#0x1403
  000960ec: blx 0x0006ed4c
  000960f0: ldrb r0,[r5,#0x0]
  000960f2: lsls r0,r0,#0x1e
  000960f4: bpl.w 0x0009626e
  000960f8: ldr r0,[r5,#0x30]
  000960fa: cmp r0,#0x0
  000960fc: beq.w 0x0009626e
  00096100: ldr r0,[r0,#0x4]
  00096102: adds r0,#0x1
  00096104: beq.w 0x0009626e
  00096108: mov r0,r4
  0009610a: movw r1,#0x8078
  0009610e: movs r2,#0x0
  00096110: blx 0x0006ed10
  00096114: ldr r0,[sp,#0x4]
  00096116: ldr r1,[r6,#0x0]
  00096118: subs r0,r1,r0
  0009611a: itttt eq
  0009611c: movw.eq r0,#0x84c0
  00096120: add.eq sp,#0x8
  00096122: pop.eq.w r8
  00096126: pop.eq.w {r4,r5,r6,r7,lr}
  0009612a: it eq
  0009612c: b.eq.w 0x001ab6c8
  00096130: blx 0x0006e824
  00096134: ldr r0,[0x000962d0]
  00096136: add r0,pc
  00096138: b 0x0009619c
  0009613a: ldr r0,[0x000962d4]
  0009613c: add r0,pc
  0009613e: b 0x0009619c
  00096140: ldr r0,[0x000962d8]
  00096142: add r0,pc
  00096144: b 0x0009619c
  00096146: ldr r0,[0x000962dc]
  00096148: add r0,pc
  0009614a: b 0x0009619c
  0009614c: ldr r0,[0x000962e0]
  0009614e: add r0,pc
  00096150: b 0x0009619c
  00096152: ldr r0,[0x000962e4]
  00096154: add r0,pc
  00096156: b 0x0009619c
  00096158: ldr r0,[0x000962e8]
  0009615a: add r0,pc
  0009615c: b 0x0009619c
  0009615e: ldr r0,[0x000962ec]
  00096160: add r0,pc
  00096162: b 0x0009619c
  00096164: ldr r0,[0x000962f0]
  00096166: add r0,pc
  00096168: b 0x0009619c
  0009616a: ldr r0,[0x000962f4]
  0009616c: add r0,pc
  0009616e: b 0x0009619c
  00096170: ldr r0,[0x000962f8]
  00096172: add r0,pc
  00096174: ldr r0,[r0,#0x0]
  00096176: ldrb r0,[r0,#0x0]
  00096178: cbz r0,0x00096180
  0009617a: ldr r0,[0x000962fc]
  0009617c: add r0,pc
  0009617e: b 0x0009619c
  00096180: ldr r0,[0x00096300]
  00096182: add r0,pc
  00096184: b 0x0009619c
  00096186: ldr r0,[0x00096304]
  00096188: add r0,pc
  0009618a: b 0x0009619c
  0009618c: ldr r0,[0x00096308]
  0009618e: add r0,pc
  00096190: b 0x0009619c
  00096192: ldr r0,[0x0009630c]
  00096194: add r0,pc
  00096196: b 0x0009619c
  00096198: ldr r0,[0x00096310]
  0009619a: add r0,pc
  0009619c: ldr r0,[r0,#0x0]
  0009619e: mov r2,r5
  000961a0: ldr r1,[r0,#0x0]
  000961a2: mov r0,r4
  000961a4: blx 0x000707c8
  000961a8: ldrb.w r0,[r8,#0x0]
  000961ac: cmp r0,#0x0
  000961ae: beq 0x0009626e
  000961b0: mov r1,sp
  000961b2: movw r0,#0x8ca6
  000961b6: blx 0x0006ee0c
  000961ba: ldrb r0,[r5,#0x0]
  000961bc: lsls r0,r0,#0x1b
  000961be: bmi 0x000961cc
  000961c0: ldrh r2,[r5,#0x2]
  000961c2: movs r0,#0x4
  000961c4: movs r1,#0x0
  000961c6: blx 0x0006fb20
  000961ca: b 0x00096258
  000961cc: ldrh.w r8,[r5,#0x28]
  000961d0: movs r1,#0x3
  000961d2: mov r0,r8
  000961d4: blx 0x0006ec2c
  000961d8: ldrb.w r2,[r5,#0x5c]
  000961dc: ldrb.w r1,[r4,#0xfd]
  000961e0: cbz r2,0x000961f2
  000961e2: cbz r1,0x00096202
  000961e4: ldr r2,[r4,#0x6c]
  000961e6: ldr r1,[r4,#0x60]
  000961e8: add r0,r2
  000961ea: str r0,[r4,#0x6c]
  000961ec: adds r0,r1,#0x1
  000961ee: str r0,[r4,#0x60]
  000961f0: b 0x0009620e
  000961f2: cbz r1,0x0009623e
  000961f4: ldrd r1,r2,[r4,#0x5c]
  000961f8: add r0,r1
  000961fa: adds r1,r2,#0x1
  000961fc: strd r0,r1,[r4,#0x5c]
  00096200: b 0x0009624a
  00096202: ldrd r1,r2,[r4,#0x64]
  00096206: add r0,r2
  00096208: adds r1,#0x1
  0009620a: strd r1,r0,[r4,#0x64]
  0009620e: ldr r0,[0x0009632c]
  00096210: ldr r1,[r5,#0x64]
  00096212: add r0,pc
  00096214: ldr.w r8,[r0,#0x0]
  00096218: movw r0,#0x8893
  0009621c: blx r8
  0009621e: ldrh r1,[r5,#0x28]
  00096220: movs r0,#0x4
  00096222: movw r2,#0x1403
  00096226: movs r3,#0x0
  00096228: blx 0x0006ed4c
  0009622c: movw r0,#0x8892
  00096230: movs r1,#0x0
  00096232: blx r8
  00096234: movw r0,#0x8893
  00096238: movs r1,#0x0
  0009623a: blx r8
  0009623c: b 0x00096258
  0009623e: ldr r1,[r4,#0x58]
  00096240: ldr r2,[r4,#0x64]
  00096242: add r0,r1
  00096244: str r0,[r4,#0x58]
  00096246: adds r0,r2,#0x1
  00096248: str r0,[r4,#0x64]
  0009624a: ldr r3,[r5,#0x2c]
  0009624c: movs r0,#0x4
  0009624e: mov r1,r8
  00096250: movw r2,#0x1403
  00096254: blx 0x0006ed4c
  00096258: ldr r0,[0x00096330]
  0009625a: ldr.w r1,[r4,#0x514]
  0009625e: add r0,pc
  00096260: ldr r0,[r0,#0x0]
  00096262: ldr r0,[r0,#0x0]
  00096264: ldr.w r0,[r1,r0,lsl #0x2]
  00096268: ldr r1,[r0,#0x0]
  0009626a: ldr r1,[r1,#0x10]
  0009626c: blx r1
  0009626e: ldr r0,[sp,#0x4]
  00096270: ldr r1,[r6,#0x0]
  00096272: subs r0,r1,r0
  00096274: ittt eq
  00096276: add.eq sp,#0x8
  00096278: pop.eq.w r8
  0009627c: pop.eq {r4,r5,r6,r7,pc}
  0009627e: blx 0x0006e824
  00096282: ldrb r1,[r5,#0x0]
  00096284: and r2,r1,#0xa
  00096288: cmp r2,#0x8
  0009628a: bne 0x00096292
  0009628c: ldr r0,[0x00096314]
  0009628e: add r0,pc
  00096290: b 0x0009619c
  00096292: ldrb.w r2,[r4,#0x424]
  00096296: cbz r2,0x0009629e
  00096298: ldr r0,[0x00096318]
  0009629a: add r0,pc
  0009629c: b 0x0009619c
  0009629e: lsls r1,r1,#0x1c
  000962a0: bmi 0x000962b0
  000962a2: lsls r0,r0,#0x8
  000962a4: bmi 0x000962b6
  000962a6: ldr r0,[0x00096324]
  000962a8: add r0,pc
  000962aa: b 0x0009619c
  000962b0: ldr r0,[0x0009631c]
  000962b2: add r0,pc
  000962b4: b 0x0009619c
  000962b6: ldr r0,[0x00096320]
  000962b8: add r0,pc
  000962ba: b 0x0009619c
  001ab6c8: bx pc
```
