# SentryGun::update
elf 0x15c3d2 body 148
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* SentryGun::update(int) */

void SentryGun::update(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  Gun::update(*(int *)(param_1 + 8));
  if (*(char *)(*(int *)(param_1 + 8) + 0x4d) != '\0') {
    *(undefined1 *)(*(int *)(param_1 + 8) + 0x4d) = 0;
    iVar1 = *(int *)(param_1 + 0xb0);
    for (iVar3 = iVar1; iVar3 < iVar1 + 3; iVar3 = iVar3 + 1) {
      piVar2 = *(int **)(*(int *)(*(int *)(*(int *)(param_1 + 0xc) + 0xb0) + 4) + iVar3 * 4);
      iVar1 = KIPlayer::isDying();
      if ((iVar1 == 0) &&
         ((iVar1 = Player::isActive(), iVar1 == 0 || (iVar1 = Player::isDead(), iVar1 != 0)))) {
        *(int *)(*(int *)(param_1 + 0xc) + 0x6c) = *(int *)(*(int *)(param_1 + 0xc) + 0x6c) + 1;
        (**(code **)(*piVar2 + 0x18))(piVar2);
        (**(code **)(*piVar2 + 0x44))
                  (piVar2,*(int *)(*(int *)(param_1 + 8) + 0xc) +
                          *(int *)(*(int *)(param_1 + 8) + 0xa0) * 0xc);
        (*(code *)(DAT_001abe14 + 0x1abe18))(piVar2,1);
        return;
      }
      iVar1 = *(int *)(param_1 + 0xb0);
    }
  }
  return;
}

```

## target disasm
```
  0016c3d2: push {r4,r5,r6,r7,lr}
  0016c3d4: add r7,sp,#0xc
  0016c3d6: push.w r11
  0016c3da: mov r4,r0
  0016c3dc: ldr r0,[r0,#0x8]
  0016c3de: blx 0x0007780c
  0016c3e2: ldr r0,[r4,#0x8]
  0016c3e4: ldrb.w r1,[r0,#0x4d]
  0016c3e8: cbz r1,0x0016c428
  0016c3ea: movs r1,#0x0
  0016c3ec: strb.w r1,[r0,#0x4d]
  0016c3f0: ldr.w r0,[r4,#0xb0]
  0016c3f4: mov r6,r0
  0016c3f6: b 0x0016c422
  0016c3f8: ldr r0,[r4,#0xc]
  0016c3fa: ldr.w r0,[r0,#0xb0]
  0016c3fe: ldr r0,[r0,#0x4]
  0016c400: ldr.w r5,[r0,r6,lsl #0x2]
  0016c404: mov r0,r5
  0016c406: blx 0x00071f50
  0016c40a: cbnz r0,0x0016c41c
  0016c40c: ldr r0,[r5,#0x4]
  0016c40e: blx 0x00071f14
  0016c412: cbz r0,0x0016c42e
  0016c414: ldr r0,[r5,#0x4]
  0016c416: blx 0x00075838
  0016c41a: cbnz r0,0x0016c42e
  0016c41c: ldr.w r0,[r4,#0xb0]
  0016c420: adds r6,#0x1
  0016c422: adds r0,#0x3
  0016c424: cmp r6,r0
  0016c426: blt 0x0016c3f8
  0016c428: pop.w r11
  0016c42c: pop {r4,r5,r6,r7,pc}
  0016c42e: ldr r0,[r4,#0xc]
  0016c430: ldr r1,[r0,#0x6c]
  0016c432: adds r1,#0x1
  0016c434: str r1,[r0,#0x6c]
  0016c436: ldr r0,[r5,#0x0]
  0016c438: ldr r1,[r0,#0x18]
  0016c43a: mov r0,r5
  0016c43c: blx r1
  0016c43e: ldr r0,[r4,#0x8]
  0016c440: ldr r1,[r5,#0x0]
  0016c442: ldr r2,[r0,#0xc]
  0016c444: ldr.w r0,[r0,#0xa0]
  0016c448: ldr r3,[r1,#0x44]
  0016c44a: add.w r0,r0,r0, lsl #0x1
  0016c44e: add.w r1,r2,r0, lsl #0x2
  0016c452: mov r0,r5
  0016c454: blx r3
  0016c456: mov r0,r5
  0016c458: movs r1,#0x1
  0016c45a: pop.w r11
  0016c45e: pop.w {r4,r5,r6,r7,lr}
  0016c462: b.w 0x001abe08
```
