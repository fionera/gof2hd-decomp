# PlayerTurret::render
elf 0x157bcc body 52
Sig: undefined __thiscall render(PlayerTurret * this)

## decompile
```c

/* PlayerTurret::render() */

void __thiscall PlayerTurret::render(PlayerTurret *this)

{
  int iVar1;
  
  if (*(int *)(this + 0x78) != 0) {
    AEGeometry::render();
  }
  iVar1 = *(int *)(this + 0x88);
  if (iVar1 == 3) {
    Explosion::render();
    iVar1 = *(int *)(this + 0x88);
  }
  if (1 < iVar1 - 3U) {
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
  00167bcc: push {r4,r6,r7,lr}
  00167bce: add r7,sp,#0x8
  00167bd0: mov r4,r0
  00167bd2: ldr r0,[r0,#0x78]
  00167bd4: cbz r0,0x00167bda
  00167bd6: blx 0x00072238
  00167bda: ldr.w r0,[r4,#0x88]
  00167bde: cmp r0,#0x3
  00167be0: bne 0x00167bee
  00167be2: ldr.w r0,[r4,#0x13c]
  00167be6: blx 0x00072d18
  00167bea: ldr.w r0,[r4,#0x88]
  00167bee: subs r0,#0x3
  00167bf0: cmp r0,#0x2
  00167bf2: it cc
  00167bf4: pop.cc {r4,r6,r7,pc}
  00167bf6: mov r0,r4
  00167bf8: pop.w {r4,r6,r7,lr}
  00167bfc: b.w 0x001ac3a8
```
