# BeamGun::BeamGun
elf 0x177754 body 186
Sig: undefined __thiscall BeamGun(BeamGun * this, int param_1, Gun * param_2, int param_3, Level * param_4)

## decompile
```c

/* BeamGun::BeamGun(int, Gun*, int, Level*) */

BeamGun * __thiscall
BeamGun::BeamGun(BeamGun *this,int param_1,Gun *param_2,int param_3,Level *param_4)

{
  ushort uVar1;
  AEGeometry *pAVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  iVar4 = param_3 + -9;
  if (param_3 == 0xe4) {
    iVar4 = 2;
  }
  *(int *)this = *(int *)(DAT_00187828 + 0x18776a) + 8;
  *(undefined4 *)(this + 4) = 0;
  *(Gun **)(this + 8) = param_2;
  *(Level **)(this + 0xc) = param_4;
  *(int *)(this + 0x10) = param_1;
  *(int *)(this + 0x14) = iVar4;
  pAVar2 = operator_new(0xc0);
  puVar5 = *(undefined4 **)(DAT_0018782c + 0x187794);
  uVar1 = (short)iVar4 + 0x3795;
  if (param_3 == 0xe4) {
    uVar1 = 0x4a92;
  }
  AEGeometry::AEGeometry(pAVar2,uVar1,(PaintCanvas *)*puVar5,false);
  *(AEGeometry **)(this + 0x18) = pAVar2;
  iVar4 = Gun::isPlayerGun();
  if ((iVar4 == 0) ||
     (iVar4 = *(int *)(DAT_00187830 + 0x1877c6 + *(int *)(param_2 + 0x58) * 4), iVar4 < 0)) {
    pAVar2 = (AEGeometry *)0x0;
    this[0x20] = (BeamGun)0x0;
  }
  else {
    iVar3 = *(int *)(param_2 + 0x5c);
    this[0x20] = (BeamGun)(iVar3 != 0xb);
    if (iVar3 == 0xb) {
      pAVar2 = (AEGeometry *)0x0;
    }
    else {
      pAVar2 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar2,(ushort)iVar4,(PaintCanvas *)*puVar5,false);
    }
  }
  this[0x21] = (BeamGun)0x0;
  *(AEGeometry **)(this + 0x1c) = pAVar2;
  return this;
}

```

## target disasm
```
  00187754: push {r4,r5,r6,r7,lr}
  00187756: add r7,sp,#0xc
  00187758: push {r8,r9,r10}
  0018775c: mov r4,r0
  0018775e: ldr r0,[0x00187828]
  00187760: mov r6,r3
  00187762: sub.w r5,r6,#0x9
  00187766: add r0,pc
  00187768: mov r8,r2
  0018776a: cmp r6,#0xe4
  0018776c: ldr r2,[r7,#0x8]
  0018776e: ldr r0,[r0,#0x0]
  00187770: mov.w r3,#0x0
  00187774: it eq
  00187776: mov.eq r5,#0x2
  00187778: adds r0,#0x8
  0018777a: stm r4,{r0,r3,r8}
  0018777e: strd r2,r1,[r4,#0xc]
  00187782: str r5,[r4,#0x14]
  00187784: movs r0,#0xc0
  00187786: blx 0x0006eb24
  0018778a: mov r10,r0
  0018778c: ldr r0,[0x0018782c]
  0018778e: cmp r6,#0xe4
  00187790: add r0,pc
  00187792: ldr.w r9,[r0,#0x0]
  00187796: movw r0,#0x3795
  0018779a: add r0,r5
  0018779c: ldr.w r2,[r9,#0x0]
  001877a0: it eq
  001877a2: movw.eq r0,#0x4a92
  001877a6: uxth r1,r0
  001877a8: mov r0,r10
  001877aa: movs r3,#0x0
  001877ac: blx 0x0007207c
  001877b0: str.w r10,[r4,#0x18]
  001877b4: mov r0,r8
  001877b6: blx 0x0007798c
  001877ba: cbz r0,0x001877f4
  001877bc: ldr r0,[0x00187830]
  001877be: ldr.w r1,[r8,#0x58]
  001877c2: add r0,pc
  001877c4: ldr.w r6,[r0,r1,lsl #0x2]
  001877c8: cmp.w r6,#0xffffffff
  001877cc: ble 0x001877f4
  001877ce: ldr.w r0,[r8,#0x5c]
  001877d2: subs r0,#0xb
  001877d4: it ne
  001877d6: mov.ne r0,#0x1
  001877d8: strb.w r0,[r4,#0x20]
  001877dc: beq 0x0018780a
  001877de: movs r0,#0xc0
  001877e0: blx 0x0006eb24
  001877e4: mov r5,r0
  001877e6: ldr.w r2,[r9,#0x0]
  001877ea: uxth r1,r6
  001877ec: movs r3,#0x0
  001877ee: blx 0x0007207c
  001877f2: b 0x001877fa
  001877f4: movs r5,#0x0
  001877f6: strb.w r5,[r4,#0x20]
  001877fa: movs r0,#0x0
  001877fc: strb.w r0,[r4,#0x21]
  00187800: mov r0,r4
  00187802: str r5,[r4,#0x1c]
  00187804: pop.w {r8,r9,r10}
  00187808: pop {r4,r5,r6,r7,pc}
  0018780a: movs r5,#0x0
  0018780c: b 0x001877fa
```
