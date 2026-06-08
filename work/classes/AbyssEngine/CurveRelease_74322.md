# AbyssEngine::CurveRelease
elf 0x74322 body 86
Sig: undefined __stdcall CurveRelease(Curve * * param_1)

## decompile
```c

/* AbyssEngine::CurveRelease(AbyssEngine::Curve**) */

void AbyssEngine::CurveRelease(Curve **param_1)

{
  char cVar1;
  char *pcVar2;
  void *pvVar3;
  Curve *pCVar4;
  uint uVar5;
  
  pCVar4 = *param_1;
  if (pCVar4 != (Curve *)0x0) {
    for (uVar5 = 0; pvVar3 = *(void **)(pCVar4 + 4), uVar5 < *(ushort *)pCVar4; uVar5 = uVar5 + 1) {
      pcVar2 = *(char **)((int)pvVar3 + uVar5 * 4);
      cVar1 = *pcVar2;
      if (cVar1 == '\x03' || cVar1 == '\x02') {
        operator_delete(pcVar2);
      }
      else if (cVar1 == '\x01') {
        operator_delete(pcVar2);
      }
      pCVar4 = *param_1;
    }
    if (pvVar3 != (void *)0x0) {
      operator_delete__(pvVar3);
      pCVar4 = *param_1;
    }
    *(undefined4 *)(pCVar4 + 4) = 0;
    if (*param_1 != (Curve *)0x0) {
      operator_delete(*param_1);
    }
    *param_1 = (Curve *)0x0;
  }
  return;
}

```

## target disasm
```
  00084322: push {r4,r5,r7,lr}
  00084324: add r7,sp,#0x8
  00084326: ldr r1,[r0,#0x0]
  00084328: mov r4,r0
  0008432a: cbz r1,0x00084376
  0008432c: movs r5,#0x0
  0008432e: b 0x00084352
  00084330: ldr.w r0,[r0,r5,lsl #0x2]
  00084334: ldrsb.w r1,[r0,#0x0]
  00084338: cmp r1,#0x3
  0008433a: it ne
  0008433c: cmp.ne r1,#0x2
  0008433e: bne 0x00084346
  00084340: blx 0x0006eb48
  00084344: b 0x0008434e
  00084346: cmp r1,#0x1
  00084348: it eq
  0008434a: blx.eq 0x0006eb48
  0008434e: ldr r1,[r4,#0x0]
  00084350: adds r5,#0x1
  00084352: ldrh r2,[r1,#0x0]
  00084354: ldr r0,[r1,#0x4]
  00084356: cmp r5,r2
  00084358: bcc 0x00084330
  0008435a: cbz r0,0x00084366
  0008435c: blx 0x0006ebfc
  00084360: ldr r0,[r4,#0x0]
  00084362: adds r0,#0x4
  00084364: b 0x00084368
  00084366: adds r0,r1,#0x4
  00084368: movs r5,#0x0
  0008436a: str r5,[r0,#0x0]
  0008436c: ldr r0,[r4,#0x0]
  0008436e: cbz r0,0x00084374
  00084370: blx 0x0006eb48
  00084374: str r5,[r4,#0x0]
  00084376: pop {r4,r5,r7,pc}
```
