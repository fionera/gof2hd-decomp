# ParticleSystemSprite::render
elf 0x18347c body 250
Sig: undefined __stdcall render(PaintCanvas * param_1, uint param_2)

## decompile
```c

/* ParticleSystemSprite::render(AbyssEngine::PaintCanvas*, unsigned int) */

void ParticleSystemSprite::render(PaintCanvas *param_1,uint param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  
  if (param_2 != 0xffffffff) {
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    puVar1 = (undefined4 *)AbyssEngine::PaintCanvas::CameraGetLocal((uint)param_1);
    uVar3 = *puVar1;
    uVar4 = puVar1[1];
    uVar5 = puVar1[2];
    uVar6 = puVar1[3];
    uVar7 = puVar1[4];
    uVar8 = puVar1[5];
    uVar9 = puVar1[6];
    uVar10 = puVar1[7];
    uVar11 = puVar1[8];
    uVar12 = puVar1[9];
    uVar13 = puVar1[10];
    uVar14 = puVar1[0xb];
    uVar15 = puVar1[0xc];
    uVar16 = puVar1[0xd];
    uVar2 = puVar1[0xe];
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    puVar1 = (undefined4 *)AbyssEngine::PaintCanvas::CameraGetLocal((uint)param_1);
    AbyssEngine::PaintCanvas::DrawSpriteSystem
              (param_1,param_2,uVar3,uVar4,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12,uVar13
               ,uVar14,uVar15,uVar16,uVar2,*puVar1,puVar1[1],puVar1[2],puVar1[3],puVar1[4],puVar1[5]
               ,puVar1[6],puVar1[7],puVar1[8],puVar1[9],puVar1[10],puVar1[0xb],puVar1[0xc],
               puVar1[0xd],puVar1[0xe]);
  }
  return;
}

```

## target disasm
```
  0019347c: push {r4,r5,r6,r7,lr}
  0019347e: add r7,sp,#0xc
  00193480: push {r8,r9,r10,r11}
  00193484: sub sp,#0xbc
  00193486: mov r5,r0
  00193488: adds r0,r1,#0x1
  0019348a: beq 0x0019356e
  0019348c: mov r0,r5
  0019348e: str r1,[sp,#0xb8]
  00193490: blx 0x000717f4
  00193494: mov r1,r0
  00193496: mov r0,r5
  00193498: blx 0x0006ff1c
  0019349c: ldr r1,[r0,#0x0]
  0019349e: str r1,[sp,#0xb4]
  001934a0: ldr r1,[r0,#0x4]
  001934a2: str r1,[sp,#0xb0]
  001934a4: ldr r1,[r0,#0x8]
  001934a6: str r1,[sp,#0xac]
  001934a8: ldr r1,[r0,#0xc]
  001934aa: str r1,[sp,#0xa8]
  001934ac: ldr r1,[r0,#0x10]
  001934ae: str r1,[sp,#0xa4]
  001934b0: ldr r1,[r0,#0x14]
  001934b2: str r1,[sp,#0xa0]
  001934b4: ldr r1,[r0,#0x18]
  001934b6: str r1,[sp,#0x9c]
  001934b8: ldr r1,[r0,#0x1c]
  001934ba: str r1,[sp,#0x98]
  001934bc: ldr r1,[r0,#0x20]
  001934be: str r1,[sp,#0x94]
  001934c0: ldr r1,[r0,#0x24]
  001934c2: str r1,[sp,#0x90]
  001934c4: ldr r1,[r0,#0x28]
  001934c6: str r1,[sp,#0x8c]
  001934c8: ldr r1,[r0,#0x2c]
  001934ca: str r1,[sp,#0x88]
  001934cc: ldr r1,[r0,#0x30]
  001934ce: str r1,[sp,#0x84]
  001934d0: ldr r1,[r0,#0x34]
  001934d2: ldr r0,[r0,#0x38]
  001934d4: str r0,[sp,#0x7c]
  001934d6: mov r0,r5
  001934d8: str r1,[sp,#0x80]
  001934da: blx 0x000717f4
  001934de: mov r1,r0
  001934e0: mov r0,r5
  001934e2: blx 0x0006ff1c
  001934e6: ldr r1,[r0,#0x0]
  001934e8: add.w lr,r0,#0x14
  001934ec: str r1,[sp,#0x78]
  001934ee: add.w r11,r0,#0x24
  001934f2: ldr r1,[r0,#0x4]
  001934f4: str r1,[sp,#0x74]
  001934f6: ldr r1,[r0,#0x8]
  001934f8: str r1,[sp,#0x70]
  001934fa: ldrd r6,r1,[r0,#0xc]
  001934fe: ldm.w lr,{r2,r3,r12,lr}
  00193502: ldm.w r11,{r4,r8,r11}
  00193506: ldrd r9,r10,[r0,#0x30]
  0019350a: ldr r0,[r0,#0x38]
  0019350c: strd r9,r10,[sp,#0x64]
  00193510: str r0,[sp,#0x6c]
  00193512: add r0,sp,#0x48
  00193514: strd r6,r1,[sp,#0x40]
  00193518: stm r0,{r2,r3,r12,lr}
  0019351c: add r0,sp,#0x58
  0019351e: stm r0,{r4,r8,r11}
  00193522: ldr r0,[sp,#0x8c]
  00193524: str r0,[sp,#0x20]
  00193526: ldr r0,[sp,#0x88]
  00193528: str r0,[sp,#0x24]
  0019352a: ldr r0,[sp,#0x84]
  0019352c: str r0,[sp,#0x28]
  0019352e: ldr r0,[sp,#0x80]
  00193530: str r0,[sp,#0x2c]
  00193532: ldr r0,[sp,#0x7c]
  00193534: str r0,[sp,#0x30]
  00193536: ldr r0,[sp,#0x78]
  00193538: str r0,[sp,#0x34]
  0019353a: ldr r0,[sp,#0x74]
  0019353c: str r0,[sp,#0x38]
  0019353e: ldr r0,[sp,#0x70]
  00193540: str r0,[sp,#0x3c]
  00193542: ldr r0,[sp,#0xac]
  00193544: str r0,[sp,#0x0]
  00193546: ldr r0,[sp,#0xa8]
  00193548: str r0,[sp,#0x4]
  0019354a: ldr r0,[sp,#0xa4]
  0019354c: str r0,[sp,#0x8]
  0019354e: ldr r0,[sp,#0xa0]
  00193550: str r0,[sp,#0xc]
  00193552: ldr r0,[sp,#0x9c]
  00193554: str r0,[sp,#0x10]
  00193556: ldr r0,[sp,#0x98]
  00193558: str r0,[sp,#0x14]
  0019355a: ldr r0,[sp,#0x94]
  0019355c: str r0,[sp,#0x18]
  0019355e: ldr r0,[sp,#0x90]
  00193560: str r0,[sp,#0x1c]
  00193562: mov r0,r5
  00193564: ldr r1,[sp,#0xb8]
  00193566: ldrd r3,r2,[sp,#0xb0]
  0019356a: blx 0x0007891c
  0019356e: add sp,#0xbc
  00193570: pop.w {r8,r9,r10,r11}
  00193574: pop {r4,r5,r6,r7,pc}
```
