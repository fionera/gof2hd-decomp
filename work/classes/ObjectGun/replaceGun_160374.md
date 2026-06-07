# ObjectGun::replaceGun
elf 0x160374 body 60
Sig: undefined __stdcall replaceGun(uint param_1, int param_2)

## decompile
```c

/* ObjectGun::replaceGun(unsigned int, int) */

void ObjectGun::replaceGun(uint param_1,int param_2)

{
  uint *puVar1;
  
  puVar1 = *(uint **)(DAT_001703a8 + 0x170388);
  AbyssEngine::PaintCanvas::TransformRemoveMesh(*puVar1,(ushort)*(undefined4 *)(param_1 + 0x10));
  *(int *)(param_1 + 0x28) = param_2;
  (*(code *)(DAT_001abd44 + 0x1abd48))(*puVar1,*(undefined4 *)(param_1 + 0x10),param_2 & 0xffff,0);
  return;
}

```

## target disasm
```
  00170374: push {r4,r5,r6,r7,lr}
  00170376: add r7,sp,#0xc
  00170378: push.w r11
  0017037c: mov r5,r0
  0017037e: ldr r0,[0x001703a8]
  00170380: mov r4,r1
  00170382: ldrh r2,[r5,#0x28]
  00170384: add r0,pc
  00170386: ldr r1,[r5,#0x10]
  00170388: ldr r6,[r0,#0x0]
  0017038a: ldr r0,[r6,#0x0]
  0017038c: blx 0x00076dd4
  00170390: str r4,[r5,#0x28]
  00170392: uxth r2,r4
  00170394: ldr r1,[r5,#0x10]
  00170396: movs r3,#0x0
  00170398: ldr r0,[r6,#0x0]
  0017039a: pop.w r11
  0017039e: pop.w {r4,r5,r6,r7,lr}
  001703a2: b.w 0x001abd38
  001abd38: bx pc
  001abd3c: ldr r12,[0x1abd44]
  001abd40: add pc,r12,pc
```
