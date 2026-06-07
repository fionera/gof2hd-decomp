# BumpMapping::UpdateMeshData
elf 0x913f8 body 206
Sig: undefined __thiscall UpdateMeshData(BumpMapping * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpMapping::UpdateMeshData(BumpMapping *this,Mesh *param_1,Engine *param_2)

{
  Mesh MVar1;
  code *pcVar2;
  
  if (this[9] != (BumpMapping)0x0) {
    this[9] = (BumpMapping)0x0;
  }
  glUniformMatrix4fv(*(undefined4 *)(this + 0x34),1,0,param_2 + 0x104);
  glUniform3f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
              *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
  pcVar2 = *(code **)(DAT_000a14c8 + 0xa1436);
  (*pcVar2)(*(undefined4 *)(this + 0x20));
  (*pcVar2)(*(undefined4 *)(this + 0x24));
  (*pcVar2)(*(undefined4 *)(this + 0x28));
  (*pcVar2)(*(undefined4 *)(this + 0x2c));
  (*pcVar2)(*(undefined4 *)(this + 0x30));
  glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  MVar1 = *param_1;
  if (((byte)MVar1 & 2) != 0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x30),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    MVar1 = *param_1;
  }
  if ((int)((uint)(byte)MVar1 << 0x1d) < 0) {
    pcVar2 = *(code **)(DAT_000a14cc + 0xa148e);
    (*pcVar2)(*(undefined4 *)(this + 0x24),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
  }
  return;
}

```

## target disasm
```
  000a13f8: push {r4,r5,r6,r7,lr}
  000a13fa: add r7,sp,#0xc
  000a13fc: push.w r8
  000a1400: sub sp,#0x8
  000a1402: mov r5,r0
  000a1404: ldrb r0,[r0,#0x9]
  000a1406: mov r6,r2
  000a1408: mov r4,r1
  000a140a: cbz r0,0x000a1410
  000a140c: movs r0,#0x0
  000a140e: strb r0,[r5,#0x9]
  000a1410: ldr r0,[r5,#0x34]
  000a1412: add.w r3,r6,#0x104
  000a1416: movs r1,#0x1
  000a1418: movs r2,#0x0
  000a141a: mov.w r8,#0x0
  000a141e: blx 0x00070990
  000a1422: add.w r3,r6,#0x330
  000a1426: ldmia r3,{r1,r2,r3}
  000a1428: ldr r0,[r5,#0x38]
  000a142a: blx 0x000709b4
  000a142e: ldr r1,[0x000a14c8]
  000a1430: ldr r0,[r5,#0x20]
  000a1432: add r1,pc
  000a1434: ldr r6,[r1,#0x0]
  000a1436: blx r6
  000a1438: ldr r0,[r5,#0x24]
  000a143a: blx r6
  000a143c: ldr r0,[r5,#0x28]
  000a143e: blx r6
  000a1440: ldr r0,[r5,#0x2c]
  000a1442: blx r6
  000a1444: ldr r0,[r5,#0x30]
  000a1446: blx r6
  000a1448: ldr r1,[r4,#0x4]
  000a144a: movw r2,#0x1406
  000a144e: ldr r0,[r5,#0x20]
  000a1450: movs r3,#0x0
  000a1452: strd r8,r1,[sp,#0x0]
  000a1456: movs r1,#0x3
  000a1458: blx 0x000709c0
  000a145c: ldrb r0,[r4,#0x0]
  000a145e: tst r0,#0x2
  000a1462: beq 0x000a147a
  000a1464: ldr r1,[r4,#0x8]
  000a1466: movw r2,#0x1406
  000a146a: ldr r0,[r5,#0x30]
  000a146c: movs r3,#0x0
  000a146e: strd r8,r1,[sp,#0x0]
  000a1472: movs r1,#0x2
  000a1474: blx 0x000709c0
  000a1478: ldrb r0,[r4,#0x0]
  000a147a: lsls r0,r0,#0x1d
  000a147c: bpl 0x000a14be
  000a147e: ldr r0,[r5,#0x24]
  000a1480: mov.w r8,#0x0
  000a1484: ldr r1,[r4,#0x10]
  000a1486: movs r3,#0x0
  000a1488: ldr r2,[0x000a14cc]
  000a148a: add r2,pc
  000a148c: strd r8,r1,[sp,#0x0]
  000a1490: movs r1,#0x3
  000a1492: ldr r6,[r2,#0x0]
  000a1494: movw r2,#0x1406
  000a1498: blx r6
  000a149a: ldr r1,[r4,#0x14]
  000a149c: movw r2,#0x1406
  000a14a0: ldr r0,[r5,#0x28]
  000a14a2: movs r3,#0x0
  000a14a4: strd r8,r1,[sp,#0x0]
  000a14a8: movs r1,#0x3
  000a14aa: blx r6
  000a14ac: ldr r1,[r4,#0x18]
  000a14ae: movw r2,#0x1406
  000a14b2: ldr r0,[r5,#0x2c]
  000a14b4: movs r3,#0x0
  000a14b6: strd r8,r1,[sp,#0x0]
  000a14ba: movs r1,#0x3
  000a14bc: blx r6
  000a14be: add sp,#0x8
  000a14c0: pop.w r8
  000a14c4: pop {r4,r5,r6,r7,pc}
```
