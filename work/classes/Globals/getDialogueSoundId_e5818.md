# Globals::getDialogueSoundId
elf 0xe5818 body 1250
Sig: undefined __thiscall getDialogueSoundId(Globals * this, int param_1, Agent * param_2)

## decompile
```c

/* Globals::getDialogueSoundId(int, Agent*) */

int __thiscall Globals::getDialogueSoundId(Globals *this,int param_1,Agent *param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  for (uVar1 = 0; uVar1 >> 6 < 0x2f; uVar1 = uVar1 + 2) {
    if (*(int *)(DAT_000f5a6c + 0xf582c + uVar1 * 4) == param_1) {
      return *(int *)(DAT_000f5a6c + 0xf582c + uVar1 * 4 + 4);
    }
  }
  if (param_2 == (Agent *)0x0) {
    return -1;
  }
  iVar2 = Agent::getRace(param_2);
  iVar3 = Agent::isMale(param_2);
  if (iVar2 == 3) {
    iVar2 = Agent::getImageParts(param_2);
    if (iVar2 != 0) {
      piVar4 = (int *)Agent::getImageParts(param_2);
      iVar2 = 0;
      if (*piVar4 == 2) {
        iVar2 = 3;
      }
      goto LAB_000f5870;
    }
switchD_000f5876_caseD_2:
    iVar3 = -1;
    iVar2 = 0x26b;
    switch(param_1) {
    case 0x172:
      iVar2 = 0x27e;
      break;
    case 0x173:
      iVar2 = 0x272;
      break;
    case 0x174:
      iVar2 = 0x278;
      break;
    case 0x175:
      iVar2 = 0x279;
      break;
    case 0x176:
      iVar2 = 0x27a;
      break;
    case 0x177:
      iVar2 = 0x27b;
      break;
    case 0x178:
      iVar2 = 0x27c;
      break;
    case 0x179:
      iVar2 = 0x27d;
      break;
    case 0x17a:
      iVar2 = 0x26c;
      break;
    case 0x17b:
      iVar2 = 0x273;
      break;
    case 0x17c:
      iVar2 = 0x274;
      break;
    case 0x17d:
      iVar2 = 0x275;
      break;
    case 0x17e:
      iVar2 = 0x276;
      break;
    case 0x17f:
      iVar2 = 0x277;
      break;
    case 0x180:
      iVar2 = 0x26d;
      break;
    case 0x181:
      iVar2 = 0x26e;
      break;
    case 0x182:
      iVar2 = 0x26f;
      break;
    case 0x183:
      iVar2 = 0x270;
      break;
    case 0x184:
      iVar2 = 0x271;
      break;
    case 0x185:
      break;
    case 0x186:
    case 0x187:
    case 0x188:
    case 0x189:
    case 0x18a:
    case 0x18b:
    case 0x18c:
    case 0x18d:
    case 0x18e:
    case 399:
    case 400:
    case 0x191:
    case 0x192:
    case 0x193:
    case 0x194:
    case 0x195:
    case 0x196:
    case 0x197:
    case 0x198:
    case 0x199:
    case 0x19a:
    case 0x19b:
    case 0x19c:
    case 0x19d:
    case 0x19e:
    case 0x19f:
    case 0x1a0:
    case 0x1a1:
    case 0x1a2:
    case 0x1a3:
    case 0x1a4:
    case 0x1a5:
    case 0x1a6:
    case 0x1a7:
    case 0x1a8:
    case 0x1a9:
    case 0x1b1:
    case 0x1b3:
    case 0x1b4:
    case 0x1b5:
    case 0x1b6:
    case 0x1b7:
    case 0x1b8:
    case 0x1b9:
    case 0x1bb:
    case 0x1bc:
      goto switchD_000f58ee_caseD_186;
    case 0x1aa:
      iVar2 = 0x285;
      break;
    case 0x1ab:
      iVar2 = 0x286;
      break;
    case 0x1ac:
      iVar2 = 0x287;
      break;
    case 0x1ad:
      iVar2 = 0x28a;
      break;
    case 0x1ae:
      iVar2 = 0x28b;
      break;
    case 0x1af:
      iVar2 = 0x28c;
      break;
    case 0x1b0:
      iVar2 = 0x24c;
      break;
    case 0x1b2:
      iVar2 = 0x24a;
      break;
    case 0x1ba:
      iVar2 = 0x24d;
      break;
    case 0x1bd:
      iVar2 = 0x27f;
      break;
    case 0x1be:
      iVar2 = 0x281;
      break;
    case 0x1bf:
      iVar2 = 0x282;
      break;
    default:
      if (param_1 != 0x139) {
        return -1;
      }
      iVar2 = 0x28f;
    }
  }
  else {
LAB_000f5870:
    switch(iVar2) {
    case 0:
    case 5:
      if (iVar3 == 0) {
        if (0x13 < param_1 - 0x172U) {
          return -1;
        }
        iVar2 = *(int *)(DAT_000f5d88 + 0xf5abe + (param_1 - 0x172U) * 4);
      }
      else {
        iVar2 = 0x2a8;
        switch(param_1) {
        case 0x172:
          iVar2 = 699;
          break;
        case 0x173:
          iVar2 = 0x2af;
          break;
        case 0x174:
          iVar2 = 0x2b5;
          break;
        case 0x175:
          iVar2 = 0x2b6;
          break;
        case 0x176:
          iVar2 = 0x2b7;
          break;
        case 0x177:
          iVar2 = 0x2b8;
          break;
        case 0x178:
          iVar2 = 0x2b9;
          break;
        case 0x179:
          iVar2 = 0x2ba;
          break;
        case 0x17a:
          iVar2 = 0x2a9;
          break;
        case 0x17b:
          iVar2 = 0x2b0;
          break;
        case 0x17c:
          iVar2 = 0x2b1;
          break;
        case 0x17d:
          iVar2 = 0x2b2;
          break;
        case 0x17e:
          iVar2 = 0x2b3;
          break;
        case 0x17f:
          iVar2 = 0x2b4;
          break;
        case 0x180:
          iVar2 = 0x2aa;
          break;
        case 0x181:
          iVar2 = 0x2ab;
          break;
        case 0x182:
          iVar2 = 0x2ac;
          break;
        case 0x183:
          iVar2 = 0x2ad;
          break;
        case 0x184:
          iVar2 = 0x2ae;
          break;
        case 0x185:
          break;
        default:
          iVar3 = -1;
          switch(param_1) {
          case 0x1aa:
            iVar2 = 0x2c1;
            break;
          case 0x1ab:
            iVar2 = 0x2c2;
            break;
          case 0x1ac:
            iVar2 = 0x2c3;
            break;
          case 0x1ad:
            iVar2 = 0x2c6;
            break;
          case 0x1ae:
            iVar2 = 0x2c7;
            break;
          case 0x1af:
            iVar2 = 0x2c8;
            break;
          case 0x1b0:
          case 0x1b1:
          case 0x1b2:
          case 0x1b3:
          case 0x1b4:
          case 0x1b5:
          case 0x1b6:
          case 0x1b7:
          case 0x1b8:
          case 0x1b9:
          case 0x1ba:
          case 0x1bb:
          case 0x1bc:
            goto switchD_000f58ee_caseD_186;
          case 0x1bd:
            iVar2 = 700;
            break;
          case 0x1be:
            iVar2 = 0x2bd;
            break;
          case 0x1bf:
            iVar2 = 0x2be;
            break;
          default:
            if (param_1 != 0x139) {
              return -1;
            }
            iVar2 = 0x2cb;
          }
        }
      }
      break;
    case 1:
      iVar2 = 0x2cc;
      switch(param_1) {
      case 0x172:
        iVar2 = 0x2df;
        break;
      case 0x173:
        iVar2 = 0x2d3;
        break;
      case 0x174:
        iVar2 = 0x2d9;
        break;
      case 0x175:
        iVar2 = 0x2da;
        break;
      case 0x176:
        iVar2 = 0x2db;
        break;
      case 0x177:
        iVar2 = 0x2dc;
        break;
      case 0x178:
        iVar2 = 0x2dd;
        break;
      case 0x179:
        iVar2 = 0x2de;
        break;
      case 0x17a:
        iVar2 = 0x2cd;
        break;
      case 0x17b:
        iVar2 = 0x2d4;
        break;
      case 0x17c:
        iVar2 = 0x2d5;
        break;
      case 0x17d:
        iVar2 = 0x2d6;
        break;
      case 0x17e:
        iVar2 = 0x2d7;
        break;
      case 0x17f:
        iVar2 = 0x2d8;
        break;
      case 0x180:
        iVar2 = 0x2ce;
        break;
      case 0x181:
        iVar2 = 0x2cf;
        break;
      case 0x182:
        iVar2 = 0x2d0;
        break;
      case 0x183:
        iVar2 = 0x2d1;
        break;
      case 0x184:
        iVar2 = 0x2d2;
        break;
      case 0x185:
        break;
      default:
        iVar3 = -1;
        switch(param_1) {
        case 0x1aa:
          iVar2 = 0x2e5;
          break;
        case 0x1ab:
          iVar2 = 0x2e6;
          break;
        case 0x1ac:
          iVar2 = 0x2e7;
          break;
        case 0x1ad:
          iVar2 = 0x2ea;
          break;
        case 0x1ae:
          iVar2 = 0x2eb;
          break;
        case 0x1af:
          iVar2 = 0x2ec;
          break;
        default:
          goto switchD_000f58ee_caseD_186;
        case 0x1bd:
          iVar2 = 0x2e0;
          break;
        case 0x1be:
          iVar2 = 0x2e1;
          break;
        case 0x1bf:
          iVar2 = 0x2e2;
        }
      }
      break;
    case 2:
    case 3:
      goto switchD_000f5876_caseD_2;
    case 4:
      iVar2 = 0x255;
      switch(param_1) {
      case 0x172:
        iVar2 = 0x267;
        break;
      case 0x173:
        iVar2 = 0x25b;
        break;
      case 0x174:
        iVar2 = 0x261;
        break;
      case 0x175:
        iVar2 = 0x262;
        break;
      case 0x176:
        iVar2 = 0x263;
        break;
      case 0x177:
        iVar2 = 0x264;
        break;
      case 0x178:
        iVar2 = 0x265;
        break;
      case 0x179:
        iVar2 = 0x266;
        break;
      case 0x17a:
        break;
      case 0x17b:
        iVar2 = 0x25c;
        break;
      case 0x17c:
        iVar2 = 0x25d;
        break;
      case 0x17d:
        iVar2 = 0x25e;
        break;
      case 0x17e:
        iVar2 = 0x25f;
        break;
      case 0x17f:
        iVar2 = 0x260;
        break;
      case 0x180:
        iVar2 = 0x256;
        break;
      case 0x181:
        iVar2 = 599;
        break;
      case 0x182:
        iVar2 = 600;
        break;
      case 0x183:
        iVar2 = 0x259;
        break;
      case 0x184:
        iVar2 = 0x25a;
        break;
      case 0x185:
        iVar2 = 0x26a;
        break;
      default:
        if (param_1 != 0x139) {
          return -1;
        }
        iVar2 = 0x268;
      }
      break;
    case 6:
      iVar2 = 0x2ef;
      switch(param_1) {
      case 0x172:
        iVar2 = 0x230;
        break;
      case 0x173:
        iVar2 = 0x2f6;
        break;
      case 0x174:
        iVar2 = 0x22a;
        break;
      case 0x175:
        iVar2 = 0x22b;
        break;
      case 0x176:
        iVar2 = 0x22c;
        break;
      case 0x177:
        iVar2 = 0x22d;
        break;
      case 0x178:
        iVar2 = 0x22e;
        break;
      case 0x179:
        iVar2 = 0x22f;
        break;
      case 0x17a:
        iVar2 = 0x2f0;
        break;
      case 0x17b:
        iVar2 = 0x225;
        break;
      case 0x17c:
        iVar2 = 0x226;
        break;
      case 0x17d:
        iVar2 = 0x227;
        break;
      case 0x17e:
        iVar2 = 0x228;
        break;
      case 0x17f:
        iVar2 = 0x229;
        break;
      case 0x180:
        iVar2 = 0x2f1;
        break;
      case 0x181:
        iVar2 = 0x2f2;
        break;
      case 0x182:
        iVar2 = 0x2f3;
        break;
      case 0x183:
        iVar2 = 0x2f4;
        break;
      case 0x184:
        iVar2 = 0x2f5;
        break;
      case 0x185:
        break;
      default:
        if (param_1 != 0x139) {
          return -1;
        }
        iVar2 = 0x231;
      }
      break;
    case 7:
      iVar2 = 0x233;
      switch(param_1) {
      case 0x172:
        iVar2 = 0x246;
        break;
      case 0x173:
        iVar2 = 0x23a;
        break;
      case 0x174:
        iVar2 = 0x240;
        break;
      case 0x175:
        iVar2 = 0x241;
        break;
      case 0x176:
        iVar2 = 0x242;
        break;
      case 0x177:
        iVar2 = 0x243;
        break;
      case 0x178:
        iVar2 = 0x244;
        break;
      case 0x179:
        iVar2 = 0x245;
        break;
      case 0x17a:
        iVar2 = 0x234;
        break;
      case 0x17b:
        iVar2 = 0x23b;
        break;
      case 0x17c:
        iVar2 = 0x23c;
        break;
      case 0x17d:
        iVar2 = 0x23d;
        break;
      case 0x17e:
        iVar2 = 0x23e;
        break;
      case 0x17f:
        iVar2 = 0x23f;
        break;
      case 0x180:
        iVar2 = 0x235;
        break;
      case 0x181:
        iVar2 = 0x236;
        break;
      case 0x182:
        iVar2 = 0x237;
        break;
      case 0x183:
        iVar2 = 0x238;
        break;
      case 0x184:
        iVar2 = 0x239;
        break;
      case 0x185:
        break;
      default:
        if (param_1 != 0x139) {
          return -1;
        }
        iVar2 = 0x247;
      }
      break;
    case 8:
      if (5 < param_1 - 0x1b3U) {
        return -1;
      }
      iVar2 = param_1 + 0x9b;
      break;
    default:
      return -1;
    }
  }
  iVar3 = iVar2;
switchD_000f58ee_caseD_186:
  return iVar3;
}

```

