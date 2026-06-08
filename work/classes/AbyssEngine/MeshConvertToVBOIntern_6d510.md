# AbyssEngine::MeshConvertToVBOIntern
elf 0x6d510 body 700
Sig: undefined __stdcall MeshConvertToVBOIntern(Mesh * param_1)

## decompile
```c

/* AbyssEngine::MeshConvertToVBOIntern(AbyssEngine::Mesh*) */

undefined4 AbyssEngine::MeshConvertToVBOIntern(Mesh *param_1)

{
  Mesh MVar1;
  undefined4 uVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  uVar2 = 0xfffffffc;
  if ((param_1 != (Mesh *)0x0) && (**(char **)(DAT_0007d7cc + 0x7d526) != '\0')) {
    if ((param_1[0x5c] == (Mesh)0x0) && (*(short *)(param_1 + 0x28) != 0)) {
      uVar8 = *(undefined4 *)(param_1 + 4);
      uVar6 = *(undefined4 *)(param_1 + 8);
      uVar2 = *(undefined4 *)(param_1 + 0xc);
      uVar7 = *(undefined4 *)(param_1 + 0x10);
      uVar10 = *(undefined4 *)(param_1 + 0x14);
      uVar9 = *(undefined4 *)(param_1 + 0x18);
      glGenBuffers(1,param_1 + 0x60);
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
      glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) * 0xc,uVar8,0x88e4);
      MVar1 = *param_1;
      if (((byte)MVar1 & 2) != 0) {
        glGenBuffers(1,param_1 + 0x68);
        glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
        glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) << 3,uVar6,0x88e4);
        *(uint *)(param_1 + 0x7c) = *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 2) * 8;
        glGenBuffers(1,param_1 + 100);
        glBindBuffer(0x8893,*(undefined4 *)(param_1 + 100));
        glBufferData(0x8893,(uint)*(ushort *)(param_1 + 0x28) << 1,*(undefined4 *)(param_1 + 0x2c),
                     0x88e4);
        MVar1 = *param_1;
        *(uint *)(param_1 + 0x7c) = *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 0x28) * 2
        ;
      }
      if ((int)((uint)(byte)MVar1 << 0x1d) < 0) {
        glGenBuffers(1,param_1 + 0x6c);
        glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x6c));
        glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) * 0xc,uVar7,0x88e4);
        pcVar3 = *(char **)(DAT_0007d7d0 + 0x7d61c);
        *(uint *)(param_1 + 0x7c) = *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 2) * 0xc;
        if (*pcVar3 != '\0') {
          glGenBuffers(1,param_1 + 0x70);
          glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x70));
          glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) * 0xc,uVar10,0x88e4);
          *(uint *)(param_1 + 0x7c) =
               *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 2) * 0xc;
          glGenBuffers(1,param_1 + 0x74);
          glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x74));
          glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) * 0xc,uVar9,0x88e4);
          *(uint *)(param_1 + 0x7c) =
               *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 2) * 0xc;
        }
      }
      if ((int)((uint)(byte)*param_1 << 0x1c) < 0) {
        glGenBuffers(1,param_1 + 0x78);
        glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x78));
        glBufferData(0x8892,(uint)*(ushort *)(param_1 + 2) << 4,uVar2,0x88e4);
        *(uint *)(param_1 + 0x7c) = *(int *)(param_1 + 0x7c) + (uint)*(ushort *)(param_1 + 2) * 0x10
        ;
      }
      glBindBuffer(0x8892,0);
      glBindBuffer(0x8893,0);
      iVar4 = glGetError();
      if (iVar4 == 0) {
        if (**(char **)(DAT_0007d7d8 + 0x7d764) == '\0') {
          if (*(void **)(param_1 + 4) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 4));
          }
          *(undefined4 *)(param_1 + 4) = 0;
          if (*(void **)(param_1 + 8) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 8));
          }
          *(undefined4 *)(param_1 + 8) = 0;
          if (*(void **)(param_1 + 0x2c) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 0x2c));
          }
          *(undefined4 *)(param_1 + 0x2c) = 0;
          if (*(void **)(param_1 + 0x10) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 0x10));
          }
          *(undefined4 *)(param_1 + 0x10) = 0;
          if (*(void **)(param_1 + 0xc) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 0xc));
          }
          *(undefined4 *)(param_1 + 0xc) = 0;
          if (*(void **)(param_1 + 0x14) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 0x14));
          }
          *(undefined4 *)(param_1 + 0x14) = 0;
          if (*(void **)(param_1 + 0x18) != (void *)0x0) {
            operator_delete__(*(void **)(param_1 + 0x18));
          }
          *(undefined4 *)(param_1 + 0x18) = 0;
        }
        piVar5 = *(int **)(DAT_0007d7dc + 0x7d7be);
        uVar2 = 1;
        param_1[0x5c] = (Mesh)0x1;
        *piVar5 = *(int *)(param_1 + 0x7c) + *piVar5;
      }
      else {
        if (param_1[0x5c] != (Mesh)0x0) {
          glDeleteBuffers(1,param_1 + 0x60);
          glDeleteBuffers(1,param_1 + 100);
          MVar1 = *param_1;
          if (((byte)MVar1 & 2) != 0) {
            glDeleteBuffers(1,param_1 + 0x68);
            MVar1 = *param_1;
          }
          if (((int)((uint)(byte)MVar1 << 0x1d) < 0) &&
             (glDeleteBuffers(1,param_1 + 0x6c), **(char **)(DAT_0007d7d4 + 0x7d72c) != '\0')) {
            glDeleteBuffers(1,param_1 + 0x70);
            glDeleteBuffers(1,param_1 + 0x74);
          }
          if ((int)((uint)(byte)*param_1 << 0x1c) < 0) {
            glDeleteBuffers(1,param_1 + 0x78);
          }
        }
        *(undefined4 *)(param_1 + 0x7c) = 0;
        uVar2 = 0xffffffff;
      }
    }
    else {
      uVar2 = 0xfffffffc;
    }
  }
  return uVar2;
}

```

