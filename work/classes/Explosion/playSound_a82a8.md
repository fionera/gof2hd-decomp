# Explosion::playSound
elf 0xa82a8 body 212
Sig: undefined __stdcall playSound(Vector * param_1)

## decompile
```c

/* Explosion::playSound(AbyssEngine::AEMath::Vector*) */

void Explosion::playSound(Vector *param_1)

{
  char cVar1;
  int iVar2;
  Vector *in_r1;
  int iVar3;
  Vector *pVVar4;
  float in_s0;
  float extraout_s0;
  
  iVar3 = *(int *)(param_1 + 0x28);
  if (iVar3 < 0) {
    if (*(int *)param_1 - 2U < 4) {
      iVar3 = **(int **)(DAT_000b83a8 + 0xb82ee);
      if (*(char *)(*(int *)(DAT_000b83ac + 0xb82f2) + 0xf) == '\0') {
        in_r1 = (Vector *)0x0;
      }
      pVVar4 = (Vector *)0x15;
      goto LAB_000b8388;
    }
    if (*(int *)param_1 != 0) {
      return;
    }
    iVar3 = **(int **)(DAT_000b839c + 0xb832a);
    iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000b83a0 + 0xb832c));
    cVar1 = *(char *)(*(int *)(DAT_000b83a4 + 0xb8342) + 0xf);
    pVVar4 = (Vector *)0x13;
    in_s0 = extraout_s0;
    if (iVar2 == 0) {
      pVVar4 = (Vector *)0x12;
    }
  }
  else {
    pVVar4 = (Vector *)0xf;
    switch(iVar3) {
    case 0x29:
      break;
    case 0x2a:
      pVVar4 = (Vector *)0x10;
      break;
    case 0x2b:
      pVVar4 = (Vector *)0x11;
      break;
    case 0x2c:
      pVVar4 = (Vector *)0xe;
      break;
    case 0x2d:
      pVVar4 = (Vector *)0xd;
      break;
    case 0x2e:
switchD_000b82be_caseD_2e:
      pVVar4 = (Vector *)0xc;
      break;
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
      return;
    case 0x3c:
    case 0x3d:
    case 0x3e:
switchD_000b82be_caseD_3c:
      pVVar4 = (Vector *)0x16;
      break;
    default:
      if (iVar3 - 0xb0U < 3) goto switchD_000b82be_caseD_3c;
      if (iVar3 == 0xb3) goto switchD_000b82be_caseD_2e;
      if (iVar3 == 0xc5 || iVar3 == 0xdd) {
        pVVar4 = (Vector *)0x8cd;
      }
      else {
        if (iVar3 != 0xe8) {
          return;
        }
        pVVar4 = (Vector *)0x8e7;
      }
    }
    iVar3 = **(int **)(DAT_000b8394 + 0xb8378);
    cVar1 = *(char *)(*(int *)(DAT_000b8398 + 0xb837c) + 0xf);
  }
  if (cVar1 == '\0') {
    in_r1 = (Vector *)0x0;
  }
LAB_000b8388:
  FModSound::play(iVar3,pVVar4,in_r1,in_s0);
  return;
}

```

## target disasm
```
  000b82a8: push {r2,r3,r4,r5,r7,lr}
  000b82aa: add r7,sp,#0x10
  000b82ac: mov r4,r1
  000b82ae: ldr r1,[r0,#0x28]
  000b82b0: cmp r1,#0x0
  000b82b2: blt 0x000b82dc
  000b82b4: sub.w r0,r1,#0x29
  000b82b8: cmp r0,#0x15
  000b82ba: bhi 0x000b8302
  000b82bc: movs r1,#0xf
  000b82be: tbb [pc,r0]
  000b82d8: movs r1,#0x10
  000b82da: b 0x000b836e
  000b82dc: ldr r0,[r0,#0x0]
  000b82de: subs r1,r0,#0x2
  000b82e0: cmp r1,#0x4
  000b82e2: bcs 0x000b8320
  000b82e4: ldr r0,[0x000b83a8]
  000b82e6: movs r2,#0x0
  000b82e8: ldr r1,[0x000b83ac]
  000b82ea: add r0,pc
  000b82ec: str r2,[sp,#0x0]
  000b82ee: add r1,pc
  000b82f0: ldr r0,[r0,#0x0]
  000b82f2: ldr r1,[r1,#0x0]
  000b82f4: ldr r0,[r0,#0x0]
  000b82f6: ldrb r1,[r1,#0xf]
  000b82f8: cmp r1,#0x0
  000b82fa: it eq
  000b82fc: mov.eq r4,r1
  000b82fe: movs r1,#0x15
  000b8300: b 0x000b8388
  000b8302: sub.w r0,r1,#0xb0
  000b8306: cmp r0,#0x3
  000b8308: bcc 0x000b831c
  000b830a: cmp r1,#0xb3
  000b830c: beq 0x000b8354
  000b830e: cmp r1,#0xc5
  000b8310: it ne
  000b8312: cmp.ne r1,#0xdd
  000b8314: bne 0x000b8358
  000b8316: movw r1,#0x8cd
  000b831a: b 0x000b836e
  000b831c: movs r1,#0x16
  000b831e: b 0x000b836e
  000b8320: cbnz r0,0x000b8362
  000b8322: ldr r0,[0x000b839c]
  000b8324: ldr r1,[0x000b83a0]
  000b8326: add r0,pc
  000b8328: add r1,pc
  000b832a: ldr r0,[r0,#0x0]
  000b832c: ldr r1,[r1,#0x0]
  000b832e: ldr r5,[r0,#0x0]
  000b8330: ldr r0,[r1,#0x0]
  000b8332: movs r1,#0x2
  000b8334: blx 0x00071848
  000b8338: ldr r1,[0x000b83a4]
  000b833a: cmp r0,#0x0
  000b833c: mov r0,r5
  000b833e: add r1,pc
  000b8340: ldr r1,[r1,#0x0]
  000b8342: ldrb r2,[r1,#0xf]
  000b8344: mov.w r1,#0x0
  000b8348: str r1,[sp,#0x0]
  000b834a: mov.w r1,#0x13
  000b834e: it eq
  000b8350: mov.eq r1,#0x12
  000b8352: b 0x000b8382
  000b8354: movs r1,#0xc
  000b8356: b 0x000b836e
  000b8358: cmp r1,#0xe8
  000b835a: bne 0x000b8362
  000b835c: movw r1,#0x8e7
  000b8360: b 0x000b836e
  000b8362: pop {r2,r3,r4,r5,r7,pc}
  000b8364: movs r1,#0x11
  000b8366: b 0x000b836e
  000b8368: movs r1,#0xe
  000b836a: b 0x000b836e
  000b836c: movs r1,#0xd
  000b836e: ldr r0,[0x000b8394]
  000b8370: movs r3,#0x0
  000b8372: ldr r2,[0x000b8398]
  000b8374: add r0,pc
  000b8376: str r3,[sp,#0x0]
  000b8378: add r2,pc
  000b837a: ldr r0,[r0,#0x0]
  000b837c: ldr r2,[r2,#0x0]
  000b837e: ldr r0,[r0,#0x0]
  000b8380: ldrb r2,[r2,#0xf]
  000b8382: cmp r2,#0x0
  000b8384: it eq
  000b8386: mov.eq r4,r2
  000b8388: mov r2,r4
  000b838a: movs r3,#0x0
  000b838c: blx 0x00071548
  000b8390: pop {r2,r3,r4,r5,r7,pc}
```
