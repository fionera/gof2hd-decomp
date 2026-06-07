# Transform::CollectAnimationData
elf 0x74b9a body 162
Sig: undefined __thiscall CollectAnimationData(Transform * this)

## decompile
```c

/* AbyssEngine::Transform::CollectAnimationData() */

void __thiscall AbyssEngine::Transform::CollectAnimationData(Transform *this)

{
  Transform *this_00;
  uint uVar1;
  int iVar2;
  int iVar3;
  Transform *pTVar4;
  uint uVar5;
  
  pTVar4 = this + 0xf8;
  for (uVar5 = 0; uVar5 < *(uint *)(this + 0x3c); uVar5 = uVar5 + 1) {
    iVar2 = *(int *)(*(int *)(this + 0x40) + uVar5 * 4);
    if ((iVar2 != 0) && (this_00 = *(Transform **)(iVar2 + 0x34), this_00 != (Transform *)0x0)) {
      CollectAnimationData(this_00);
      iVar3 = *(int *)(this + 0xfc);
      iVar2 = *(int *)(*(int *)(*(int *)(this + 0x40) + uVar5 * 4) + 0x34);
      uVar1 = *(uint *)(iVar2 + 0xf8);
      iVar2 = *(int *)(iVar2 + 0xfc);
      if ((int)((iVar3 - iVar2) - (uint)(*(uint *)pTVar4 < uVar1)) < 0 !=
          (SBORROW4(iVar3,iVar2) != SBORROW4(iVar3 - iVar2,(uint)(*(uint *)pTVar4 < uVar1)))) {
        *(uint *)pTVar4 = uVar1;
        *(int *)(this + 0xfc) = iVar2;
      }
    }
  }
  uVar5 = 0;
  while (uVar5 < *(uint *)(this + 0x4c)) {
    CollectAnimationData(*(Transform **)(*(int *)(this + 0x50) + uVar5 * 4));
    iVar3 = *(int *)(this + 0xfc);
    iVar2 = *(int *)(*(int *)(this + 0x50) + uVar5 * 4);
    uVar5 = uVar5 + 1;
    uVar1 = *(uint *)(iVar2 + 0xf8);
    iVar2 = *(int *)(iVar2 + 0xfc);
    if ((int)((iVar3 - iVar2) - (uint)(*(uint *)pTVar4 < uVar1)) < 0 !=
        (SBORROW4(iVar3,iVar2) != SBORROW4(iVar3 - iVar2,(uint)(*(uint *)pTVar4 < uVar1)))) {
      *(uint *)pTVar4 = uVar1;
      *(int *)(this + 0xfc) = iVar2;
    }
  }
  if (*(int *)(this + 0x11c) != 0) {
    iVar3 = *(int *)(this + 0xfc);
    iVar2 = *(int *)(*(int *)(this + 0x120) + *(int *)(this + 0x11c) * 4 + -4);
    uVar5 = *(uint *)(iVar2 + 0x50);
    iVar2 = *(int *)(iVar2 + 0x54);
    if ((int)((iVar3 - iVar2) - (uint)(*(uint *)pTVar4 < uVar5)) < 0 !=
        (SBORROW4(iVar3,iVar2) != SBORROW4(iVar3 - iVar2,(uint)(*(uint *)pTVar4 < uVar5)))) {
      *(uint *)pTVar4 = uVar5;
      *(int *)(this + 0xfc) = iVar2;
    }
  }
  return;
}

```

## target disasm
```
  00084b9a: push {r4,r5,r6,r7,lr}
  00084b9c: add r7,sp,#0xc
  00084b9e: push.w r11
  00084ba2: add.w r5,r0,#0xf8
  00084ba6: mov r4,r0
  00084ba8: movs r6,#0x0
  00084baa: b 0x00084bda
  00084bac: ldr r0,[r4,#0x40]
  00084bae: ldr.w r0,[r0,r6,lsl #0x2]
  00084bb2: cbz r0,0x00084bd8
  00084bb4: ldr r0,[r0,#0x34]
  00084bb6: cbz r0,0x00084bd8
  00084bb8: blx 0x0006ecec
  00084bbc: ldr r0,[r4,#0x40]
  00084bbe: ldrd r2,r3,[r5,#0x0]
  00084bc2: ldr.w r0,[r0,r6,lsl #0x2]
  00084bc6: ldr r0,[r0,#0x34]
  00084bc8: ldrd r0,r1,[r0,#0xf8]
  00084bcc: subs r2,r2,r0
  00084bce: sbcs.w r2,r3,r1
  00084bd2: it lt
  00084bd4: strd.lt r0,r1,[r5,#0x0]
  00084bd8: adds r6,#0x1
  00084bda: ldr r0,[r4,#0x3c]
  00084bdc: cmp r6,r0
  00084bde: bcc 0x00084bac
  00084be0: movs r6,#0x0
  00084be2: b 0x00084c0a
  00084be4: ldr r0,[r4,#0x50]
  00084be6: ldr.w r0,[r0,r6,lsl #0x2]
  00084bea: blx 0x0006ecec
  00084bee: ldr r0,[r4,#0x50]
  00084bf0: ldrd r2,r3,[r5,#0x0]
  00084bf4: ldr.w r0,[r0,r6,lsl #0x2]
  00084bf8: adds r6,#0x1
  00084bfa: ldrd r0,r1,[r0,#0xf8]
  00084bfe: subs r2,r2,r0
  00084c00: sbcs.w r2,r3,r1
  00084c04: it lt
  00084c06: strd.lt r0,r1,[r5,#0x0]
  00084c0a: ldr r0,[r4,#0x4c]
  00084c0c: cmp r6,r0
  00084c0e: bcc 0x00084be4
  00084c10: ldr.w r0,[r4,#0x11c]
  00084c14: cbz r0,0x00084c36
  00084c16: ldr.w r1,[r4,#0x120]
  00084c1a: ldrd r2,r3,[r5,#0x0]
  00084c1e: add.w r0,r1,r0, lsl #0x2
  00084c22: ldr.w r0,[r0,#-0x4]
  00084c26: ldrd r0,r1,[r0,#0x50]
  00084c2a: subs r2,r2,r0
  00084c2c: sbcs.w r2,r3,r1
  00084c30: it lt
  00084c32: strd.lt r0,r1,[r5,#0x0]
  00084c36: pop.w r11
  00084c3a: pop {r4,r5,r6,r7,pc}
```
