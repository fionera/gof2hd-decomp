# Gun::setIndex
elf 0x1521ac body 240
Sig: undefined __thiscall setIndex(Gun * this, int param_1)

## decompile
```c

/* Gun::setIndex(int) */

void __thiscall Gun::setIndex(Gun *this,int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  uint uVar3;
  void *pvVar4;
  AEGeometry *this_00;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint *puVar8;
  
  iVar7 = DAT_001622ac;
  *(int *)(this + 0x58) = param_1;
  piVar1 = *(int **)(iVar7 + 0x1621c2);
  this[0x108] = (Gun)(param_1 == 0xe4 || param_1 - 9U < 3);
  uVar2 = Item::getAttribute(*(int *)(*(int *)(*piVar1 + 4) + param_1 * 4));
  iVar7 = DAT_001622b0;
  *(undefined4 *)(this + 100) = uVar2;
  iVar7 = *(int *)(iVar7 + 0x1621f2 + param_1 * 4);
  if (-1 < iVar7) {
    uVar6 = *(uint *)(this + 8);
    uVar3 = (uint)((ulonglong)uVar6 * 4);
    if ((int)((ulonglong)uVar6 * 4 >> 0x20) != 0) {
      uVar3 = 0xffffffff;
    }
    pvVar4 = operator_new__(uVar3);
    *(void **)(this + 0x10c) = pvVar4;
    pvVar4 = operator_new__(uVar6);
    *(void **)(this + 0x110) = pvVar4;
    puVar8 = *(uint **)(DAT_001622b4 + 0x16222a);
    piVar1 = *(int **)(DAT_001622b8 + 0x162232);
    for (uVar3 = 0; uVar3 < uVar6; uVar3 = uVar3 + 1) {
      this_00 = operator_new(0xc0);
      AEGeometry::AEGeometry(this_00,(ushort)iVar7,(PaintCanvas *)*puVar8,false);
      *(undefined4 *)(*(int *)(this + 0x10c) + uVar3 * 4) = *(undefined4 *)(this_00 + 0xc);
      iVar5 = AbyssEngine::AERandom::nextInt(*piVar1);
      *(bool *)(*(int *)(this + 0x110) + uVar3) = iVar5 == 0;
      uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      AbyssEngine::Transform::SetAnimationState(uVar2,0,0);
      pvVar4 = (void *)AEGeometry::~AEGeometry(this_00);
      operator_delete(pvVar4);
      uVar6 = *(uint *)(this + 8);
    }
  }
  return;
}

```

## target disasm
```
  001621ac: push {r4,r5,r6,r7,lr}
  001621ae: add r7,sp,#0xc
  001621b0: push {r8,r9,r10}
  001621b4: mov r4,r0
  001621b6: ldr r0,[0x001622ac]
  001621b8: mov r5,r1
  001621ba: str r1,[r4,#0x58]
  001621bc: subs r1,#0x9
  001621be: add r0,pc
  001621c0: cmp r1,#0x3
  001621c2: sub.w r1,r5,#0xe4
  001621c6: clz r1,r1
  001621ca: mov.w r2,#0x0
  001621ce: ldr r0,[r0,#0x0]
  001621d0: it cc
  001621d2: mov.cc r2,#0x1
  001621d4: lsrs r1,r1,#0x5
  001621d6: orrs r1,r2
  001621d8: strb.w r1,[r4,#0x108]
  001621dc: ldr r0,[r0,#0x0]
  001621de: movs r1,#0xa
  001621e0: ldr r0,[r0,#0x4]
  001621e2: ldr.w r0,[r0,r5,lsl #0x2]
  001621e6: blx 0x000718e4
  001621ea: ldr r1,[0x001622b0]
  001621ec: str r0,[r4,#0x64]
  001621ee: add r1,pc
  001621f0: ldr.w r8,[r1,r5,lsl #0x2]
  001621f4: cmp.w r8,#0x0
  001621f8: blt 0x00162296
  001621fa: ldr r5,[r4,#0x8]
  001621fc: movs r0,#0x4
  001621fe: umull r0,r1,r5,r0
  00162202: cmp r1,#0x0
  00162204: it ne
  00162206: mov.ne r1,#0x1
  00162208: cmp r1,#0x0
  0016220a: it ne
  0016220c: mov.ne.w r0,#0xffffffff
  00162210: blx 0x0006ec08
  00162214: str.w r0,[r4,#0x10c]
  00162218: mov r0,r5
  0016221a: blx 0x0006ec08
  0016221e: str.w r0,[r4,#0x110]
  00162222: movs r6,#0x0
  00162224: ldr r0,[0x001622b4]
  00162226: add r0,pc
  00162228: ldr.w r9,[r0,#0x0]
  0016222c: ldr r0,[0x001622b8]
  0016222e: add r0,pc
  00162230: ldr.w r10,[r0,#0x0]
  00162234: b 0x00162292
  00162236: movs r0,#0xc0
  00162238: blx 0x0006eb24
  0016223c: mov r5,r0
  0016223e: ldr.w r2,[r9,#0x0]
  00162242: uxth.w r1,r8
  00162246: movs r3,#0x0
  00162248: blx 0x0007207c
  0016224c: ldr.w r0,[r4,#0x10c]
  00162250: ldr r1,[r5,#0xc]
  00162252: str.w r1,[r0,r6,lsl #0x2]
  00162256: movs r1,#0x2
  00162258: ldr.w r0,[r10,#0x0]
  0016225c: blx 0x00071848
  00162260: clz r0,r0
  00162264: ldr.w r1,[r4,#0x110]
  00162268: lsrs r0,r0,#0x5
  0016226a: strb r0,[r1,r6]
  0016226c: ldr.w r1,[r4,#0x10c]
  00162270: ldr.w r0,[r9,#0x0]
  00162274: ldr.w r1,[r1,r6,lsl #0x2]
  00162278: blx 0x00072088
  0016227c: movs r1,#0x0
  0016227e: movs r2,#0x0
  00162280: blx 0x0006fd18
  00162284: mov r0,r5
  00162286: blx 0x00071fc8
  0016228a: blx 0x0006eb48
  0016228e: ldr r5,[r4,#0x8]
  00162290: adds r6,#0x1
  00162292: cmp r6,r5
  00162294: bcc 0x00162236
  00162296: pop.w {r8,r9,r10}
  0016229a: pop {r4,r5,r6,r7,pc}
```
