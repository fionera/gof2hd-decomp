# PlayerCreature::render
elf 0x11cfb8 body 36
Sig: undefined __thiscall render(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::render() */

void __thiscall PlayerCreature::render(PlayerCreature *this)

{
  if (*(int *)(this + 0x78) != 0) {
    AEGeometry::render();
  }
  if (1 < *(int *)(this + 0x88) - 3U) {
                    /* WARNING: Could not recover jumptable at 0x001ac3b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(DAT_001ac3b4 + 0x1ac3b8))(this);
    return;
  }
  return;
}

```

## target disasm
```
  0012cfb8: push {r4,r6,r7,lr}
  0012cfba: add r7,sp,#0x8
  0012cfbc: mov r4,r0
  0012cfbe: ldr r0,[r0,#0x78]
  0012cfc0: cbz r0,0x0012cfc6
  0012cfc2: blx 0x00072238
  0012cfc6: ldr.w r0,[r4,#0x88]
  0012cfca: subs r0,#0x3
  0012cfcc: cmp r0,#0x2
  0012cfce: it cc
  0012cfd0: pop.cc {r4,r6,r7,pc}
  0012cfd2: mov r0,r4
  0012cfd4: pop.w {r4,r6,r7,lr}
  0012cfd8: b.w 0x001ac3a8
```