## target disasm
```
  000f5818: push {r4,r5,r6,r7,lr}
  000f581a: add r7,sp,#0xc
  000f581c: push.w r8
  000f5820: mov r4,r1
  000f5822: ldr r1,[0x000f5a6c]
  000f5824: mov r5,r2
  000f5826: movs r0,#0x0
  000f5828: add r1,pc
  000f582a: b 0x000f5836
  000f582c: ldr.w r2,[r1,r0,lsl #0x2]
  000f5830: cmp r2,r4
  000f5832: beq 0x000f58d4
  000f5834: adds r0,#0x2
  000f5836: lsrs r2,r0,#0x6
  000f5838: cmp r2,#0x2e
  000f583a: bls 0x000f582c
  000f583c: cmp r5,#0x0
  000f583e: beq.w 0x000f5c38
  000f5842: mov r0,r5
  000f5844: blx 0x00071bd8
  000f5848: mov r6,r0
  000f584a: mov r0,r5
  000f584c: blx 0x00074110
  000f5850: mov r8,r0
  000f5852: cmp r6,#0x3
  000f5854: bne 0x000f5870
  000f5856: mov r0,r5
  000f5858: blx 0x00071c74
  000f585c: cmp r0,#0x0
  000f585e: beq 0x000f58dc
  000f5860: mov r0,r5
  000f5862: blx 0x00071c74
  000f5866: ldr r0,[r0,#0x0]
  000f5868: movs r6,#0x0
  000f586a: cmp r0,#0x2
  000f586c: it eq
  000f586e: mov.eq r6,#0x3
  000f5870: cmp r6,#0x8
  000f5872: bhi.w 0x000f5c38
  000f5876: tbh [pc,r6]
  000f588c: cmp.w r8,#0x0
  000f5890: beq.w 0x000f5aae
  000f5894: sub.w r0,r4,#0x172
  000f5898: cmp r0,#0x13
  000f589a: bhi.w 0x000f5ac4
  000f589e: mov.w r1,#0x2a8
  000f58a2: tbh [pc,r0]
  000f58ce: movw r1,#0x2bb
  000f58d2: b 0x000f5ac0
  000f58d4: add.w r0,r1,r0, lsl #0x2
  000f58d8: ldr r0,[r0,#0x4]
  000f58da: b 0x000f5c3c
  000f58dc: sub.w r2,r4,#0x172
  000f58e0: mov.w r0,#0xffffffff
  000f58e4: cmp r2,#0x4d
  000f58e6: bhi.w 0x000f5b4c
  000f58ea: movw r1,#0x26b
  000f58ee: tbh [pc,r2]
  000f598e: movw r1,#0x27e
  000f5992: b 0x000f5ac0
  000f5994: sub.w r0,r4,#0x172
  000f5998: cmp r0,#0x13
  000f599a: bhi.w 0x000f5b08
  000f599e: mov.w r1,#0x2cc
  000f59a2: tbh [pc,r0]
  000f59ce: movw r1,#0x2df
  000f59d2: b 0x000f5ac0
  000f59d4: sub.w r0,r4,#0x172
  000f59d8: cmp r0,#0x13
  000f59da: bhi.w 0x000f5c0e
  000f59de: movw r1,#0x255
  000f59e2: tbh [pc,r0]
  000f5a0e: movw r1,#0x267
  000f5a12: b 0x000f5ac0
  000f5a14: sub.w r0,r4,#0x172
  000f5a18: cmp r0,#0x13
  000f5a1a: bhi.w 0x000f5c1c
  000f5a1e: movw r1,#0x2ef
  000f5a22: tbh [pc,r0]
  000f5a4e: mov.w r1,#0x230
  000f5a52: b 0x000f5ac0
  000f5a54: sub.w r0,r4,#0x172
  000f5a58: cmp r0,#0x13
  000f5a5a: bhi.w 0x000f5c2a
  000f5a5e: adr.w r2,0xf5a70
  000f5a62: movw r1,#0x233
  000f5a66: tbh [r2,r0]
  000f5a98: movw r1,#0x246
  000f5a9c: b 0x000f5ac0
  000f5a9e: subw r0,r4,#0x1b3
  000f5aa2: cmp r0,#0x6
  000f5aa4: bcs.w 0x000f5c38
  000f5aa8: add.w r1,r4,#0x9b
  000f5aac: b 0x000f5ac0
  000f5aae: sub.w r0,r4,#0x172
  000f5ab2: cmp r0,#0x14
  000f5ab4: bcs.w 0x000f5c38
  000f5ab8: ldr r1,[0x000f5d88]
  000f5aba: add r1,pc
  000f5abc: ldr.w r1,[r1,r0,lsl #0x2]
  000f5ac0: mov r0,r1
  000f5ac2: b 0x000f5c3c
  000f5ac4: sub.w r1,r4,#0x1aa
  000f5ac8: mov.w r0,#0xffffffff
  000f5acc: cmp r1,#0x15
  000f5ace: bhi.w 0x000f5d8c
  000f5ad2: tbh [pc,r1]
  000f5b02: movw r1,#0x2c1
  000f5b06: b 0x000f5ac0
  000f5b08: sub.w r1,r4,#0x1aa
  000f5b0c: mov.w r0,#0xffffffff
  000f5b10: cmp r1,#0x15
  000f5b12: bhi.w 0x000f5c3c
  000f5b16: tbh [pc,r1]
  000f5b46: movw r1,#0x2e5
  000f5b4a: b 0x000f5ac0
  000f5b4c: movw r1,#0x139
  000f5b50: cmp r4,r1
  000f5b52: bne 0x000f5c3c
  000f5b54: movw r1,#0x28f
  000f5b58: b 0x000f5ac0
  000f5b5a: movw r1,#0x272
  000f5b5e: b 0x000f5ac0
  000f5b60: mov.w r1,#0x278
  000f5b64: b 0x000f5ac0
  000f5b66: movw r1,#0x279
  000f5b6a: b 0x000f5ac0
  000f5b6c: movw r1,#0x27a
  000f5b70: b 0x000f5ac0
  000f5b72: movw r1,#0x27b
  000f5b76: b 0x000f5ac0
  000f5b78: mov.w r1,#0x27c
  000f5b7c: b 0x000f5ac0
  000f5b7e: movw r1,#0x27d
  000f5b82: b 0x000f5ac0
  000f5b84: mov.w r1,#0x26c
  000f5b88: b 0x000f5ac0
  000f5b8a: movw r1,#0x273
  000f5b8e: b 0x000f5ac0
  000f5b90: mov.w r1,#0x274
  000f5b94: b 0x000f5ac0
  000f5b96: movw r1,#0x275
  000f5b9a: b 0x000f5ac0
  000f5b9c: movw r1,#0x276
  000f5ba0: b 0x000f5ac0
  000f5ba2: movw r1,#0x277
  000f5ba6: b 0x000f5ac0
  000f5ba8: movw r1,#0x26d
  000f5bac: b 0x000f5ac0
  000f5bae: movw r1,#0x26e
  000f5bb2: b 0x000f5ac0
  000f5bb4: movw r1,#0x26f
  000f5bb8: b 0x000f5ac0
  000f5bba: mov.w r1,#0x270
  000f5bbe: b 0x000f5ac0
  000f5bc0: movw r1,#0x271
  000f5bc4: b 0x000f5ac0
  000f5bc6: movw r1,#0x285
  000f5bca: b 0x000f5ac0
  000f5bcc: movw r1,#0x286
  000f5bd0: b 0x000f5ac0
  000f5bd2: movw r1,#0x287
  000f5bd6: b 0x000f5ac0
  000f5bd8: movw r1,#0x28a
  000f5bdc: b 0x000f5ac0
  000f5bde: movw r1,#0x28b
  000f5be2: b 0x000f5ac0
  000f5be4: mov.w r1,#0x28c
  000f5be8: b 0x000f5ac0
  000f5bea: mov.w r1,#0x24c
  000f5bee: b 0x000f5ac0
  000f5bf0: movw r1,#0x24a
  000f5bf4: b 0x000f5ac0
  000f5bf6: movw r1,#0x24d
  000f5bfa: b 0x000f5ac0
  000f5bfc: movw r1,#0x27f
  000f5c00: b 0x000f5ac0
  000f5c02: movw r1,#0x281
  000f5c06: b 0x000f5ac0
  000f5c08: movw r1,#0x282
  000f5c0c: b 0x000f5ac0
  000f5c0e: movw r0,#0x139
  000f5c12: cmp r4,r0
  000f5c14: bne 0x000f5c38
  000f5c16: mov.w r1,#0x268
  000f5c1a: b 0x000f5ac0
  000f5c1c: movw r0,#0x139
  000f5c20: cmp r4,r0
  000f5c22: bne 0x000f5c38
  000f5c24: movw r1,#0x231
  000f5c28: b 0x000f5ac0
  000f5c2a: movw r0,#0x139
  000f5c2e: cmp r4,r0
  000f5c30: bne 0x000f5c38
  000f5c32: movw r1,#0x247
  000f5c36: b 0x000f5ac0
  000f5c38: mov.w r0,#0xffffffff
  000f5c3c: pop.w r8
  000f5c40: pop {r4,r5,r6,r7,pc}
  000f5c42: movw r1,#0x25b
  000f5c46: b 0x000f5ac0
  000f5c48: movw r1,#0x261
  000f5c4c: b 0x000f5ac0
  000f5c4e: movw r1,#0x262
  000f5c52: b 0x000f5ac0
  000f5c54: movw r1,#0x263
  000f5c58: b 0x000f5ac0
  000f5c5a: mov.w r1,#0x264
  000f5c5e: b 0x000f5ac0
  000f5c60: movw r1,#0x265
  000f5c64: b 0x000f5ac0
  000f5c66: movw r1,#0x266
  000f5c6a: b 0x000f5ac0
  000f5c6c: mov.w r1,#0x25c
  000f5c70: b 0x000f5ac0
  000f5c72: movw r1,#0x25d
  000f5c76: b 0x000f5ac0
  000f5c78: movw r1,#0x25e
  000f5c7c: b 0x000f5ac0
  000f5c7e: movw r1,#0x25f
  000f5c82: b 0x000f5ac0
  000f5c84: mov.w r1,#0x260
  000f5c88: b 0x000f5ac0
  000f5c8a: movw r1,#0x256
  000f5c8e: b 0x000f5ac0
  000f5c90: movw r1,#0x257
  000f5c94: b 0x000f5ac0
  000f5c96: mov.w r1,#0x258
  000f5c9a: b 0x000f5ac0
  000f5c9c: movw r1,#0x259
  000f5ca0: b 0x000f5ac0
  000f5ca2: movw r1,#0x25a
  000f5ca6: b 0x000f5ac0
  000f5ca8: movw r1,#0x26a
  000f5cac: b 0x000f5ac0
  000f5cae: movw r1,#0x2f6
  000f5cb2: b 0x000f5ac0
  000f5cb4: movw r1,#0x22a
  000f5cb8: b 0x000f5ac0
  000f5cba: movw r1,#0x22b
  000f5cbe: b 0x000f5ac0
  000f5cc0: mov.w r1,#0x22c
  000f5cc4: b 0x000f5ac0
  000f5cc6: movw r1,#0x22d
  000f5cca: b 0x000f5ac0
  000f5ccc: movw r1,#0x22e
  000f5cd0: b 0x000f5ac0
  000f5cd2: movw r1,#0x22f
  000f5cd6: b 0x000f5ac0
  000f5cd8: mov.w r1,#0x2f0
  000f5cdc: b 0x000f5ac0
  000f5cde: movw r1,#0x225
  000f5ce2: b 0x000f5ac0
  000f5ce4: movw r1,#0x226
  000f5ce8: b 0x000f5ac0
  000f5cea: movw r1,#0x227
  000f5cee: b 0x000f5ac0
  000f5cf0: mov.w r1,#0x228
  000f5cf4: b 0x000f5ac0
  000f5cf6: movw r1,#0x229
  000f5cfa: b 0x000f5ac0
  000f5cfc: movw r1,#0x2f1
  000f5d00: b 0x000f5ac0
  000f5d02: movw r1,#0x2f2
  000f5d06: b 0x000f5ac0
  000f5d08: movw r1,#0x2f3
  000f5d0c: b 0x000f5ac0
  000f5d0e: mov.w r1,#0x2f4
  000f5d12: b 0x000f5ac0
  000f5d14: movw r1,#0x2f5
  000f5d18: b 0x000f5ac0
  000f5d1a: movw r1,#0x23a
  000f5d1e: b 0x000f5ac0
  000f5d20: mov.w r1,#0x240
  000f5d24: b 0x000f5ac0
  000f5d26: movw r1,#0x241
  000f5d2a: b 0x000f5ac0
  000f5d2c: movw r1,#0x242
  000f5d30: b 0x000f5ac0
  000f5d32: movw r1,#0x243
  000f5d36: b 0x000f5ac0
  000f5d38: mov.w r1,#0x244
  000f5d3c: b 0x000f5ac0
  000f5d3e: movw r1,#0x245
  000f5d42: b 0x000f5ac0
  000f5d44: mov.w r1,#0x234
  000f5d48: b 0x000f5ac0
  000f5d4a: movw r1,#0x23b
  000f5d4e: b 0x000f5ac0
  000f5d50: mov.w r1,#0x23c
  000f5d54: b 0x000f5ac0
  000f5d56: movw r1,#0x23d
  000f5d5a: b 0x000f5ac0
  000f5d5c: movw r1,#0x23e
  000f5d60: b 0x000f5ac0
  000f5d62: movw r1,#0x23f
  000f5d66: b 0x000f5ac0
  000f5d68: movw r1,#0x235
  000f5d6c: b 0x000f5ac0
  000f5d6e: movw r1,#0x236
  000f5d72: b 0x000f5ac0
  000f5d74: movw r1,#0x237
  000f5d78: b 0x000f5ac0
  000f5d7a: mov.w r1,#0x238
  000f5d7e: b 0x000f5ac0
  000f5d80: movw r1,#0x239
  000f5d84: b 0x000f5ac0
  000f5d8c: movw r1,#0x139
  000f5d90: cmp r4,r1
  000f5d92: bne.w 0x000f5c3c
  000f5d96: movw r1,#0x2cb
  000f5d9a: b 0x000f5ac0
  000f5d9c: movw r1,#0x2d3
  000f5da0: b 0x000f5ac0
  000f5da2: movw r1,#0x2d9
  000f5da6: b 0x000f5ac0
  000f5da8: movw r1,#0x2da
  000f5dac: b 0x000f5ac0
  000f5dae: movw r1,#0x2db
  000f5db2: b 0x000f5ac0
  000f5db4: mov.w r1,#0x2dc
  000f5db8: b 0x000f5ac0
  000f5dba: movw r1,#0x2dd
  000f5dbe: b 0x000f5ac0
  000f5dc0: movw r1,#0x2de
  000f5dc4: b 0x000f5ac0
  000f5dc6: movw r1,#0x2cd
  000f5dca: b 0x000f5ac0
  000f5dcc: mov.w r1,#0x2d4
  000f5dd0: b 0x000f5ac0
  000f5dd2: movw r1,#0x2d5
  000f5dd6: b 0x000f5ac0
  000f5dd8: movw r1,#0x2d6
  000f5ddc: b 0x000f5ac0
  000f5dde: movw r1,#0x2d7
  000f5de2: b 0x000f5ac0
  000f5de4: mov.w r1,#0x2d8
  000f5de8: b 0x000f5ac0
  000f5dea: movw r1,#0x2ce
  000f5dee: b 0x000f5ac0
  000f5df0: movw r1,#0x2cf
  000f5df4: b 0x000f5ac0
  000f5df6: mov.w r1,#0x2d0
  000f5dfa: b 0x000f5ac0
  000f5dfc: movw r1,#0x2d1
  000f5e00: b 0x000f5ac0
  000f5e02: movw r1,#0x2d2
  000f5e06: b 0x000f5ac0
  000f5e08: movw r1,#0x2c2
  000f5e0c: b 0x000f5ac0
  000f5e0e: movw r1,#0x2c3
  000f5e12: b 0x000f5ac0
  000f5e14: movw r1,#0x2c6
  000f5e18: b 0x000f5ac0
  000f5e1a: movw r1,#0x2c7
  000f5e1e: b 0x000f5ac0
  000f5e20: mov.w r1,#0x2c8
  000f5e24: b 0x000f5ac0
  000f5e26: mov.w r1,#0x2bc
  000f5e2a: b 0x000f5ac0
  000f5e2c: movw r1,#0x2bd
  000f5e30: b 0x000f5ac0
  000f5e32: movw r1,#0x2be
  000f5e36: b 0x000f5ac0
  000f5e38: movw r1,#0x2e6
  000f5e3c: b 0x000f5ac0
  000f5e3e: movw r1,#0x2e7
  000f5e42: b 0x000f5ac0
  000f5e44: movw r1,#0x2ea
  000f5e48: b 0x000f5ac0
  000f5e4a: movw r1,#0x2eb
  000f5e4e: b 0x000f5ac0
  000f5e50: mov.w r1,#0x2ec
  000f5e54: b 0x000f5ac0
  000f5e56: mov.w r1,#0x2e0
  000f5e5a: b 0x000f5ac0
  000f5e5c: movw r1,#0x2e1
  000f5e60: b 0x000f5ac0
  000f5e62: movw r1,#0x2e2
  000f5e66: b 0x000f5ac0
  000f5e68: movw r1,#0x2af
  000f5e6c: b 0x000f5ac0
  000f5e6e: movw r1,#0x2b5
  000f5e72: b 0x000f5ac0
  000f5e74: movw r1,#0x2b6
  000f5e78: b 0x000f5ac0
  000f5e7a: movw r1,#0x2b7
  000f5e7e: b 0x000f5ac0
  000f5e80: mov.w r1,#0x2b8
  000f5e84: b 0x000f5ac0
  000f5e86: movw r1,#0x2b9
  000f5e8a: b 0x000f5ac0
  000f5e8c: movw r1,#0x2ba
  000f5e90: b 0x000f5ac0
  000f5e92: movw r1,#0x2a9
  000f5e96: b 0x000f5ac0
  000f5e98: mov.w r1,#0x2b0
  000f5e9c: b 0x000f5ac0
  000f5e9e: movw r1,#0x2b1
  000f5ea2: b 0x000f5ac0
  000f5ea4: movw r1,#0x2b2
  000f5ea8: b 0x000f5ac0
  000f5eaa: movw r1,#0x2b3
  000f5eae: b 0x000f5ac0
  000f5eb0: mov.w r1,#0x2b4
  000f5eb4: b 0x000f5ac0
  000f5eb6: movw r1,#0x2aa
  000f5eba: b 0x000f5ac0
  000f5ebc: movw r1,#0x2ab
  000f5ec0: b 0x000f5ac0
  000f5ec2: mov.w r1,#0x2ac
  000f5ec6: b 0x000f5ac0
  000f5ec8: movw r1,#0x2ad
  000f5ecc: b 0x000f5ac0
  000f5ece: movw r1,#0x2ae
  000f5ed2: b 0x000f5ac0
```
