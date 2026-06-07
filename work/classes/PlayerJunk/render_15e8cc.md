# PlayerJunk::render
elf 0x15e8cc body 58
Sig: undefined __thiscall render(PlayerJunk * this)

## decompile
```c

/* PlayerJunk::render() */

void __thiscall PlayerJunk::render(PlayerJunk *this)

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
  0007336c: adr r12,0x173374
  00073370: add r12,r12,#0xae000
  00073374: ldr pc,[r12,#0xc48]!
  0016e8cc: push {r4,r6,r7,lr}
  0016e8ce: add r7,sp,#0x8
  0016e8d0: mov r4,r0
  0016e8d2: ldr r0,[r0,#0x78]
  0016e8d4: cbz r0,0x0016e8da
  0016e8d6: blx 0x00072238
  0016e8da: ldr.w r0,[r4,#0x88]
  0016e8de: subs r0,#0x3
  0016e8e0: cmp r0,#0x2
  0016e8e2: it cc
  0016e8e4: pop.cc {r4,r6,r7,pc}
  0016e8e6: mov r0,r4
  0016e8e8: pop.w {r4,r6,r7,lr}
  0016e8ec: b.w 0x001ac3a8
  001ac3a8: bx pc
  001ac3ac: ldr r12,[0x1ac3b4]
  001ac3b0: add pc,r12,pc
```
