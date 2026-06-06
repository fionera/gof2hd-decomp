# Player::playShootSound
elf 0xa3a64 body 164
Sig: undefined __stdcall playShootSound(int param_1, int param_2, Vector * param_3, float param_4)

## decompile
```c

/* Player::playShootSound(int, int, AbyssEngine::AEMath::Vector*, float) */

void Player::playShootSound(int param_1,int param_2,Vector *param_3,float param_4)

{
  uint uVar1;
  int iVar2;
  Vector *in_r3;
  int iVar3;
  int *piVar4;
  float extraout_s0;
  
  if (*(int *)(param_1 + 0xd0) == 0) {
    iVar3 = *(int *)(DAT_000b3b08 + 0xb3a8c + param_2 * 4);
  }
  else {
    uVar1 = *(uint *)(*(int *)(param_1 + 0xd0) + 0x28);
    if (uVar1 < 0xb) {
      iVar3 = *(int *)(DAT_000b3b0c + 0xb3a82 + uVar1 * 4);
    }
    else {
      iVar3 = 0x3d;
    }
  }
  piVar4 = *(int **)(DAT_000b3b10 + 0xb3a9a);
  iVar2 = *piVar4;
  if ((param_3 < (Vector *)0x9) && ((1 << ((uint)param_3 & 0xff) & 0x10cU) != 0)) {
    iVar2 = FModSound::isPlaying(iVar2);
    if (iVar2 != 0) {
      if (*(char *)(*(int *)(DAT_000b3b18 + 0xb3ab6) + 0xf) != '\0') {
        FModSound::updateEvent3DAttributes((FModSound *)*piVar4,iVar3,in_r3,(Vector *)0x0,false);
      }
      return;
    }
    if (*(char *)(*(int *)(DAT_000b3b14 + 0xb3ad8) + 0xf) == '\0') {
      in_r3 = (Vector *)0x0;
    }
    iVar2 = *piVar4;
    param_4 = extraout_s0;
  }
  else if (*(char *)(*(int *)(DAT_000b3b1c + 0xb3aea) + 0xf) == '\0') {
    in_r3 = (Vector *)0x0;
  }
  (*(code *)(DAT_001abd34 + 0x1abd38))(param_4,iVar2,iVar3,in_r3,0);
  return;
}

```
## target disasm
```
  000b3a64: push {r4,r5,r6,r7,lr}
  000b3a66: add r7,sp,#0xc
  000b3a68: push.w r11
  000b3a6c: sub sp,#0x8
  000b3a6e: ldr.w r0,[r0,#0xd0]
  000b3a72: mov r4,r3
  000b3a74: cbz r0,0x000b3a86
  000b3a76: ldr r0,[r0,#0x28]
  000b3a78: cmp r0,#0xa
  000b3a7a: bhi 0x000b3a90
  000b3a7c: ldr r1,[0x000b3b0c]
  000b3a7e: add r1,pc
  000b3a80: ldr.w r5,[r1,r0,lsl #0x2]
  000b3a84: b 0x000b3a92
  000b3a86: ldr r0,[0x000b3b08]
  000b3a88: add r0,pc
  000b3a8a: ldr.w r5,[r0,r1,lsl #0x2]
  000b3a8e: b 0x000b3a92
  000b3a90: movs r5,#0x3d
  000b3a92: ldr r0,[0x000b3b10]
  000b3a94: cmp r2,#0x8
  000b3a96: add r0,pc
  000b3a98: ldr r6,[r0,#0x0]
  000b3a9a: ldr r0,[r6,#0x0]
  000b3a9c: bhi 0x000b3ae4
  000b3a9e: movs r1,#0x1
  000b3aa0: lsls r1,r2
  000b3aa2: tst r1,#0x10c
  000b3aa6: beq 0x000b3ae4
  000b3aa8: mov r1,r5
  000b3aaa: blx 0x0007153c
  000b3aae: cbz r0,0x000b3ad2
  000b3ab0: ldr r0,[0x000b3b18]
  000b3ab2: add r0,pc
  000b3ab4: ldr r0,[r0,#0x0]
  000b3ab6: ldrb r0,[r0,#0xf]
  000b3ab8: cbz r0,0x000b3aca
  000b3aba: ldr r0,[r6,#0x0]
  000b3abc: movs r1,#0x0
  000b3abe: str r1,[sp,#0x0]
  000b3ac0: mov r1,r5
  000b3ac2: mov r2,r4
  000b3ac4: movs r3,#0x0
  000b3ac6: blx 0x000726e8
  000b3aca: add sp,#0x8
  000b3acc: pop.w r11
  000b3ad0: pop {r4,r5,r6,r7,pc}
  000b3ad2: ldr r0,[0x000b3b14]
  000b3ad4: add r0,pc
  000b3ad6: ldr r0,[r0,#0x0]
  000b3ad8: ldrb r0,[r0,#0xf]
  000b3ada: cmp r0,#0x0
  000b3adc: it eq
  000b3ade: mov.eq r4,r0
  000b3ae0: ldr r0,[r6,#0x0]
  000b3ae2: b 0x000b3af2
  000b3ae4: ldr r1,[0x000b3b1c]
  000b3ae6: add r1,pc
  000b3ae8: ldr r1,[r1,#0x0]
  000b3aea: ldrb r1,[r1,#0xf]
  000b3aec: cmp r1,#0x0
  000b3aee: it eq
  000b3af0: mov.eq r4,r1
  000b3af2: mov r1,r5
  000b3af4: mov r2,r4
  000b3af6: movs r3,#0x0
  000b3af8: add sp,#0x8
  000b3afa: pop.w r11
  000b3afe: pop.w {r4,r5,r6,r7,lr}
  000b3b02: b.w 0x001abd28
  001abd28: bx pc
```
