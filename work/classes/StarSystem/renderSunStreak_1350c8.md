# StarSystem::renderSunStreak
elf 0x1350c8 body 56
Sig: undefined __thiscall renderSunStreak(StarSystem * this)

## decompile
```c

/* StarSystem::renderSunStreak() */

void __thiscall StarSystem::renderSunStreak(StarSystem *this)

{
  StarSystem *pSVar1;
  uint *puVar2;
  
  puVar2 = *(uint **)(DAT_00145100 + 0x1450da);
  if (this[0xc] == (StarSystem)0x0) {
    pSVar1 = *(StarSystem **)(*(int *)(this + 0x14) + 4);
  }
  else {
    pSVar1 = this + 0x10;
  }
  AbyssEngine::PaintCanvas::SetTexture(*puVar2,*(uint *)pSVar1);
  AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar2,2);
  (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(this + 0x40));
  return;
}

```

## target disasm
```
  001450c8: push {r4,r5,r7,lr}
  001450ca: add r7,sp,#0x8
  001450cc: mov r4,r0
  001450ce: ldr r0,[0x00145100]
  001450d0: ldrb r1,[r4,#0xc]
  001450d2: mov.w r2,#0xffffffff
  001450d6: add r0,pc
  001450d8: cmp r1,#0x0
  001450da: ldr r5,[r0,#0x0]
  001450dc: ldr r0,[r5,#0x0]
  001450de: itte eq
  001450e0: ldr.eq r1,[r4,#0x14]
  001450e2: ldr.eq r1,[r1,#0x4]
  001450e4: add.ne.w r1,r4,#0x10
  001450e8: ldr r1,[r1,#0x0]
  001450ea: blx 0x0006fa6c
  001450ee: ldr r0,[r5,#0x0]
  001450f0: movs r1,#0x2
  001450f2: blx 0x0006febc
  001450f6: ldr r0,[r4,#0x40]
  001450f8: pop.w {r4,r5,r7,lr}
  001450fc: b.w 0x001abdc8
```
