# BeamGun::render
elf 0x17787c body 50
Sig: undefined __thiscall render(BeamGun * this)

## decompile
```c

/* BeamGun::render() */

void __thiscall BeamGun::render(BeamGun *this)

{
  if (((*(char *)(*(int *)(this + 8) + 0x4c) != '\0') &&
      (AEGeometry::render(), this[0x21] != (BeamGun)0x0)) && (*(int *)(this + 0x1c) != 0)) {
    (*(code *)(DAT_001abdd4 + 0x1abdd8))();
    return;
  }
  return;
}

```

## target disasm
```
  0018787c: push {r4,r6,r7,lr}
  0018787e: add r7,sp,#0x8
  00187880: mov r4,r0
  00187882: ldr r0,[r0,#0x8]
  00187884: ldrb.w r0,[r0,#0x4c]
  00187888: cbz r0,0x001878a2
  0018788a: ldr r0,[r4,#0x18]
  0018788c: blx 0x00072238
  00187890: ldrb.w r0,[r4,#0x21]
  00187894: cbz r0,0x001878a2
  00187896: ldr r0,[r4,#0x1c]
  00187898: cbz r0,0x001878a2
  0018789a: pop.w {r4,r6,r7,lr}
  0018789e: b.w 0x001abdc8
  001878a2: pop {r4,r6,r7,pc}
  001abdc8: bx pc
  001abdcc: ldr r12,[0x1abdd4]
  001abdd0: add pc,r12,pc
```