## target disasm
```
  0007d510: push {r4,r5,r6,r7,lr}
  0007d512: add r7,sp,#0xc
  0007d514: push {r7,r8,r9,r10,r11}
  0007d518: mov r4,r0
  0007d51a: mvn r0,#0x3
  0007d51e: cbz r4,0x0007d534
  0007d520: ldr r1,[0x0007d7cc]
  0007d522: add r1,pc
  0007d524: ldr r1,[r1,#0x0]
  0007d526: ldrb r1,[r1,#0x0]
  0007d528: cbz r1,0x0007d534
  0007d52a: ldrb.w r0,[r4,#0x5c]
  0007d52e: cbz r0,0x0007d53a
  0007d530: mvn r0,#0x3
  0007d534: pop.w {r3,r8,r9,r10,r11}
  0007d538: pop {r4,r5,r6,r7,pc}
  0007d53a: ldrh r0,[r4,#0x28]
  0007d53c: cmp r0,#0x0
  0007d53e: beq 0x0007d530
  0007d540: add.w r11,r4,#0x60
  0007d544: ldrd r8,r5,[r4,#0x4]
  0007d548: ldr r0,[r4,#0xc]
  0007d54a: str r0,[sp,#0x0]
  0007d54c: movs r0,#0x1
  0007d54e: mov r1,r11
  0007d550: ldrd r6,r10,[r4,#0x10]
  0007d554: ldr.w r9,[r4,#0x18]
  0007d558: blx 0x0006ed58
  0007d55c: ldr r1,[r4,#0x60]
  0007d55e: movw r0,#0x8892
  0007d562: blx 0x0006ed04
  0007d566: ldrh r0,[r4,#0x2]
  0007d568: mov r2,r8
  0007d56a: movw r3,#0x88e4
  0007d56e: add.w r0,r0,r0, lsl #0x1
  0007d572: lsls r1,r0,#0x2
  0007d574: movw r0,#0x8892
  0007d578: blx 0x0006ed64
  0007d57c: ldrb r0,[r4,#0x0]
  0007d57e: tst r0,#0x2
  0007d582: beq 0x0007d5e6
  0007d584: add.w r1,r4,#0x68
  0007d588: movs r0,#0x1
  0007d58a: blx 0x0006ed58
  0007d58e: ldr r1,[r4,#0x68]
  0007d590: movw r0,#0x8892
  0007d594: blx 0x0006ed04
  0007d598: ldrh r0,[r4,#0x2]
  0007d59a: mov r2,r5
  0007d59c: movw r3,#0x88e4
  0007d5a0: lsls r1,r0,#0x3
  0007d5a2: movw r0,#0x8892
  0007d5a6: blx 0x0006ed64
  0007d5aa: ldrh r0,[r4,#0x2]
  0007d5ac: ldr r1,[r4,#0x7c]
  0007d5ae: add.w r0,r1,r0, lsl #0x3
  0007d5b2: add.w r1,r4,#0x64
  0007d5b6: str r0,[r4,#0x7c]
  0007d5b8: movs r0,#0x1
  0007d5ba: blx 0x0006ed58
  0007d5be: ldr r1,[r4,#0x64]
  0007d5c0: movw r0,#0x8893
  0007d5c4: blx 0x0006ed04
  0007d5c8: ldrh r0,[r4,#0x28]
  0007d5ca: movw r3,#0x88e4
  0007d5ce: ldr r2,[r4,#0x2c]
  0007d5d0: lsls r1,r0,#0x1
  0007d5d2: movw r0,#0x8893
  0007d5d6: blx 0x0006ed64
  0007d5da: ldrb r0,[r4,#0x0]
  0007d5dc: ldrh r1,[r4,#0x28]
  0007d5de: ldr r2,[r4,#0x7c]
  0007d5e0: add.w r1,r2,r1, lsl #0x1
  0007d5e4: str r1,[r4,#0x7c]
  0007d5e6: lsls r0,r0,#0x1d
  0007d5e8: bpl 0x0007d69c
  0007d5ea: add.w r1,r4,#0x6c
  0007d5ee: movs r0,#0x1
  0007d5f0: blx 0x0006ed58
  0007d5f4: ldr r1,[r4,#0x6c]
  0007d5f6: movw r0,#0x8892
  0007d5fa: blx 0x0006ed04
  0007d5fe: ldrh r0,[r4,#0x2]
  0007d600: mov r2,r6
  0007d602: movw r3,#0x88e4
  0007d606: add.w r0,r0,r0, lsl #0x1
  0007d60a: lsls r1,r0,#0x2
  0007d60c: movw r0,#0x8892
  0007d610: blx 0x0006ed64
  0007d614: ldr r0,[0x0007d7d0]
  0007d616: ldrh r1,[r4,#0x2]
  0007d618: add r0,pc
  0007d61a: ldr r2,[r4,#0x7c]
  0007d61c: add.w r1,r1,r1, lsl #0x1
  0007d620: ldr r0,[r0,#0x0]
  0007d622: add.w r1,r2,r1, lsl #0x2
  0007d626: str r1,[r4,#0x7c]
  0007d628: ldrb r0,[r0,#0x0]
  0007d62a: cbz r0,0x0007d69c
  0007d62c: add.w r1,r4,#0x70
  0007d630: movs r0,#0x1
  0007d632: blx 0x0006ed58
  0007d636: ldr r1,[r4,#0x70]
  0007d638: movw r0,#0x8892
  0007d63c: blx 0x0006ed04
  0007d640: ldrh r0,[r4,#0x2]
  0007d642: mov r2,r10
  0007d644: movw r3,#0x88e4
  0007d648: add.w r0,r0,r0, lsl #0x1
  0007d64c: lsls r1,r0,#0x2
  0007d64e: movw r0,#0x8892
  0007d652: blx 0x0006ed64
  0007d656: ldrh r0,[r4,#0x2]
  0007d658: ldr r1,[r4,#0x7c]
  0007d65a: add.w r0,r0,r0, lsl #0x1
  0007d65e: add.w r0,r1,r0, lsl #0x2
  0007d662: add.w r1,r4,#0x74
  0007d666: str r0,[r4,#0x7c]
  0007d668: movs r0,#0x1
  0007d66a: blx 0x0006ed58
  0007d66e: ldr r1,[r4,#0x74]
  0007d670: movw r0,#0x8892
  0007d674: blx 0x0006ed04
  0007d678: ldrh r0,[r4,#0x2]
  0007d67a: mov r2,r9
  0007d67c: movw r3,#0x88e4
  0007d680: add.w r0,r0,r0, lsl #0x1
  0007d684: lsls r1,r0,#0x2
  0007d686: movw r0,#0x8892
  0007d68a: blx 0x0006ed64
  0007d68e: ldrh r0,[r4,#0x2]
  0007d690: ldr r1,[r4,#0x7c]
  0007d692: add.w r0,r0,r0, lsl #0x1
  0007d696: add.w r0,r1,r0, lsl #0x2
  0007d69a: str r0,[r4,#0x7c]
  0007d69c: ldrb r0,[r4,#0x0]
  0007d69e: lsls r0,r0,#0x1c
  0007d6a0: bpl 0x0007d6d2
  0007d6a2: add.w r1,r4,#0x78
  0007d6a6: movs r0,#0x1
  0007d6a8: blx 0x0006ed58
  0007d6ac: ldr r1,[r4,#0x78]
  0007d6ae: movw r0,#0x8892
  0007d6b2: blx 0x0006ed04
  0007d6b6: ldrh r0,[r4,#0x2]
  0007d6b8: movw r3,#0x88e4
  0007d6bc: ldr r2,[sp,#0x0]
  0007d6be: lsls r1,r0,#0x4
  0007d6c0: movw r0,#0x8892
  0007d6c4: blx 0x0006ed64
  0007d6c8: ldrh r0,[r4,#0x2]
  0007d6ca: ldr r1,[r4,#0x7c]
  0007d6cc: add.w r0,r1,r0, lsl #0x4
  0007d6d0: str r0,[r4,#0x7c]
  0007d6d2: movw r0,#0x8892
  0007d6d6: movs r1,#0x0
  0007d6d8: blx 0x0006ed04
  0007d6dc: movw r0,#0x8893
  0007d6e0: movs r1,#0x0
  0007d6e2: blx 0x0006ed04
  0007d6e6: blx 0x0006ed70
  0007d6ea: cbz r0,0x0007d75e
  0007d6ec: ldrb.w r0,[r4,#0x5c]
  0007d6f0: cbz r0,0x0007d754
  0007d6f2: movs r0,#0x1
  0007d6f4: mov r1,r11
  0007d6f6: blx 0x0006ebf0
  0007d6fa: add.w r1,r4,#0x64
  0007d6fe: movs r0,#0x1
  0007d700: blx 0x0006ebf0
  0007d704: ldrb r0,[r4,#0x0]
  0007d706: tst r0,#0x2
  0007d70a: beq 0x0007d718
  0007d70c: add.w r1,r4,#0x68
  0007d710: movs r0,#0x1
  0007d712: blx 0x0006ebf0
  0007d716: ldrb r0,[r4,#0x0]
  0007d718: lsls r0,r0,#0x1d
  0007d71a: bpl 0x0007d744
  0007d71c: add.w r1,r4,#0x6c
  0007d720: movs r0,#0x1
  0007d722: blx 0x0006ebf0
  0007d726: ldr r0,[0x0007d7d4]
  0007d728: add r0,pc
  0007d72a: ldr r0,[r0,#0x0]
  0007d72c: ldrb r0,[r0,#0x0]
  0007d72e: cbz r0,0x0007d744
  0007d730: add.w r1,r4,#0x70
  0007d734: movs r0,#0x1
  0007d736: blx 0x0006ebf0
  0007d73a: add.w r1,r4,#0x74
  0007d73e: movs r0,#0x1
  0007d740: blx 0x0006ebf0
  0007d744: ldrb r0,[r4,#0x0]
  0007d746: lsls r0,r0,#0x1c
  0007d748: bpl 0x0007d754
  0007d74a: add.w r1,r4,#0x78
  0007d74e: movs r0,#0x1
  0007d750: blx 0x0006ebf0
  0007d754: movs r0,#0x0
  0007d756: str r0,[r4,#0x7c]
  0007d758: mov.w r0,#0xffffffff
  0007d75c: b 0x0007d534
  0007d75e: ldr r0,[0x0007d7d8]
  0007d760: add r0,pc
  0007d762: ldr r0,[r0,#0x0]
  0007d764: ldrb r0,[r0,#0x0]
  0007d766: cbnz r0,0x0007d7b6
  0007d768: ldr r0,[r4,#0x4]
  0007d76a: cbz r0,0x0007d770
  0007d76c: blx 0x0006ebfc
  0007d770: ldr r0,[r4,#0x8]
  0007d772: movs r5,#0x0
  0007d774: str r5,[r4,#0x4]
  0007d776: cbz r0,0x0007d77c
  0007d778: blx 0x0006ebfc
  0007d77c: ldr r0,[r4,#0x2c]
  0007d77e: str r5,[r4,#0x8]
  0007d780: cbz r0,0x0007d786
  0007d782: blx 0x0006ebfc
  0007d786: ldr r0,[r4,#0x10]
  0007d788: movs r5,#0x0
  0007d78a: str r5,[r4,#0x2c]
  0007d78c: cbz r0,0x0007d792
  0007d78e: blx 0x0006ebfc
  0007d792: ldr r0,[r4,#0xc]
  0007d794: str r5,[r4,#0x10]
  0007d796: cbz r0,0x0007d79c
  0007d798: blx 0x0006ebfc
  0007d79c: ldr r0,[r4,#0x14]
  0007d79e: movs r5,#0x0
  0007d7a0: str r5,[r4,#0xc]
  0007d7a2: cbz r0,0x0007d7a8
  0007d7a4: blx 0x0006ebfc
  0007d7a8: ldr r0,[r4,#0x18]
  0007d7aa: str r5,[r4,#0x14]
  0007d7ac: cbz r0,0x0007d7b2
  0007d7ae: blx 0x0006ebfc
  0007d7b2: movs r0,#0x0
  0007d7b4: str r0,[r4,#0x18]
  0007d7b6: ldr r0,[0x0007d7dc]
  0007d7b8: ldr r2,[r4,#0x7c]
  0007d7ba: add r0,pc
  0007d7bc: ldr r1,[r0,#0x0]
  0007d7be: movs r0,#0x1
  0007d7c0: strb.w r0,[r4,#0x5c]
  0007d7c4: ldr r3,[r1,#0x0]
  0007d7c6: add r2,r3
  0007d7c8: str r2,[r1,#0x0]
  0007d7ca: b 0x0007d534
```
