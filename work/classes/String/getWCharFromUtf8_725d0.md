# String::getWCharFromUtf8
elf 0x725d0 body 286
Sig: undefined __stdcall getWCharFromUtf8(char * param_1, int param_2)

## decompile
```c

/* AbyssEngine::String::getWCharFromUtf8(char*, int) */

void AbyssEngine::String::getWCharFromUtf8(char *param_1,int param_2)

{
  int iVar1;
  ushort *puVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  ushort *puVar6;
  int iVar7;
  
  iVar1 = -2;
  iVar7 = 0;
  for (iVar3 = 0; iVar3 < param_2; iVar3 = iVar3 + 1) {
    if ((param_1[iVar3] & 0xe0U) == 0xc0) {
      iVar3 = iVar3 + 1;
    }
    else if ((param_1[iVar3] & 0xf0U) == 0xe0) {
      iVar3 = iVar3 + 2;
    }
    iVar1 = iVar1 + -2;
    iVar7 = iVar7 + 1;
  }
  puVar2 = operator_new__(-iVar1);
  puVar6 = puVar2;
  for (iVar1 = 0; iVar1 < param_2; iVar1 = iVar1 + 1) {
    uVar4 = (ushort)(byte)param_1[iVar1];
    uVar5 = (ushort)(byte)param_1[iVar1];
    if ((uVar4 & 0xe0) == 0xc0) {
      uVar4 = uVar4 & 0x1f;
      iVar3 = iVar1 + 1;
LAB_00082646:
      uVar5 = uVar4 << 6 | (byte)param_1[iVar3] & 0x3f;
      iVar1 = iVar3;
    }
    else if ((uVar4 & 0xf0) == 0xe0) {
      iVar3 = iVar1 + 2;
      uVar4 = (byte)param_1[iVar1 + 1] & 0x3f | (uVar4 & 0xf) << 6;
      goto LAB_00082646;
    }
    *puVar6 = uVar5;
    puVar6 = puVar6 + 1;
  }
  iVar1 = 0;
  puVar2[iVar7] = 0;
  do {
    if (iVar7 == iVar1) {
      return;
    }
    uVar4 = puVar2[iVar1];
    switch(uVar4) {
    case 0x410:
      uVar4 = 0x41;
      break;
    case 0x411:
    case 0x413:
    case 0x414:
    case 0x416:
    case 0x417:
    case 0x418:
    case 0x419:
    case 0x41b:
    case 0x41f:
    case 0x423:
    case 0x424:
      break;
    case 0x412:
      uVar4 = 0x42;
      break;
    case 0x415:
      uVar4 = 0x45;
      break;
    case 0x41a:
      uVar4 = 0x4b;
      break;
    case 0x41c:
      uVar4 = 0x4d;
      break;
    case 0x41d:
      uVar4 = 0x48;
      break;
    case 0x41e:
      uVar4 = 0x4f;
      break;
    case 0x420:
      uVar4 = 0x50;
      break;
    case 0x421:
      uVar4 = 0x43;
      break;
    case 0x422:
      uVar4 = 0x54;
      break;
    case 0x425:
      uVar4 = 0x58;
      break;
    default:
      switch(uVar4) {
      case 0x43e:
switchD_000826b0_caseD_43e:
        uVar4 = 0x6f;
        break;
      case 0x43f:
      case 0x442:
      case 0x443:
      case 0x444:
        break;
      case 0x440:
        uVar4 = 0x70;
        break;
      case 0x441:
        uVar4 = 99;
        break;
      case 0x445:
        uVar4 = 0x78;
        break;
      default:
        if (uVar4 == 0x435) {
          uVar4 = 0x65;
        }
        else {
          if (uVar4 != 0xaa) {
            if (uVar4 == 0xba) goto switchD_000826b0_caseD_43e;
            if (uVar4 != 0x430) {
              if (uVar4 == 0x60) {
                uVar4 = 0x27;
              }
              break;
            }
          }
          uVar4 = 0x61;
        }
      }
    }
    puVar2[iVar1] = uVar4;
    iVar1 = iVar1 + 1;
  } while( true );
}

```

