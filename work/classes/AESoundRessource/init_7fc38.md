# AESoundRessource::init
elf 0x7fc38 body 126
Sig: int __thiscall init(AESoundRessource * this, EVP_PKEY_CTX * ctx)

## decompile
```c

/* AbyssEngine::AESoundRessource::init(int) */

int __thiscall AbyssEngine::AESoundRessource::init(AESoundRessource *this,EVP_PKEY_CTX *ctx)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  AESoundInfo aAStack_2c [4];
  int local_28;
  undefined4 local_24;
  int local_20;
  int local_1c;
  
  piVar4 = *(int **)(DAT_0008fcb8 + 0x8fc4c);
  local_1c = *piVar4;
  local_28 = DAT_0008fcbc + 0x8fc5c;
  getSoundInfo(this,(int)ctx,aAStack_2c,&local_20);
  if (local_20 != -1) {
    iVar3 = *(int *)(this + 8);
    puVar1 = *(undefined4 **)(iVar3 + local_20 * 4);
    if (puVar1 == (undefined4 *)0x0) {
      piVar2 = operator_new(4);
      *piVar2 = *(int *)(DAT_0008fcc0 + 0x8fc7c) + 8;
      *(int **)(iVar3 + local_20 * 4) = piVar2;
      puVar1 = *(undefined4 **)(*(int *)(this + 8) + local_20 * 4);
    }
    (**(code **)*puVar1)(puVar1,local_28);
    piVar2 = *(int **)(*(int *)(this + 8) + local_20 * 4);
    (**(code **)(*piVar2 + 0x24))(piVar2,local_24);
  }
  if (*piVar4 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

```

## target disasm
```
  0008fc38: push {r4,r5,r6,r7,lr}
  0008fc3a: add r7,sp,#0xc
  0008fc3c: push.w r8
  0008fc40: sub sp,#0x18
  0008fc42: mov r4,r0
  0008fc44: ldr r0,[0x0008fcb8]
  0008fc46: add r3,sp,#0x10
  0008fc48: add r0,pc
  0008fc4a: ldr.w r8,[r0,#0x0]
  0008fc4e: ldr.w r0,[r8,#0x0]
  0008fc52: ldr r2,[0x0008fcbc]
  0008fc54: str r0,[sp,#0x14]
  0008fc56: mov r0,r4
  0008fc58: add r2,pc
  0008fc5a: str r2,[sp,#0x8]
  0008fc5c: add r2,sp,#0x4
  0008fc5e: blx 0x000700c0
  0008fc62: ldr r6,[sp,#0x10]
  0008fc64: adds r0,r6,#0x1
  0008fc66: beq 0x0008fca0
  0008fc68: ldr r5,[r4,#0x8]
  0008fc6a: ldr.w r0,[r5,r6,lsl #0x2]
  0008fc6e: cbnz r0,0x0008fc8a
  0008fc70: movs r0,#0x4
  0008fc72: blx 0x0006eb24
  0008fc76: ldr r1,[0x0008fcc0]
  0008fc78: add r1,pc
  0008fc7a: ldr r1,[r1,#0x0]
  0008fc7c: adds r1,#0x8
  0008fc7e: str r1,[r0,#0x0]
  0008fc80: str.w r0,[r5,r6,lsl #0x2]
  0008fc84: ldr r0,[r4,#0x8]
  0008fc86: ldr.w r0,[r0,r6,lsl #0x2]
  0008fc8a: ldr r2,[r0,#0x0]
  0008fc8c: ldr r1,[sp,#0x8]
  0008fc8e: ldr r2,[r2,#0x0]
  0008fc90: blx r2
  0008fc92: ldr r0,[r4,#0x8]
  0008fc94: ldr r1,[sp,#0xc]
  0008fc96: ldr.w r0,[r0,r6,lsl #0x2]
  0008fc9a: ldr r2,[r0,#0x0]
  0008fc9c: ldr r2,[r2,#0x24]
  0008fc9e: blx r2
  0008fca0: ldr r0,[sp,#0x14]
  0008fca2: ldr.w r1,[r8,#0x0]
  0008fca6: subs r0,r1,r0
  0008fca8: ittt eq
  0008fcaa: add.eq sp,#0x18
  0008fcac: pop.eq.w r8
  0008fcb0: pop.eq {r4,r5,r6,r7,pc}
  0008fcb2: blx 0x0006e824
```
