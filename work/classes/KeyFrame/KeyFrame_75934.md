# KeyFrame::KeyFrame
elf 0x75934 body 102
Sig: undefined __thiscall KeyFrame(KeyFrame * this)

## decompile
```c

/* AbyssEngine::KeyFrame::KeyFrame() */

void __thiscall AbyssEngine::KeyFrame::KeyFrame(KeyFrame *this)

{
  int iVar1;
  int *piVar2;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_0008599c + 0x8594c);
  local_1c = *piVar2;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  __aeabi_memclr4(this,0x48);
  local_28 = 0x3f800000;
  uStack_24 = 0x3f800000;
  local_20 = 0x3f800000;
  AEMath::Vector::operator=((Vector *)(this + 0xc),(Vector *)&local_28);
  local_28 = 0x3f800000;
  uStack_24 = 0x3f800000;
  local_20 = 0x3f800000;
  AEMath::Vector::operator=((Vector *)(this + 0x30),(Vector *)&local_28);
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x48) = 0x3f800000;
  iVar1 = *piVar2 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00085934: push {r4,r5,r6,r7,lr}
  00085936: add r7,sp,#0xc
  00085938: push.w r8
  0008593c: sub sp,#0x10
  0008593e: mov r4,r0
  00085940: ldr r0,[0x0008599c]
  00085942: mov.w r8,#0x0
  00085946: movs r1,#0x48
  00085948: add r0,pc
  0008594a: ldr r5,[r0,#0x0]
  0008594c: ldr r0,[r5,#0x0]
  0008594e: str r0,[sp,#0xc]
  00085950: mov r0,r4
  00085952: strd r8,r8,[r4,#0x50]
  00085956: blx 0x0006ec14
  0008595a: add.w r0,r4,#0xc
  0008595e: mov.w r6,#0x3f800000
  00085962: mov r1,sp
  00085964: strd r6,r6,[sp,#0x0]
  00085968: str r6,[sp,#0x8]
  0008596a: blx 0x0006eb3c
  0008596e: add.w r0,r4,#0x30
  00085972: mov r1,sp
  00085974: strd r6,r6,[sp,#0x0]
  00085978: str r6,[sp,#0x8]
  0008597a: blx 0x0006eb3c
  0008597e: strd r8,r8,[r4,#0x58]
  00085982: str r6,[r4,#0x48]
  00085984: ldr r0,[sp,#0xc]
  00085986: ldr r1,[r5,#0x0]
  00085988: subs r0,r1,r0
  0008598a: itttt eq
  0008598c: mov.eq r0,r4
  0008598e: add.eq sp,#0x10
  00085990: pop.eq.w r8
  00085994: pop.eq {r4,r5,r6,r7,pc}
  00085996: blx 0x0006e824
```
