# Transform::AddKeyFrame
elf 0x758a0 body 130
Sig: undefined __thiscall AddKeyFrame(Transform * this, Vector * param_1, Vector * param_2, Vector * param_3, Vector * param_4, Vector * param_5, Vector * param_6, int param_7)

## decompile
```c

/* AbyssEngine::Transform::AddKeyFrame(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&, int) */

void __thiscall
AbyssEngine::Transform::AddKeyFrame
          (Transform *this,Vector *param_1,Vector *param_2,Vector *param_3,Vector *param_4,
          Vector *param_5,Vector *param_6,int param_7)

{
  int iVar1;
  KeyFrame *this_00;
  int iVar2;
  code *pcVar3;
  bool bVar4;
  
  this_00 = operator_new(0x60);
  KeyFrame::KeyFrame(this_00);
  pcVar3 = *(code **)(DAT_00085930 + 0x858ca);
  (*pcVar3)(this_00 + 0x18,param_2);
  (*pcVar3)(this_00,param_3);
  (*pcVar3)(this_00 + 0xc,param_1);
  (*pcVar3)(this_00 + 0x3c,param_5);
  (*pcVar3)(this_00 + 0x24,param_6);
  (*pcVar3)(this_00 + 0x30,param_4);
  iVar1 = param_7 >> 0x1f;
  *(int *)(this_00 + 0x50) = param_7;
  *(int *)(this_00 + 0x54) = iVar1;
  ArrayAdd<AbyssEngine::KeyFrame*>(this_00,(Array *)(this + 0x11c));
  iVar2 = *(int *)(this + 0xfc);
  bVar4 = *(uint *)(this + 0xf8) < (uint)param_7;
  if ((int)((iVar2 - iVar1) - (uint)bVar4) < 0 !=
      (SBORROW4(iVar2,iVar1) != SBORROW4(iVar2 - iVar1,(uint)bVar4))) {
    *(int *)(this + 0xf8) = param_7;
    *(int *)(this + 0xfc) = iVar1;
  }
  return;
}

```

## target disasm
```
  000858a0: push {r4,r5,r6,r7,lr}
  000858a2: add r7,sp,#0xc
  000858a4: push {r7,r8,r9,r10,r11}
  000858a8: mov r4,r0
  000858aa: movs r0,#0x60
  000858ac: mov r10,r3
  000858ae: mov r6,r2
  000858b0: mov r9,r1
  000858b2: blx 0x0006eb24
  000858b6: mov r5,r0
  000858b8: blx 0x0006f838
  000858bc: ldr r1,[0x00085930]
  000858be: add.w r0,r5,#0x18
  000858c2: ldr.w r11,[r7,#0x14]
  000858c6: add r1,pc
  000858c8: ldr.w r8,[r1,#0x0]
  000858cc: mov r1,r6
  000858ce: blx r8
  000858d0: mov r0,r5
  000858d2: mov r1,r10
  000858d4: blx r8
  000858d6: add.w r0,r5,#0xc
  000858da: mov r1,r9
  000858dc: blx r8
  000858de: ldr r1,[r7,#0xc]
  000858e0: add.w r0,r5,#0x3c
  000858e4: blx r8
  000858e6: ldr r1,[r7,#0x10]
  000858e8: add.w r0,r5,#0x24
  000858ec: blx r8
  000858ee: ldr r1,[r7,#0x8]
  000858f0: add.w r0,r5,#0x30
  000858f4: blx r8
  000858f6: add.w r1,r4,#0x11c
  000858fa: asr.w r6,r11, asr #0x1f
  000858fe: mov r0,r5
  00085900: strd r11,r6,[r5,#0x50]
  00085904: blx 0x0006f844
  00085908: ldr.w r0,[r4,#0xf8]!
  0008590c: ldr r1,[r4,#0x4]
  0008590e: subs.w r0,r0,r11
  00085912: sbcs.w r0,r1,r11, asr #0x1f
  00085916: it lt
  00085918: strd.lt r11,r6,[r4,#0x0]
  0008591c: pop.w {r3,r8,r9,r10,r11}
  00085920: pop {r4,r5,r6,r7,pc}
```
