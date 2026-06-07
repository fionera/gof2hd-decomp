# FModSound::FModSound
elf 0x92b70 body 100
Sig: undefined __thiscall FModSound(FModSound * this)

## decompile
```c

/* FModSound::FModSound() */

FModSound * __thiscall FModSound::FModSound(FModSound *this)

{
  undefined4 uVar1;
  int iVar2;
  
  *(undefined4 *)(this + 0x2400) = 0;
  *(undefined4 *)(this + 0x23fc) = 0;
  *(undefined4 *)(this + 0x2404) = 0;
  uVar1 = AEFile::GetAppRootDir();
  *(undefined4 *)(this + 0xc) = uVar1;
  this[0x10] = (FModSound)0x1;
  for (iVar2 = 0x11; iVar2 != 0x15; iVar2 = iVar2 + 1) {
    this[iVar2] = (FModSound)0x1;
  }
  for (iVar2 = 0; iVar2 != 0x23d4; iVar2 = iVar2 + 4) {
    *(undefined4 *)(this + iVar2 + 0x18) = 0;
  }
  *(undefined4 *)(this + 0x2438) = 0;
  *(undefined4 *)(this + 0x2434) = 0;
  *(undefined4 *)(this + 0x2424) = 0;
  *(undefined4 *)(this + 0x2428) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x242c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x2430) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return this;
}

```

## target disasm
```
  000a2b70: push {r4,r6,r7,lr}
  000a2b72: add r7,sp,#0x8
  000a2b74: mov r4,r0
  000a2b76: mov.w r0,#0x2400
  000a2b7a: movs r1,#0x0
  000a2b7c: str r1,[r4,r0]
  000a2b7e: movw r0,#0x23fc
  000a2b82: str r1,[r4,r0]
  000a2b84: movw r0,#0x2404
  000a2b88: str r1,[r4,r0]
  000a2b8a: blx 0x00071440
  000a2b8e: str r0,[r4,#0xc]
  000a2b90: movs r0,#0x1
  000a2b92: movs r1,#0x11
  000a2b94: strb r0,[r4,#0x10]
  000a2b96: b 0x000a2b9c
  000a2b98: strb r0,[r4,r1]
  000a2b9a: adds r1,#0x1
  000a2b9c: cmp r1,#0x15
  000a2b9e: bne 0x000a2b98
  000a2ba0: movs r0,#0x0
  000a2ba2: movw r1,#0x23d4
  000a2ba6: movs r2,#0x0
  000a2ba8: b 0x000a2bb0
  000a2baa: adds r3,r4,r2
  000a2bac: adds r2,#0x4
  000a2bae: str r0,[r3,#0x18]
  000a2bb0: cmp r2,r1
  000a2bb2: bne 0x000a2baa
  000a2bb4: movw r0,#0x2438
  000a2bb8: movs r1,#0x0
  000a2bba: str r1,[r4,r0]
  000a2bbc: movw r0,#0x2434
  000a2bc0: vmov.i32 q8,#0x0
  000a2bc4: str r1,[r4,r0]
  000a2bc6: movw r0,#0x2424
  000a2bca: add r0,r4
  000a2bcc: vst1.32 {d16,d17},[r0]
  000a2bd0: mov r0,r4
  000a2bd2: pop {r4,r6,r7,pc}
```
