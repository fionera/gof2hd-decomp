# VertexColorShader::UpdateMeshData
elf 0x90374 body 570
Sig: undefined __thiscall UpdateMeshData(VertexColorShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::VertexColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::VertexColorShader::UpdateMeshData
          (VertexColorShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix4fv(*(int *)(this + 0x38),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix3fv(*(int *)(this + 0x3c),1,0,param_2 + 0x204);
  }
  if (this[9] != (VertexColorShader)0x0) {
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform4fv(*(int *)(this + 0x58),1,param_2 + 0x2b8);
    }
    this[9] = (VertexColorShader)0x0;
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x30)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x34)) {
    glEnableVertexAttribArray();
  }
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < *(int *)(this + 0x20)) {
      glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    if (-1 < *(int *)(this + 0x24)) {
      glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    }
    if (-1 < *(int *)(this + 0x28)) {
      glVertexAttribPointer(*(int *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
    }
    if (-1 < *(int *)(this + 0x2c)) {
      glVertexAttribPointer(*(int *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
    }
    if (-1 < *(int *)(this + 0x30)) {
      glVertexAttribPointer(*(int *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
    }
    if (-1 < *(int *)(this + 0x34)) {
      glVertexAttribPointer(*(int *)(this + 0x34),4,0x1406,0,0,*(undefined4 *)(param_1 + 0xc));
    }
  }
  else {
    pcVar1 = *(code **)(DAT_000a05b0 + 0xa045c);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_000a05b4 + 0xa0472);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x78));
    (*pcVar2)(*(undefined4 *)(this + 0x34),4,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  000a0374: push {r4,r5,r6,r7,lr}
  000a0376: add r7,sp,#0xc
  000a0378: push {r6,r7,r8,r9,r11}
  000a037c: mov r4,r0
  000a037e: ldr r0,[r0,#0x38]
  000a0380: mov r6,r2
  000a0382: mov r8,r1
  000a0384: cmp r0,#0x0
  000a0386: blt 0x000a0394
  000a0388: add.w r3,r6,#0x104
  000a038c: movs r1,#0x1
  000a038e: movs r2,#0x0
  000a0390: blx 0x00070990
  000a0394: ldr r0,[r4,#0x3c]
  000a0396: cmp r0,#0x0
  000a0398: blt 0x000a03a6
  000a039a: add.w r3,r6,#0x204
  000a039e: movs r1,#0x1
  000a03a0: movs r2,#0x0
  000a03a2: blx 0x0007099c
  000a03a6: ldrb r0,[r4,#0x9]
  000a03a8: cbz r0,0x000a040e
  000a03aa: ldr r0,[r4,#0x40]
  000a03ac: cmp r0,#0x0
  000a03ae: blt 0x000a03ba
  000a03b0: add.w r3,r6,#0x330
  000a03b4: ldmia r3,{r1,r2,r3}
  000a03b6: blx 0x000709b4
  000a03ba: ldr r0,[r4,#0x44]
  000a03bc: cmp r0,#0x0
  000a03be: blt 0x000a03ca
  000a03c0: add.w r3,r6,#0x34c
  000a03c4: ldmia r3,{r1,r2,r3}
  000a03c6: blx 0x000709b4
  000a03ca: ldr r0,[r4,#0x4c]
  000a03cc: cmp r0,#0x0
  000a03ce: blt 0x000a03da
  000a03d0: add.w r2,r6,#0xd0
  000a03d4: movs r1,#0x1
  000a03d6: blx 0x00070984
  000a03da: ldr r0,[r4,#0x50]
  000a03dc: cmp r0,#0x0
  000a03de: blt 0x000a03ea
  000a03e0: add.w r2,r6,#0x2a8
  000a03e4: movs r1,#0x1
  000a03e6: blx 0x00070984
  000a03ea: ldr r0,[r4,#0x54]
  000a03ec: cmp r0,#0x0
  000a03ee: blt 0x000a03fa
  000a03f0: add.w r2,r6,#0x298
  000a03f4: movs r1,#0x1
  000a03f6: blx 0x00070984
  000a03fa: ldr r0,[r4,#0x58]
  000a03fc: cmp r0,#0x0
  000a03fe: blt 0x000a040a
  000a0400: add.w r2,r6,#0x2b8
  000a0404: movs r1,#0x1
  000a0406: blx 0x00070984
  000a040a: movs r0,#0x0
  000a040c: strb r0,[r4,#0x9]
  000a040e: ldr r0,[r4,#0x20]
  000a0410: cmp r0,#0x0
  000a0412: blt 0x000a0418
  000a0414: blx 0x00070a5c
  000a0418: ldr r0,[r4,#0x24]
  000a041a: cmp r0,#0x0
  000a041c: blt 0x000a0422
  000a041e: blx 0x00070a5c
  000a0422: ldr r0,[r4,#0x28]
  000a0424: cmp r0,#0x0
  000a0426: blt 0x000a042c
  000a0428: blx 0x00070a5c
  000a042c: ldr r0,[r4,#0x2c]
  000a042e: cmp r0,#0x0
  000a0430: blt 0x000a0436
  000a0432: blx 0x00070a5c
  000a0436: ldr r0,[r4,#0x30]
  000a0438: cmp r0,#0x0
  000a043a: blt 0x000a0440
  000a043c: blx 0x00070a5c
  000a0440: ldr r0,[r4,#0x34]
  000a0442: cmp r0,#0x0
  000a0444: blt 0x000a044a
  000a0446: blx 0x00070a5c
  000a044a: ldrb.w r0,[r8,#0x5c]
  000a044e: cmp r0,#0x0
  000a0450: beq 0x000a0500
  000a0452: ldr r0,[0x000a05b0]
  000a0454: ldr.w r1,[r8,#0x60]
  000a0458: add r0,pc
  000a045a: ldr r5,[r0,#0x0]
  000a045c: movw r0,#0x8892
  000a0460: blx r5
  000a0462: ldr r1,[0x000a05b4]
  000a0464: mov.w r9,#0x0
  000a0468: ldr r0,[r4,#0x20]
  000a046a: movw r2,#0x1406
  000a046e: add r1,pc
  000a0470: movs r3,#0x0
  000a0472: strd r9,r9,[sp,#0x0]
  000a0476: ldr r6,[r1,#0x0]
  000a0478: movs r1,#0x3
  000a047a: blx r6
  000a047c: ldr.w r1,[r8,#0x68]
  000a0480: movw r0,#0x8892
  000a0484: blx r5
  000a0486: ldr r0,[r4,#0x24]
  000a0488: movs r1,#0x2
  000a048a: movw r2,#0x1406
  000a048e: movs r3,#0x0
  000a0490: strd r9,r9,[sp,#0x0]
  000a0494: blx r6
  000a0496: ldr.w r1,[r8,#0x6c]
  000a049a: movw r0,#0x8892
  000a049e: blx r5
  000a04a0: ldr r0,[r4,#0x28]
  000a04a2: movs r1,#0x3
  000a04a4: movw r2,#0x1406
  000a04a8: movs r3,#0x0
  000a04aa: strd r9,r9,[sp,#0x0]
  000a04ae: blx r6
  000a04b0: ldr.w r1,[r8,#0x70]
  000a04b4: movw r0,#0x8892
  000a04b8: blx r5
  000a04ba: ldr r0,[r4,#0x2c]
  000a04bc: movs r1,#0x3
  000a04be: movw r2,#0x1406
  000a04c2: movs r3,#0x0
  000a04c4: strd r9,r9,[sp,#0x0]
  000a04c8: blx r6
  000a04ca: ldr.w r1,[r8,#0x74]
  000a04ce: movw r0,#0x8892
  000a04d2: blx r5
  000a04d4: ldr r0,[r4,#0x30]
  000a04d6: movs r1,#0x3
  000a04d8: movw r2,#0x1406
  000a04dc: movs r3,#0x0
  000a04de: strd r9,r9,[sp,#0x0]
  000a04e2: blx r6
  000a04e4: ldr.w r1,[r8,#0x78]
  000a04e8: movw r0,#0x8892
  000a04ec: blx r5
  000a04ee: ldr r0,[r4,#0x34]
  000a04f0: movs r1,#0x4
  000a04f2: movw r2,#0x1406
  000a04f6: movs r3,#0x0
  000a04f8: strd r9,r9,[sp,#0x0]
  000a04fc: blx r6
  000a04fe: b 0x000a05a8
  000a0500: ldr r0,[r4,#0x20]
  000a0502: cmp r0,#0x0
  000a0504: blt 0x000a051c
  000a0506: ldr.w r1,[r8,#0x4]
  000a050a: movs r2,#0x0
  000a050c: movs r3,#0x0
  000a050e: strd r2,r1,[sp,#0x0]
  000a0512: movs r1,#0x3
  000a0514: movw r2,#0x1406
  000a0518: blx 0x000709c0
  000a051c: ldr r0,[r4,#0x24]
  000a051e: cmp r0,#0x0
  000a0520: blt 0x000a0538
  000a0522: ldr.w r1,[r8,#0x8]
  000a0526: movs r2,#0x0
  000a0528: movs r3,#0x0
  000a052a: strd r2,r1,[sp,#0x0]
  000a052e: movs r1,#0x2
  000a0530: movw r2,#0x1406
  000a0534: blx 0x000709c0
  000a0538: ldr r0,[r4,#0x28]
  000a053a: cmp r0,#0x0
  000a053c: blt 0x000a0554
  000a053e: ldr.w r1,[r8,#0x10]
  000a0542: movs r2,#0x0
  000a0544: movs r3,#0x0
  000a0546: strd r2,r1,[sp,#0x0]
  000a054a: movs r1,#0x3
  000a054c: movw r2,#0x1406
  000a0550: blx 0x000709c0
  000a0554: ldr r0,[r4,#0x2c]
  000a0556: cmp r0,#0x0
  000a0558: blt 0x000a0570
  000a055a: ldr.w r1,[r8,#0x14]
  000a055e: movs r2,#0x0
  000a0560: movs r3,#0x0
  000a0562: strd r2,r1,[sp,#0x0]
  000a0566: movs r1,#0x3
  000a0568: movw r2,#0x1406
  000a056c: blx 0x000709c0
  000a0570: ldr r0,[r4,#0x30]
  000a0572: cmp r0,#0x0
  000a0574: blt 0x000a058c
  000a0576: ldr.w r1,[r8,#0x18]
  000a057a: movs r2,#0x0
  000a057c: movs r3,#0x0
  000a057e: strd r2,r1,[sp,#0x0]
  000a0582: movs r1,#0x3
  000a0584: movw r2,#0x1406
  000a0588: blx 0x000709c0
  000a058c: ldr r0,[r4,#0x34]
  000a058e: cmp r0,#0x0
  000a0590: blt 0x000a05a8
  000a0592: ldr.w r1,[r8,#0xc]
  000a0596: movs r2,#0x0
  000a0598: movs r3,#0x0
  000a059a: strd r2,r1,[sp,#0x0]
  000a059e: movs r1,#0x4
  000a05a0: movw r2,#0x1406
  000a05a4: blx 0x000709c0
  000a05a8: pop.w {r2,r3,r8,r9,r11}
  000a05ac: pop {r4,r5,r6,r7,pc}
```
