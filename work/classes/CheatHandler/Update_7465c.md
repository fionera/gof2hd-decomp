# CheatHandler::Update
elf 0x7465c body 66
Sig: undefined __thiscall Update(CheatHandler * this, ushort param_1)

## decompile
```c

/* AbyssEngine::CheatHandler::Update(unsigned short) */

void __thiscall AbyssEngine::CheatHandler::Update(CheatHandler *this,ushort param_1)

{
  int iVar1;
  uint uVar2;
  
  if (*(int *)this != 0) {
    for (uVar2 = 0; uVar2 < **(uint **)(this + 8); uVar2 = uVar2 + 1) {
      iVar1 = CheatCode::Update(*(CheatCode **)((*(uint **)(this + 8))[1] + uVar2 * 4),param_1);
      if (iVar1 != 0) {
        (**(code **)this)(*(undefined4 *)
                           (*(int *)(*(int *)(*(int *)(this + 8) + 4) + uVar2 * 4) + 4),
                          *(undefined4 *)(this + 4));
      }
    }
  }
  return;
}

```

## target disasm
```
  0008465c: push {r4,r5,r6,r7,lr}
  0008465e: add r7,sp,#0xc
  00084660: push.w r11
  00084664: mov r5,r0
  00084666: ldr r0,[r0,#0x0]
  00084668: mov r4,r1
  0008466a: cbz r0,0x00084698
  0008466c: movs r6,#0x0
  0008466e: b 0x00084690
  00084670: ldr r0,[r0,#0x4]
  00084672: mov r1,r4
  00084674: ldr.w r0,[r0,r6,lsl #0x2]
  00084678: blx 0x0006f718
  0008467c: cbz r0,0x0008468e
  0008467e: ldrd r2,r1,[r5,#0x0]
  00084682: ldr r0,[r5,#0x8]
  00084684: ldr r0,[r0,#0x4]
  00084686: ldr.w r0,[r0,r6,lsl #0x2]
  0008468a: ldr r0,[r0,#0x4]
  0008468c: blx r2
  0008468e: adds r6,#0x1
  00084690: ldr r0,[r5,#0x8]
  00084692: ldr r1,[r0,#0x0]
  00084694: cmp r6,r1
  00084696: bcc 0x00084670
  00084698: pop.w r11
  0008469c: pop {r4,r5,r6,r7,pc}
```
