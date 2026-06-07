# PlayerStation::setPosition
elf 0x1226f8 body 84
Sig: undefined __thiscall setPosition(PlayerStation * this, Vector * param_1)

## decompile
```c

/* PlayerStation::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerStation::setPosition(PlayerStation *this,Vector *param_1)

{
  uint *puVar1;
  int *piVar2;
  uint uVar3;
  
  *(undefined4 *)(this + 0x58) = *(undefined4 *)param_1;
  *(undefined4 *)(this + 0x5c) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 0x60) = *(undefined4 *)(param_1 + 8);
  AEGeometry::setPosition(*(AEGeometry **)(this + 0x140),param_1);
  puVar1 = *(uint **)(this + 0x130);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      piVar2 = *(int **)(puVar1[1] + uVar3 * 4);
      (**(code **)(*piVar2 + 4))
                (piVar2,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),
                 *(undefined4 *)(param_1 + 8));
      puVar1 = *(uint **)(this + 0x130);
    }
  }
  return;
}

```

## target disasm
```
  001326f8: push {r4,r5,r6,r7,lr}
  001326fa: add r7,sp,#0xc
  001326fc: push.w r8
  00132700: mov r8,r0
  00132702: ldr r0,[r1,#0x0]
  00132704: str.w r0,[r8,#0x58]
  00132708: mov r4,r1
  0013270a: ldr r0,[r1,#0x4]
  0013270c: str.w r0,[r8,#0x5c]
  00132710: ldr.w r0,[r8,#0x140]
  00132714: ldr r1,[r1,#0x8]
  00132716: str.w r1,[r8,#0x60]
  0013271a: mov r1,r4
  0013271c: blx 0x00072148
  00132720: ldr.w r0,[r8,#0x130]
  00132724: cbz r0,0x00132746
  00132726: movs r6,#0x0
  00132728: b 0x00132740
  0013272a: ldr r0,[r0,#0x4]
  0013272c: ldr.w r0,[r0,r6,lsl #0x2]
  00132730: ldr r5,[r0,#0x0]
  00132732: ldm.w r4,{r1,r2,r3}
  00132736: ldr r5,[r5,#0x4]
  00132738: blx r5
  0013273a: ldr.w r0,[r8,#0x130]
  0013273e: adds r6,#0x1
  00132740: ldr r1,[r0,#0x0]
  00132742: cmp r6,r1
  00132744: bcc 0x0013272a
  00132746: pop.w r8
  0013274a: pop {r4,r5,r6,r7,pc}
```
