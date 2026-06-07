# ParticleSystemSprite::render
elf 0x1833e0 body 130
Sig: undefined __stdcall render(PaintCanvas * param_1, uint param_2, uint param_3, BlendMode param_4)

## decompile
```c

/* ParticleSystemSprite::render(AbyssEngine::PaintCanvas*, unsigned int, unsigned int,
   AbyssEngine::BlendMode) */

void ParticleSystemSprite::render(Matrix *param_1,int param_2,uint param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  undefined4 local_58 [5];
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined4 local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_00193478 + 0x1933f6);
  local_1c = *piVar2;
  if (param_2 != -1) {
    AbyssEngine::PaintCanvas::SetTexture((uint)param_1,param_3);
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)param_1,param_4);
    uStack_28 = DAT_00193470;
    uStack_30 = DAT_00193468;
    uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_38 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar1 = (undefined4 *)((uint)local_58 | 4);
    *puVar1 = 0;
    puVar1[1] = uStack_3c;
    puVar1[2] = uStack_38;
    puVar1[3] = uStack_34;
    local_40 = 0;
    local_58[0] = 0x3f800000;
    local_44 = 0x3f800000;
    local_20 = 0x3f800000;
    AbyssEngine::PaintCanvas::SetWorldViewMatrix(param_1);
    AbyssEngine::PaintCanvas::DrawSpriteSystem((uint)param_1);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001933e0: push {r4,r5,r6,r7,lr}
  001933e2: add r7,sp,#0xc
  001933e4: push.w r8
  001933e8: sub sp,#0x40
  001933ea: mov r5,r0
  001933ec: ldr r0,[0x00193478]
  001933ee: mov r8,r3
  001933f0: mov r4,r1
  001933f2: add r0,pc
  001933f4: ldr r6,[r0,#0x0]
  001933f6: ldr r0,[r6,#0x0]
  001933f8: str r0,[sp,#0x3c]
  001933fa: adds r0,r1,#0x1
  001933fc: beq 0x0019344e
  001933fe: mov r1,r2
  00193400: mov r0,r5
  00193402: mov.w r2,#0xffffffff
  00193406: blx 0x0006fa6c
  0019340a: mov r0,r5
  0019340c: mov r1,r8
  0019340e: blx 0x0006febc
  00193412: adr r0,[0x193468]
  00193414: vmov.i32 q8,#0x0
  00193418: vld1.64 {d18,d19},[r0]
  0019341c: mov r1,sp
  0019341e: orr r0,r1,#0x4
  00193422: vst1.32 {d16,d17},[r0]
  00193426: add.w r0,r1,#0x18
  0019342a: vst1.64 {d16,d17},[r0]
  0019342e: add.w r0,r1,#0x28
  00193432: vst1.64 {d18,d19},[r0]
  00193436: mov.w r0,#0x3f800000
  0019343a: str r0,[sp,#0x0]
  0019343c: str r0,[sp,#0x14]
  0019343e: str r0,[sp,#0x38]
  00193440: mov r0,r5
  00193442: blx 0x0006f55c
  00193446: mov r0,r5
  00193448: mov r1,r4
  0019344a: blx 0x000778f0
  0019344e: ldr r0,[sp,#0x3c]
  00193450: ldr r1,[r6,#0x0]
  00193452: subs r0,r1,r0
  00193454: ittt eq
  00193456: add.eq sp,#0x40
  00193458: pop.eq.w r8
  0019345c: pop.eq {r4,r5,r6,r7,pc}
  0019345e: blx 0x0006e824
```
