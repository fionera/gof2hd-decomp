# FModSound::setDownPitch
elf 0x93a74 body 94
Sig: undefined __stdcall setDownPitch(bool param_1)

## decompile
```c

/* FModSound::setDownPitch(bool) */

void FModSound::setDownPitch(bool param_1)

{
  uint uVar1;
  int iVar2;
  int in_r1;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  uVar1 = (uint)param_1;
  iVar4 = 0;
  uVar5 = DAT_000a3ad4;
  if (in_r1 != 0) {
    uVar5 = 0xbf800000;
  }
  *(char *)(uVar1 + 8) = (char)in_r1;
  for (; iVar4 != 0x8f5; iVar4 = iVar4 + 1) {
    if (((*(int *)(uVar1 + 0x23fc) != 0) && (iVar3 = uVar1 + iVar4 * 4, *(int *)(iVar3 + 0x18) != 0)
        ) && (iVar2 = isPlaying(uVar1), iVar2 != 0)) {
      FMOD::Event::setPitch(*(undefined4 *)(iVar3 + 0x18),uVar5,1);
    }
  }
  return;
}

```

## target disasm
```
  000a3a74: push {r4,r5,r6,r7,lr}
  000a3a76: add r7,sp,#0xc
  000a3a78: push {r8,r9,r10}
  000a3a7c: vmov.f32 s2,0xbf800000
  000a3a80: vldr.32 s0,[pc,#0x50]
  000a3a84: cmp r1,#0x0
  000a3a86: mov r10,r0
  000a3a88: mov.w r5,#0x0
  000a3a8c: movw r9,#0x8f5
  000a3a90: it ne
  000a3a92: vmov.ne.f32 s0,s2
  000a3a96: strb r1,[r0,#0x8]
  000a3a98: vmov r8,s0
  000a3a9c: movw r0,#0x23fc
  000a3aa0: add.w r6,r10,r0
  000a3aa4: b 0x000a3ac8
  000a3aa6: ldr r0,[r6,#0x0]
  000a3aa8: cbz r0,0x000a3ac6
  000a3aaa: add.w r4,r10,r5, lsl #0x2
  000a3aae: ldr r0,[r4,#0x18]
  000a3ab0: cbz r0,0x000a3ac6
  000a3ab2: mov r0,r10
  000a3ab4: mov r1,r5
  000a3ab6: blx 0x0007153c
  000a3aba: cbz r0,0x000a3ac6
  000a3abc: ldr r0,[r4,#0x18]
  000a3abe: mov r1,r8
  000a3ac0: movs r2,#0x1
  000a3ac2: blx 0x00071560
  000a3ac6: adds r5,#0x1
  000a3ac8: cmp r5,r9
  000a3aca: bne 0x000a3aa6
  000a3acc: pop.w {r8,r9,r10}
  000a3ad0: pop {r4,r5,r6,r7,pc}
```
