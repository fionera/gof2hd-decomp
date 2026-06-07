# PulseShader::UpdateMeshData
elf 0x92594 body 560
Sig: undefined __thiscall UpdateMeshData(PulseShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::PulseShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::PulseShader::UpdateMeshData(PulseShader *this,Mesh *param_1,Engine *param_2)

{
  float fVar1;
  code *pcVar2;
  code *pcVar3;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (PulseShader)0x0) {
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform4fv(*(int *)(this + 0x48),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x2b8);
    }
    ApplicationManager::GetCurrentTimeMillis();
    fVar1 = (float)__aeabi_l2f();
    fVar1 = (float)sinf(fVar1 / DAT_000a27c4);
    glUniform1f(*(undefined4 *)(this + 0x58),fVar1 + 2.0);
    this[9] = (PulseShader)0x0;
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
  }
  else {
    pcVar2 = *(code **)(DAT_000a27c8 + 0xa26a8);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_000a27cc + 0xa26be);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar3)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar3)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  000a2594: push {r4,r5,r6,r7,lr}
  000a2596: add r7,sp,#0xc
  000a2598: push {r6,r7,r8,r9,r11}
  000a259c: mov r4,r0
  000a259e: ldr r0,[r0,#0x34]
  000a25a0: mov r6,r2
  000a25a2: mov r8,r1
  000a25a4: cmp r0,#0x0
  000a25a6: blt 0x000a25b4
  000a25a8: add.w r3,r6,#0x104
  000a25ac: movs r1,#0x1
  000a25ae: movs r2,#0x0
  000a25b0: blx 0x00070990
  000a25b4: ldr r0,[r4,#0x38]
  000a25b6: cmp r0,#0x0
  000a25b8: blt 0x000a25c6
  000a25ba: add.w r3,r6,#0x204
  000a25be: movs r1,#0x1
  000a25c0: movs r2,#0x0
  000a25c2: blx 0x0007099c
  000a25c6: ldrb r0,[r4,#0x9]
  000a25c8: cmp r0,#0x0
  000a25ca: beq 0x000a2664
  000a25cc: ldr r0,[r4,#0x3c]
  000a25ce: cmp r0,#0x0
  000a25d0: blt 0x000a25dc
  000a25d2: add.w r3,r6,#0x330
  000a25d6: ldmia r3,{r1,r2,r3}
  000a25d8: blx 0x000709b4
  000a25dc: ldr r0,[r4,#0x40]
  000a25de: cmp r0,#0x0
  000a25e0: blt 0x000a25ec
  000a25e2: add.w r3,r6,#0x34c
  000a25e6: ldmia r3,{r1,r2,r3}
  000a25e8: blx 0x000709b4
  000a25ec: ldr r0,[r4,#0x48]
  000a25ee: cmp r0,#0x0
  000a25f0: blt 0x000a25fc
  000a25f2: add.w r2,r6,#0xd0
  000a25f6: movs r1,#0x1
  000a25f8: blx 0x00070984
  000a25fc: ldr r0,[r4,#0x4c]
  000a25fe: cmp r0,#0x0
  000a2600: blt 0x000a260c
  000a2602: add.w r2,r6,#0x2a8
  000a2606: movs r1,#0x1
  000a2608: blx 0x00070984
  000a260c: ldr r0,[r4,#0x50]
  000a260e: cmp r0,#0x0
  000a2610: blt 0x000a261c
  000a2612: add.w r2,r6,#0x298
  000a2616: movs r1,#0x1
  000a2618: blx 0x00070984
  000a261c: ldr r0,[r4,#0x54]
  000a261e: cmp r0,#0x0
  000a2620: blt 0x000a262c
  000a2622: add.w r2,r6,#0x2b8
  000a2626: movs r1,#0x1
  000a2628: blx 0x00070984
  000a262c: ldr r0,[r6,#0x30]
  000a262e: blx 0x000713d4
  000a2632: blx 0x0006f7d8
  000a2636: vldr.32 s0,[pc,#0x18c]
  000a263a: vmov s2,r0
  000a263e: vdiv.f32 s0,s2,s0
  000a2642: vmov r0,s0
  000a2646: blx 0x00070828
  000a264a: vmov.f32 s0,0x40000000
  000a264e: vmov s2,r0
  000a2652: ldr r0,[r4,#0x58]
  000a2654: vadd.f32 s0,s2,s0
  000a2658: vmov r1,s0
  000a265c: blx 0x00070978
  000a2660: movs r0,#0x0
  000a2662: strb r0,[r4,#0x9]
  000a2664: ldr r0,[r4,#0x20]
  000a2666: cmp r0,#0x0
  000a2668: blt 0x000a266e
  000a266a: blx 0x00070a5c
  000a266e: ldr r0,[r4,#0x24]
  000a2670: cmp r0,#0x0
  000a2672: blt 0x000a2678
  000a2674: blx 0x00070a5c
  000a2678: ldr r0,[r4,#0x28]
  000a267a: cmp r0,#0x0
  000a267c: blt 0x000a2682
  000a267e: blx 0x00070a5c
  000a2682: ldr r0,[r4,#0x2c]
  000a2684: cmp r0,#0x0
  000a2686: blt 0x000a268c
  000a2688: blx 0x00070a5c
  000a268c: ldr r0,[r4,#0x30]
  000a268e: cmp r0,#0x0
  000a2690: blt 0x000a2696
  000a2692: blx 0x00070a5c
  000a2696: ldrb.w r0,[r8,#0x5c]
  000a269a: cmp r0,#0x0
  000a269c: beq 0x000a2732
  000a269e: ldr r0,[0x000a27c8]
  000a26a0: ldr.w r1,[r8,#0x60]
  000a26a4: add r0,pc
  000a26a6: ldr r5,[r0,#0x0]
  000a26a8: movw r0,#0x8892
  000a26ac: blx r5
  000a26ae: ldr r1,[0x000a27cc]
  000a26b0: mov.w r9,#0x0
  000a26b4: ldr r0,[r4,#0x20]
  000a26b6: movw r2,#0x1406
  000a26ba: add r1,pc
  000a26bc: movs r3,#0x0
  000a26be: strd r9,r9,[sp,#0x0]
  000a26c2: ldr r6,[r1,#0x0]
  000a26c4: movs r1,#0x3
  000a26c6: blx r6
  000a26c8: ldr.w r1,[r8,#0x68]
  000a26cc: movw r0,#0x8892
  000a26d0: blx r5
  000a26d2: ldr r0,[r4,#0x24]
  000a26d4: movs r1,#0x2
  000a26d6: movw r2,#0x1406
  000a26da: movs r3,#0x0
  000a26dc: strd r9,r9,[sp,#0x0]
  000a26e0: blx r6
  000a26e2: ldr.w r1,[r8,#0x6c]
  000a26e6: movw r0,#0x8892
  000a26ea: blx r5
  000a26ec: ldr r0,[r4,#0x28]
  000a26ee: movs r1,#0x3
  000a26f0: movw r2,#0x1406
  000a26f4: movs r3,#0x0
  000a26f6: strd r9,r9,[sp,#0x0]
  000a26fa: blx r6
  000a26fc: ldr.w r1,[r8,#0x70]
  000a2700: movw r0,#0x8892
  000a2704: blx r5
  000a2706: ldr r0,[r4,#0x2c]
  000a2708: movs r1,#0x3
  000a270a: movw r2,#0x1406
  000a270e: movs r3,#0x0
  000a2710: strd r9,r9,[sp,#0x0]
  000a2714: blx r6
  000a2716: ldr.w r1,[r8,#0x74]
  000a271a: movw r0,#0x8892
  000a271e: blx r5
  000a2720: ldr r0,[r4,#0x30]
  000a2722: movs r1,#0x3
  000a2724: movw r2,#0x1406
  000a2728: movs r3,#0x0
  000a272a: strd r9,r9,[sp,#0x0]
  000a272e: blx r6
  000a2730: b 0x000a27be
  000a2732: ldr r0,[r4,#0x20]
  000a2734: cmp r0,#0x0
  000a2736: blt 0x000a274e
  000a2738: ldr.w r1,[r8,#0x4]
  000a273c: movs r2,#0x0
  000a273e: movs r3,#0x0
  000a2740: strd r2,r1,[sp,#0x0]
  000a2744: movs r1,#0x3
  000a2746: movw r2,#0x1406
  000a274a: blx 0x000709c0
  000a274e: ldr r0,[r4,#0x24]
  000a2750: cmp r0,#0x0
  000a2752: blt 0x000a276a
  000a2754: ldr.w r1,[r8,#0x8]
  000a2758: movs r2,#0x0
  000a275a: movs r3,#0x0
  000a275c: strd r2,r1,[sp,#0x0]
  000a2760: movs r1,#0x2
  000a2762: movw r2,#0x1406
  000a2766: blx 0x000709c0
  000a276a: ldr r0,[r4,#0x28]
  000a276c: cmp r0,#0x0
  000a276e: blt 0x000a2786
  000a2770: ldr.w r1,[r8,#0x10]
  000a2774: movs r2,#0x0
  000a2776: movs r3,#0x0
  000a2778: strd r2,r1,[sp,#0x0]
  000a277c: movs r1,#0x3
  000a277e: movw r2,#0x1406
  000a2782: blx 0x000709c0
  000a2786: ldr r0,[r4,#0x2c]
  000a2788: cmp r0,#0x0
  000a278a: blt 0x000a27a2
  000a278c: ldr.w r1,[r8,#0x14]
  000a2790: movs r2,#0x0
  000a2792: movs r3,#0x0
  000a2794: strd r2,r1,[sp,#0x0]
  000a2798: movs r1,#0x3
  000a279a: movw r2,#0x1406
  000a279e: blx 0x000709c0
  000a27a2: ldr r0,[r4,#0x30]
  000a27a4: cmp r0,#0x0
  000a27a6: blt 0x000a27be
  000a27a8: ldr.w r1,[r8,#0x18]
  000a27ac: movs r2,#0x0
  000a27ae: movs r3,#0x0
  000a27b0: strd r2,r1,[sp,#0x0]
  000a27b4: movs r1,#0x3
  000a27b6: movw r2,#0x1406
  000a27ba: blx 0x000709c0
  000a27be: pop.w {r2,r3,r8,r9,r11}
  000a27c2: pop {r4,r5,r6,r7,pc}
```