## target disasm
```
  000825d0: push {r4,r5,r6,r7,lr}
  000825d2: add r7,sp,#0xc
  000825d4: push.w r8
  000825d8: mov r4,r1
  000825da: mov r5,r0
  000825dc: movs r1,#0x0
  000825de: mvn r0,#0x1
  000825e2: mov.w r8,#0x0
  000825e6: b 0x00082608
  000825e8: ldrb r2,[r5,r1]
  000825ea: and r3,r2,#0xe0
  000825ee: cmp r3,#0xc0
  000825f0: bne 0x000825f6
  000825f2: adds r1,#0x1
  000825f4: b 0x00082600
  000825f6: and r2,r2,#0xf0
  000825fa: cmp r2,#0xe0
  000825fc: it eq
  000825fe: add.eq r1,#0x2
  00082600: subs r0,#0x2
  00082602: adds r1,#0x1
  00082604: add.w r8,r8,#0x1
  00082608: cmp r1,r4
  0008260a: blt 0x000825e8
  0008260c: rsbs r0,r0
  0008260e: blx 0x0006ec08
  00082612: movs r1,#0x0
  00082614: mov.w r12,#0x7c0
  00082618: mov r3,r0
  0008261a: b 0x00082654
  0008261c: ldrb r2,[r5,r1]
  0008261e: and r6,r2,#0xe0
  00082622: cmp r6,#0xc0
  00082624: bne 0x0008262e
  00082626: and.w r2,r12,r2, lsl #0x6
  0008262a: adds r1,#0x1
  0008262c: b 0x00082646
  0008262e: and r6,r2,#0xf0
  00082632: cmp r6,#0xe0
  00082634: bne 0x0008264e
  00082636: adds r6,r5,r1
  00082638: adds r1,#0x2
  0008263a: ldrb r6,[r6,#0x1]
  0008263c: and r6,r6,#0x3f
  00082640: bfi r6,r2,#0x6,#0x4
  00082644: lsls r2,r6,#0x6
  00082646: ldrb r6,[r5,r1]
  00082648: and r6,r6,#0x3f
  0008264c: orrs r2,r6
  0008264e: strh r2,[r3],#0x2
  00082652: adds r1,#0x1
  00082654: cmp r1,r4
  00082656: blt 0x0008261c
  00082658: movs r1,#0x0
  0008265a: movw r2,#0x435
  0008265e: strh.w r1,[r0,r8,lsl #0x1]
  00082662: b 0x00082700
  00082664: ldrh.w r3,[r0,r1,lsl #0x1]
  00082668: sub.w r5,r3,#0x410
  0008266c: cmp r5,#0x15
  0008266e: bhi 0x0008268e
  00082670: tbb [pc,r5]
  0008268a: movs r3,#0x41
  0008268c: b 0x000826fa
  0008268e: subw r5,r3,#0x43e
  00082692: cmp r5,#0x7
  00082694: bls 0x000826b0
  00082696: cmp r3,r2
  00082698: beq 0x000826f8
  0008269a: cmp r3,#0xaa
  0008269c: beq 0x000826c0
  0008269e: cmp r3,#0xba
  000826a0: beq 0x000826bc
  000826a2: cmp.w r3,#0x430
  000826a6: beq 0x000826c0
  000826a8: cmp r3,#0x60
  000826aa: it eq
  000826ac: mov.eq r3,#0x27
  000826ae: b 0x000826fa
  000826b0: tbb [pc,r5]
  000826bc: movs r3,#0x6f
  000826be: b 0x000826fa
  000826c0: movs r3,#0x61
  000826c2: b 0x000826fa
  000826c4: movs r3,#0x42
  000826c6: b 0x000826fa
  000826c8: movs r3,#0x45
  000826ca: b 0x000826fa
  000826cc: movs r3,#0x4b
  000826ce: b 0x000826fa
  000826d0: movs r3,#0x4d
  000826d2: b 0x000826fa
  000826d4: movs r3,#0x48
  000826d6: b 0x000826fa
  000826d8: movs r3,#0x4f
  000826da: b 0x000826fa
  000826dc: movs r3,#0x50
  000826de: b 0x000826fa
  000826e0: movs r3,#0x43
  000826e2: b 0x000826fa
  000826e4: movs r3,#0x54
  000826e6: b 0x000826fa
  000826e8: movs r3,#0x58
  000826ea: b 0x000826fa
  000826ec: movs r3,#0x70
  000826ee: b 0x000826fa
  000826f0: movs r3,#0x63
  000826f2: b 0x000826fa
  000826f4: movs r3,#0x78
  000826f6: b 0x000826fa
  000826f8: movs r3,#0x65
  000826fa: strh.w r3,[r0,r1,lsl #0x1]
  000826fe: adds r1,#0x1
  00082700: cmp r8,r1
  00082702: bne.w 0x00082664
  00082706: pop.w r8
  0008270a: pop {r4,r5,r6,r7,pc}
```
