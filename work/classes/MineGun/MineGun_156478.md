# MineGun::MineGun
elf 0x156478 body 246
Sig: undefined __thiscall MineGun(MineGun * this, Gun * param_1, int param_2, int param_3, int param_4, Level * param_5)

## decompile
```c

/* MineGun::MineGun(Gun*, int, int, int, Level*) */

MineGun * __thiscall
MineGun::MineGun(MineGun *this,Gun *param_1,int param_2,int param_3,int param_4,Level *param_5)

{
  Array<Explosion*> *this_00;
  void *pvVar1;
  Explosion *this_01;
  AEGeometry *this_02;
  undefined4 uVar2;
  int iVar3;
  uint *puVar4;
  uint uVar5;
  
  ObjectGun::ObjectGun((ObjectGun *)this,param_3,param_1,param_2,0,param_5);
  iVar3 = DAT_0016658c;
  *(undefined4 *)(this + 0xc0) = 0;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(int *)this = *(int *)(iVar3 + 0x1664a4) + 8;
  this_00 = operator_new(0xc);
  Array<Explosion*>::Array(this_00);
  *(Array<Explosion*> **)(this + 0xb4) = this_00;
  ArraySetLength<Explosion*>(*(uint *)(param_1 + 8),(Array *)this_00);
  puVar4 = *(uint **)(this + 0xb4);
  pvVar1 = operator_new__(*puVar4);
  *(void **)(this + 0xb8) = pvVar1;
  for (uVar5 = 0; uVar5 < *puVar4; uVar5 = uVar5 + 1) {
    this_01 = operator_new(0x68);
    iVar3 = 0;
    if (*(int *)(param_1 + 0x60) == 0) {
      iVar3 = 7;
    }
    Explosion::Explosion(this_01,iVar3);
    *(Explosion **)(*(int *)(*(int *)(this + 0xb4) + 4) + uVar5 * 4) = this_01;
    Explosion::setWeaponIndex
              (*(Explosion **)(*(int *)(*(int *)(this + 0xb4) + 4) + uVar5 * 4),
               *(int *)(param_1 + 0x58));
    *(undefined1 *)(*(int *)(this + 0xb8) + uVar5) = 1;
    puVar4 = *(uint **)(this + 0xb4);
  }
  this_02 = operator_new(0xc0);
  puVar4 = *(uint **)(DAT_00166590 + 0x166534);
  AEGeometry::AEGeometry(this_02,(short)param_2 + 1,(PaintCanvas *)*puVar4,false);
  *(AEGeometry **)(this + 0xbc) = this_02;
  AbyssEngine::PaintCanvas::TransformAddChild
            ((PaintCanvas *)*puVar4,*(uint *)(this + 0x10),*(uint *)(this_02 + 0xc));
  uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar4);
  AbyssEngine::Transform::SetAnimationState(uVar2,2,0);
  return this;
}

```

## target disasm
```
  00166478: push {r4,r5,r6,r7,lr}
  0016647a: add r7,sp,#0xc
  0016647c: push {r6,r7,r8,r9,r10}
  00166480: mov r4,r0
  00166482: ldr r0,[r7,#0xc]
  00166484: mov r9,r1
  00166486: mov r8,r2
  00166488: movs r5,#0x0
  0016648a: mov r1,r3
  0016648c: strd r5,r0,[sp,#0x0]
  00166490: mov r0,r4
  00166492: mov r2,r9
  00166494: mov r3,r8
  00166496: blx 0x0007759c
  0016649a: ldr r0,[0x0016658c]
  0016649c: strd r5,r5,[r4,#0xc0]
  001664a0: add r0,pc
  001664a2: str.w r5,[r4,#0xc8]
  001664a6: ldr r0,[r0,#0x0]
  001664a8: adds r0,#0x8
  001664aa: str r0,[r4,#0x0]
  001664ac: movs r0,#0xc
  001664ae: blx 0x0006eb24
  001664b2: mov r5,r0
  001664b4: blx 0x000775a8
  001664b8: str.w r5,[r4,#0xb4]
  001664bc: ldr.w r0,[r9,#0x8]
  001664c0: mov r1,r5
  001664c2: blx 0x000775b4
  001664c6: ldr.w r5,[r4,#0xb4]
  001664ca: ldr r0,[r5,#0x0]
  001664cc: blx 0x0006ec08
  001664d0: movs r6,#0x0
  001664d2: mov.w r10,#0x1
  001664d6: str.w r0,[r4,#0xb8]
  001664da: b 0x00166520
  001664dc: movs r0,#0x68
  001664de: blx 0x0006eb24
  001664e2: mov r5,r0
  001664e4: ldr.w r0,[r9,#0x60]
  001664e8: movs r1,#0x0
  001664ea: cmp r0,#0x0
  001664ec: it eq
  001664ee: mov.eq r1,#0x7
  001664f0: mov r0,r5
  001664f2: blx 0x000727f0
  001664f6: ldr.w r0,[r4,#0xb4]
  001664fa: ldr r0,[r0,#0x4]
  001664fc: str.w r5,[r0,r6,lsl #0x2]
  00166500: ldr.w r0,[r4,#0xb4]
  00166504: ldr.w r1,[r9,#0x58]
  00166508: ldr r0,[r0,#0x4]
  0016650a: ldr.w r0,[r0,r6,lsl #0x2]
  0016650e: blx 0x00076dc8
  00166512: ldr.w r0,[r4,#0xb8]
  00166516: strb.w r10,[r0,r6]
  0016651a: adds r6,#0x1
  0016651c: ldr.w r5,[r4,#0xb4]
  00166520: ldr r0,[r5,#0x0]
  00166522: cmp r6,r0
  00166524: bcc 0x001664dc
  00166526: movs r0,#0xc0
  00166528: blx 0x0006eb24
  0016652c: mov r5,r0
  0016652e: ldr r0,[0x00166590]
  00166530: add r0,pc
  00166532: ldr r6,[r0,#0x0]
  00166534: ldr r2,[r6,#0x0]
  00166536: add.w r0,r8,#0x1
  0016653a: movs r3,#0x0
  0016653c: uxth r1,r0
  0016653e: mov r0,r5
  00166540: blx 0x0007207c
  00166544: ldr r1,[r4,#0x10]
  00166546: str.w r5,[r4,#0xbc]
  0016654a: ldr r0,[r6,#0x0]
  0016654c: ldr r2,[r5,#0xc]
  0016654e: blx 0x000720d0
  00166552: ldr.w r0,[r4,#0xbc]
  00166556: ldr r1,[r0,#0xc]
  00166558: ldr r0,[r6,#0x0]
  0016655a: blx 0x00072088
  0016655e: movs r1,#0x2
  00166560: movs r2,#0x0
  00166562: blx 0x0006fd18
  00166566: mov r0,r4
  00166568: pop.w {r2,r3,r8,r9,r10}
  0016656c: pop {r4,r5,r6,r7,pc}
```
