# StarMap::render
elf 0xc8058 body 134
Sig: undefined __stdcall render(void)

## decompile
```c

/* StarMap::render() */

void StarMap::render(void)

{
  int in_r0;
  uint *puVar1;
  code *pcVar2;
  
  puVar1 = *(uint **)(DAT_000d80e0 + 0xd806c);
  AbyssEngine::PaintCanvas::SetColor(*puVar1);
  pcVar2 = *(code **)(DAT_000d80e4 + 0xd807c);
  (*pcVar2)(*(undefined4 *)(in_r0 + 0x1b0));
  (*pcVar2)(*(undefined4 *)(in_r0 + 0x1b4));
  (*pcVar2)(*(undefined4 *)(in_r0 + 0x1b8));
  (*pcVar2)(*(undefined4 *)(in_r0 + 0x6c));
  AbyssEngine::PaintCanvas::End3d();
  AbyssEngine::PaintCanvas::Begin3d();
  if (*(int *)(in_r0 + 0xa4) != 0) {
    AbyssEngine::PaintCanvas::SetTexture(*puVar1,*(uint *)(in_r0 + 0x178));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar1,2);
    AEGeometry::render();
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xf8) == 0) {
    return;
  }
  (*(code *)(DAT_001abdd4 + 0x1abdd8))();
  return;
}

```

## target disasm
```
  000d8058: push {r4,r5,r6,r7,lr}
  000d805a: add r7,sp,#0xc
  000d805c: push.w r11
  000d8060: mov r4,r0
  000d8062: ldr r0,[0x000d80e0]
  000d8064: mov.w r1,#0xffffffff
  000d8068: add r0,pc
  000d806a: ldr r5,[r0,#0x0]
  000d806c: ldr r0,[r5,#0x0]
  000d806e: blx 0x0006fac0
  000d8072: ldr r1,[0x000d80e4]
  000d8074: ldr.w r0,[r4,#0x1b0]
  000d8078: add r1,pc
  000d807a: ldr r6,[r1,#0x0]
  000d807c: blx r6
  000d807e: ldr.w r0,[r4,#0x1b4]
  000d8082: blx r6
  000d8084: ldr.w r0,[r4,#0x1b8]
  000d8088: blx r6
  000d808a: ldr r0,[r4,#0x6c]
  000d808c: blx r6
  000d808e: ldr r0,[r5,#0x0]
  000d8090: blx 0x00071dac
  000d8094: ldr r0,[r5,#0x0]
  000d8096: blx 0x00071da0
  000d809a: ldr.w r0,[r4,#0xa4]
  000d809e: cbz r0,0x000d80c6
  000d80a0: ldr.w r1,[r4,#0x178]
  000d80a4: mov.w r2,#0xffffffff
  000d80a8: ldr r0,[r5,#0x0]
  000d80aa: blx 0x0006fa6c
  000d80ae: ldr r0,[r5,#0x0]
  000d80b0: movs r1,#0x2
  000d80b2: blx 0x0006febc
  000d80b6: ldr.w r0,[r4,#0x1bc]
  000d80ba: blx 0x00072238
  000d80be: ldr.w r0,[r4,#0xa4]
  000d80c2: blx 0x00072238
  000d80c6: ldr.w r0,[r4,#0xf8]
  000d80ca: cbz r0,0x000d80d8
  000d80cc: pop.w r11
  000d80d0: pop.w {r4,r5,r6,r7,lr}
  000d80d4: b.w 0x001abdc8
  000d80d8: pop.w r11
  000d80dc: pop {r4,r5,r6,r7,pc}
```
