# Gun::render
elf 0x15395c body 234
Sig: undefined __stdcall render(void)

## decompile
```c

/* Gun::render() */

void Gun::render(void)

{
  int in_r0;
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  float extraout_s1;
  float extraout_s2;
  Matrix local_a4 [60];
  undefined1 auStack_68 [64];
  int local_28;
  
  piVar1 = *(int **)(DAT_00163a48 + 0x16396e);
  local_28 = *piVar1;
  if (*(Sparks **)(in_r0 + 0xb8) != (Sparks *)0x0) {
    Sparks::render(*(Sparks **)(in_r0 + 0xb8));
  }
  if (*(int *)(in_r0 + 0x10c) != 0) {
    puVar6 = *(uint **)(DAT_00163a4c + 0x163998);
    for (uVar4 = 0; uVar4 < *(uint *)(in_r0 + 8); uVar4 = uVar4 + 1) {
      iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar6);
      if (*(char *)(iVar2 + 0xed) != '\0') {
        uVar5 = *puVar6;
        AbyssEngine::PaintCanvas::CameraGetCurrent();
        uVar3 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
        __aeabi_memcpy(auStack_68,uVar3,0x3c);
        AbyssEngine::PaintCanvas::TransformGetLocal(*puVar6);
        AbyssEngine::AEMath::MatrixGetPosition(local_a4);
        AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0xd8),(Vector *)local_a4);
        AbyssEngine::AEMath::MatrixSetTranslation
                  (local_a4,*(float *)(in_r0 + 0xe0),extraout_s1,extraout_s2);
        AbyssEngine::PaintCanvas::TransformSetLocal
                  (*puVar6,*(Matrix **)(*(int *)(in_r0 + 0x10c) + uVar4 * 4));
        AbyssEngine::PaintCanvas::DrawTransform
                  (*puVar6,*(Matrix **)(*(int *)(in_r0 + 0x10c) + uVar4 * 4));
      }
    }
  }
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016395c: push {r4,r5,r6,r7,lr}
  0016395e: add r7,sp,#0xc
  00163960: push {r8,r9,r10,r11}
  00163964: sub sp,#0x8c
  00163966: mov r4,r0
  00163968: ldr r0,[0x00163a48]
  0016396a: add r0,pc
  0016396c: ldr r0,[r0,#0x0]
  0016396e: str r0,[sp,#0x8]
  00163970: ldr r0,[r0,#0x0]
  00163972: str r0,[sp,#0x88]
  00163974: ldr.w r0,[r4,#0xb8]
  00163978: cbz r0,0x0016397e
  0016397a: blx 0x000773ec
  0016397e: ldr.w r0,[r4,#0x10c]
  00163982: cmp r0,#0x0
  00163984: beq 0x00163a30
  00163986: ldr r0,[0x00163a4c]
  00163988: add.w r8,r4,#0xd8
  0016398c: add.w r9,sp,#0x48
  00163990: add.w r11,sp,#0xc
  00163994: add r0,pc
  00163996: movs r5,#0x0
  00163998: ldr.w r10,[r0,#0x0]
  0016399c: b 0x00163a2a
  0016399e: ldr.w r0,[r4,#0x10c]
  001639a2: ldr.w r1,[r0,r5,lsl #0x2]
  001639a6: ldr.w r0,[r10,#0x0]
  001639aa: blx 0x00072088
  001639ae: ldrb.w r0,[r0,#0xed]
  001639b2: cbz r0,0x00163a28
  001639b4: ldr.w r6,[r10,#0x0]
  001639b8: mov r0,r6
  001639ba: blx 0x000717f4
  001639be: mov r1,r0
  001639c0: mov r0,r6
  001639c2: blx 0x0006ff1c
  001639c6: mov r1,r0
  001639c8: mov r0,r9
  001639ca: movs r2,#0x3c
  001639cc: blx 0x0006f1e4
  001639d0: ldr.w r1,[r4,#0x10c]
  001639d4: ldr.w r0,[r10,#0x0]
  001639d8: ldr.w r1,[r1,r5,lsl #0x2]
  001639dc: blx 0x000720c4
  001639e0: mov r1,r0
  001639e2: mov r0,r11
  001639e4: blx 0x0006f16c
  001639e8: mov r0,r8
  001639ea: mov r1,r11
  001639ec: blx 0x0006eb3c
  001639f0: ldrd r2,r3,[r4,#0xd8]
  001639f4: mov r0,r11
  001639f6: vldr.32 s0,[r4,#0xe0]
  001639fa: mov r1,r9
  001639fc: vstr.32 s0,[sp]
  00163a00: blx 0x0006f820
  00163a04: ldr.w r1,[r4,#0x10c]
  00163a08: mov r2,r9
  00163a0a: ldr.w r0,[r10,#0x0]
  00163a0e: ldr.w r1,[r1,r5,lsl #0x2]
  00163a12: blx 0x000721c0
  00163a16: ldr.w r1,[r4,#0x10c]
  00163a1a: movs r2,#0x0
  00163a1c: ldr.w r0,[r10,#0x0]
  00163a20: ldr.w r1,[r1,r5,lsl #0x2]
  00163a24: blx 0x0007306c
  00163a28: adds r5,#0x1
  00163a2a: ldr r0,[r4,#0x8]
  00163a2c: cmp r5,r0
  00163a2e: bcc 0x0016399e
  00163a30: ldr r0,[sp,#0x88]
  00163a32: ldr r1,[sp,#0x8]
  00163a34: ldr r1,[r1,#0x0]
  00163a36: subs r0,r1,r0
  00163a38: ittt eq
  00163a3a: add.eq sp,#0x8c
  00163a3c: pop.eq.w {r8,r9,r10,r11}
  00163a40: pop.eq {r4,r5,r6,r7,pc}
  00163a42: blx 0x0006e824
```
