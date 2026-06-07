# ParticleSystemMesh::reset
elf 0x18620a body 94
Sig: undefined __stdcall reset(void)

## decompile
```c

/* ParticleSystemMesh::reset() */

void ParticleSystemMesh::reset(void)

{
  int in_r0;
  int iVar1;
  float in_s0;
  float extraout_s0;
  float in_s1;
  float extraout_s1;
  float in_s2;
  float extraout_s2;
  
  for (iVar1 = 0; iVar1 < *(int *)(in_r0 + 0x70); iVar1 = iVar1 + 1) {
    AbyssEngine::PaintCanvas::MeshSetPoint
              (*(uint *)(in_r0 + 8),(ushort)*(undefined4 *)(in_r0 + 0x54),in_s0,in_s1,in_s2);
    in_s0 = extraout_s0;
    in_s1 = extraout_s1;
    in_s2 = extraout_s2;
  }
  for (iVar1 = 0; iVar1 < *(int *)(in_r0 + 0x48); iVar1 = iVar1 + 1) {
    *(undefined4 *)(*(int *)(in_r0 + 0x68) + iVar1 * 4) = 0xffffffff;
  }
  *(undefined4 *)(in_r0 + 0x94) = 0;
  *(undefined4 *)(in_r0 + 0x50) = 0;
  *(undefined1 *)(in_r0 + 0x90) = 1;
  *(undefined4 *)(in_r0 + 0x60) = 0;
  *(undefined1 *)(in_r0 + 4) = 1;
  return;
}

```

## target disasm
```
  0019620a: push {r4,r5,r6,r7,lr}
  0019620c: add r7,sp,#0xc
  0019620e: push.w r11
  00196212: sub sp,#0x8
  00196214: mov r4,r0
  00196216: movs r5,#0x0
  00196218: movs r6,#0x0
  0019621a: b 0x00196232
  0019621c: ldr r0,[r4,#0x8]
  0019621e: movs r3,#0x0
  00196220: ldrd r1,r2,[r4,#0x54]
  00196224: add r2,r6
  00196226: strd r5,r5,[sp,#0x0]
  0019622a: uxth r2,r2
  0019622c: blx 0x000777f4
  00196230: adds r6,#0x1
  00196232: ldr r0,[r4,#0x70]
  00196234: cmp r6,r0
  00196236: blt 0x0019621c
  00196238: movs r0,#0x0
  0019623a: mov.w r1,#0xffffffff
  0019623e: b 0x00196248
  00196240: ldr r2,[r4,#0x68]
  00196242: str.w r1,[r2,r0,lsl #0x2]
  00196246: adds r0,#0x1
  00196248: ldr r2,[r4,#0x48]
  0019624a: cmp r0,r2
  0019624c: blt 0x00196240
  0019624e: movs r0,#0x0
  00196250: movs r1,#0x1
  00196252: str.w r0,[r4,#0x94]
  00196256: str r0,[r4,#0x50]
  00196258: strb.w r1,[r4,#0x90]
  0019625c: str r0,[r4,#0x60]
  0019625e: strb r1,[r4,#0x4]
  00196260: add sp,#0x8
  00196262: pop.w r11
  00196266: pop {r4,r5,r6,r7,pc}
```
