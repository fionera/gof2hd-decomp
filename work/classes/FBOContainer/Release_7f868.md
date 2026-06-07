# FBOContainer::Release
elf 0x7f868 body 106
Sig: undefined __thiscall Release(FBOContainer * this)

## decompile
```c

/* AbyssEngine::FBOContainer::Release() */

void __thiscall AbyssEngine::FBOContainer::Release(FBOContainer *this)

{
  int iVar1;
  code *pcVar2;
  
  if (this[0x18] == (FBOContainer)0x0) {
    return;
  }
  glDeleteFramebuffers(1,this);
  *(undefined4 *)this = 0;
  glDeleteTextures(1,this + 4);
  iVar1 = DAT_0008f8d0;
  *(undefined4 *)(this + 4) = 0;
  pcVar2 = *(code **)(iVar1 + 0x8f896);
  (*pcVar2)(1,this + 8);
  this[0x18] = (FBOContainer)0x0;
  *(undefined4 *)(this + 8) = 0;
  (*pcVar2)(1,this + 0x30);
  (*pcVar2)(1,this + 0x34);
  glBindFramebuffer(0x8d40,0);
  (*(code *)(DAT_001ab464 + 0x1ab468))(1,this + 0x2c);
  return;
}

```

## target disasm
```
  0008f868: push {r4,r5,r6,r7,lr}
  0008f86a: add r7,sp,#0xc
  0008f86c: push.w r11
  0008f870: mov r4,r0
  0008f872: ldrb r0,[r0,#0x18]
  0008f874: cbz r0,0x0008f8ca
  0008f876: movs r0,#0x1
  0008f878: mov r1,r4
  0008f87a: blx 0x00070024
  0008f87e: adds r1,r4,#0x4
  0008f880: movs r5,#0x0
  0008f882: movs r0,#0x1
  0008f884: str r5,[r4,#0x0]
  0008f886: blx 0x0006f2a4
  0008f88a: ldr r0,[0x0008f8d0]
  0008f88c: add.w r1,r4,#0x8
  0008f890: str r5,[r4,#0x4]
  0008f892: add r0,pc
  0008f894: ldr r6,[r0,#0x0]
  0008f896: movs r0,#0x1
  0008f898: blx r6
  0008f89a: add.w r1,r4,#0x30
  0008f89e: movs r0,#0x1
  0008f8a0: strb r5,[r4,#0x18]
  0008f8a2: str r5,[r4,#0x8]
  0008f8a4: blx r6
  0008f8a6: add.w r1,r4,#0x34
  0008f8aa: movs r0,#0x1
  0008f8ac: blx r6
  0008f8ae: movw r0,#0x8d40
  0008f8b2: movs r1,#0x0
  0008f8b4: blx 0x0006ee6c
  0008f8b8: add.w r1,r4,#0x2c
  0008f8bc: movs r0,#0x1
  0008f8be: pop.w r11
  0008f8c2: pop.w {r4,r5,r6,r7,lr}
  0008f8c6: b.w 0x001ab458
  0008f8ca: pop.w r11
  0008f8ce: pop {r4,r5,r6,r7,pc}
  001ab458: bx pc
```
