# BoundingVolume::collide
elf 0x11f870 body 60
Sig: undefined __stdcall collide(float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingVolume::collide(float, float, float) */

undefined4 BoundingVolume::collide(float param_1,float param_2,float param_3)

{
  int in_r0;
  uint *puVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  
  puVar1 = *(uint **)(in_r0 + 4);
  if (puVar1 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      piVar2 = *(int **)(puVar1[1] + uVar4 * 4);
      iVar3 = (**(code **)(*piVar2 + 8))
                        (param_1,param_2,param_3,piVar2,*(undefined4 *)(in_r0 + 8),
                         *(undefined4 *)(in_r0 + 0xc),*(undefined4 *)(in_r0 + 0x10));
      if (iVar3 != 0) {
        return 1;
      }
      puVar1 = *(uint **)(in_r0 + 4);
      param_1 = extraout_s0;
      param_2 = extraout_s1;
      param_3 = extraout_s2;
    }
  }
  return 0;
}

```

## target disasm
```
  0012f870: push {r4,r5,r6,r7,lr}
  0012f872: add r7,sp,#0xc
  0012f874: push.w r11
  0012f878: mov r6,r0
  0012f87a: ldr r0,[r0,#0x4]
  0012f87c: cbz r0,0x0012f8a0
  0012f87e: movs r5,#0x0
  0012f880: b 0x0012f89a
  0012f882: ldr r0,[r0,#0x4]
  0012f884: add.w r3,r6,#0x8
  0012f888: ldr.w r0,[r0,r5,lsl #0x2]
  0012f88c: ldr r4,[r0,#0x0]
  0012f88e: ldmia r3,{r1,r2,r3}
  0012f890: ldr r4,[r4,#0x8]
  0012f892: blx r4
  0012f894: cbnz r0,0x0012f8a4
  0012f896: ldr r0,[r6,#0x4]
  0012f898: adds r5,#0x1
  0012f89a: ldr r1,[r0,#0x0]
  0012f89c: cmp r5,r1
  0012f89e: bcc 0x0012f882
  0012f8a0: movs r0,#0x0
  0012f8a2: b 0x0012f8a6
  0012f8a4: movs r0,#0x1
  0012f8a6: pop.w r11
  0012f8aa: pop {r4,r5,r6,r7,pc}
```
