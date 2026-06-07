# StarSystem::render2D
elf 0x1354e8 body 160
Sig: undefined __stdcall render2D(void)

## decompile
```c

/* StarSystem::render2D() */

void StarSystem::render2D(void)

{
  int in_r0;
  int iVar1;
  uint uVar2;
  uint *puVar3;
  int *piVar4;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float local_28;
  float local_24;
  float local_20;
  int local_1c;
  
  piVar4 = *(int **)(DAT_0014558c + 0x1454fa);
  local_1c = *piVar4;
  if (*(int *)(in_r0 + 0x2c) != 0) {
    puVar3 = *(uint **)(DAT_00145590 + 0x14550c);
    uVar2 = *puVar3;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    iVar1 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar2);
    local_28 = *(float *)(iVar1 + 0xc) + *(float *)(in_r0 + 0x30) * DAT_00145588;
    local_24 = *(float *)(iVar1 + 0x1c) + *(float *)(in_r0 + 0x34) * DAT_00145588;
    local_20 = *(float *)(iVar1 + 0x2c) + *(float *)(in_r0 + 0x38) * DAT_00145588;
    LensFlare::update(*(int *)(in_r0 + 0x2c));
    iVar1 = AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar3,(Vector *)&local_28);
    if (iVar1 != 0) {
      LensFlare::render2D(*(LensFlare **)(in_r0 + 0x2c),extraout_s0,extraout_s1,extraout_s2,
                          (int)local_28);
    }
  }
  if (*piVar4 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001454e8: push {r4,r5,r6,r7,lr}
  001454ea: add r7,sp,#0xc
  001454ec: push.w r8
  001454f0: sub sp,#0x18
  001454f2: mov r4,r0
  001454f4: ldr r0,[0x0014558c]
  001454f6: add r0,pc
  001454f8: ldr.w r8,[r0,#0x0]
  001454fc: ldr.w r0,[r8,#0x0]
  00145500: str r0,[sp,#0x14]
  00145502: ldr r0,[r4,#0x2c]
  00145504: cbz r0,0x00145572
  00145506: ldr r0,[0x00145590]
  00145508: add r0,pc
  0014550a: ldr r6,[r0,#0x0]
  0014550c: ldr r5,[r6,#0x0]
  0014550e: mov r0,r5
  00145510: blx 0x000717f4
  00145514: mov r1,r0
  00145516: mov r0,r5
  00145518: blx 0x0006ff1c
  0014551c: vldr.32 s6,[pc,#0x68]
  00145520: movs r1,#0x0
  00145522: vldr.32 s8,[r4,#0x30]
  00145526: vldr.32 s10,[r4,#0x34]
  0014552a: vldr.32 s12,[r4,#0x38]
  0014552e: vldr.32 s0,[r0,#0xc]
  00145532: vldr.32 s2,[r0,#0x1c]
  00145536: vmla.f32 s0,s8,s6
  0014553a: vldr.32 s4,[r0,#0x2c]
  0014553e: vmla.f32 s2,s10,s6
  00145542: vmla.f32 s4,s12,s6
  00145546: vstr.32 s2,[sp,#0xc]
  0014554a: vstr.32 s0,[sp,#0x8]
  0014554e: vstr.32 s4,[sp,#0x10]
  00145552: ldr r0,[r4,#0x2c]
  00145554: blx 0x00076a2c
  00145558: add r1,sp,#0x8
  0014555a: ldr r0,[r6,#0x0]
  0014555c: mov r2,r1
  0014555e: blx 0x000729c4
  00145562: cbz r0,0x00145572
  00145564: add r3,sp,#0x8
  00145566: ldmia r3,{r1,r2,r3}
  00145568: ldr r0,[r4,#0x2c]
  0014556a: ldr r6,[r4,#0x3c]
  0014556c: str r6,[sp,#0x0]
  0014556e: blx 0x00076a38
  00145572: ldr r0,[sp,#0x14]
  00145574: ldr.w r1,[r8,#0x0]
  00145578: subs r0,r1,r0
  0014557a: ittt eq
  0014557c: add.eq sp,#0x18
  0014557e: pop.eq.w r8
  00145582: pop.eq {r4,r5,r6,r7,pc}
  00145584: blx 0x0006e824
```
