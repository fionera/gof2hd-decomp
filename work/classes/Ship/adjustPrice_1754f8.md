# Ship::adjustPrice
elf 0x1754f8 body 162
Sig: undefined __thiscall adjustPrice(Ship * this)

## decompile
```c

/* Ship::adjustPrice() */

void __thiscall Ship::adjustPrice(Ship *this)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  float fVar6;
  
  iVar1 = Status::getStation();
  if ((iVar1 != 0) && (0 < *(int *)(this + 0x14))) {
    piVar3 = *(int **)(DAT_001855ac + 0x185520);
    iVar2 = **(int **)(*(int *)(*piVar3 + 4) + *(int *)this * 4);
    Status::getSystem();
    iVar1 = SolarSystem::getRace();
    fVar4 = (float)VectorSignedToFloat(*(undefined4 *)
                                        (*(int *)(*(int *)(*piVar3 + 4) + *(int *)this * 4) + 0x14),
                                       (byte)(in_fpscr >> 0x16) & 3);
    fVar6 = DAT_001855a0;
    if (*(int *)(DAT_001855b0 + 0x185558 + iVar2 * 4) == iVar1) {
      fVar6 = fVar4 * DAT_0018559c;
    }
    fVar5 = DAT_001855a0;
    if (**(int **)(DAT_001855b4 + 0x185570) != 0) {
      fVar5 = (float)VectorSignedToFloat(**(int **)(DAT_001855b4 + 0x185570),
                                         (byte)(in_fpscr >> 0x16) & 3);
      fVar5 = fVar4 * fVar5 * DAT_001855a4;
    }
    *(int *)(this + 0x14) = (int)(fVar6 + fVar4 + fVar5);
  }
  return;
}

```
## target disasm
```
  001854f8: push {r4,r5,r6,r7,lr}
  001854fa: add r7,sp,#0xc
  001854fc: push.w r11
  00185500: mov r4,r0
  00185502: ldr r0,[0x001855a8]
  00185504: add r0,pc
  00185506: ldr r5,[r0,#0x0]
  00185508: ldr r0,[r5,#0x0]
  0018550a: blx 0x00071c14
  0018550e: cmp r0,#0x0
  00185510: beq 0x00185594
  00185512: ldr r0,[r4,#0x14]
  00185514: cmp r0,#0x1
  00185516: blt 0x00185594
  00185518: ldr r0,[0x001855ac]
  0018551a: ldr r1,[r4,#0x0]
  0018551c: add r0,pc
  0018551e: ldr r6,[r0,#0x0]
  00185520: ldr r0,[r6,#0x0]
  00185522: ldr r0,[r0,#0x4]
  00185524: ldr.w r1,[r0,r1,lsl #0x2]
  00185528: ldr r0,[r5,#0x0]
  0018552a: ldr r5,[r1,#0x0]
  0018552c: blx 0x00071a10
  00185530: blx 0x00071950
  00185534: ldr r1,[r6,#0x0]
  00185536: ldr r2,[r4,#0x0]
  00185538: vldr.32 s4,[pc,#0x60]
  0018553c: ldr r1,[r1,#0x4]
  0018553e: vldr.32 s2,[pc,#0x60]
  00185542: ldr.w r1,[r1,r2,lsl #0x2]
  00185546: vmov.f32 s6,s2
  0018554a: vldr.32 s0,[r1,#0x14]
  0018554e: vcvt.f32.s32 s0,s0
  00185552: ldr r1,[0x001855b0]
  00185554: add r1,pc
  00185556: ldr.w r1,[r1,r5,lsl #0x2]
  0018555a: vmul.f32 s4,s0,s4
  0018555e: cmp r1,r0
  00185560: it eq
  00185562: vmov.eq.f32 s6,s4
  00185566: ldr r0,[0x001855b4]
  00185568: vadd.f32 s4,s6,s0
  0018556c: add r0,pc
  0018556e: ldr r0,[r0,#0x0]
  00185570: ldr r0,[r0,#0x0]
  00185572: cbz r0,0x00185588
  00185574: vmov s2,r0
  00185578: vcvt.f32.s32 s2,s2
  0018557c: vmul.f32 s0,s0,s2
  00185580: vldr.32 s2,[pc,#0x20]
  00185584: vmul.f32 s2,s0,s2
  00185588: vadd.f32 s0,s4,s2
  0018558c: vcvt.s32.f32 s0,s0
  00185590: vstr.32 s0,[r4,#0x14]
  00185594: pop.w r11
  00185598: pop {r4,r5,r6,r7,pc}
```
