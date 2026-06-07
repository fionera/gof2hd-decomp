# BombGun::render
elf 0x147cf0 body 32
Sig: undefined __thiscall render(BombGun * this)

## decompile
```c

/* BombGun::render() */

void __thiscall BombGun::render(BombGun *this)

{
  RocketGun::render();
  if (*(char *)(*(int *)(this + 8) + 0x88) == '\0') {
    return;
  }
  (*(code *)(DAT_001ac2b4 + 0x1ac2b8))(*(undefined4 *)(this + 0xf0));
  return;
}

```

## target disasm
```
  00157cf0: push {r4,r6,r7,lr}
  00157cf2: add r7,sp,#0x8
  00157cf4: mov r4,r0
  00157cf6: blx 0x00076e28
  00157cfa: ldr r0,[r4,#0x8]
  00157cfc: ldrb.w r0,[r0,#0x88]
  00157d00: cbz r0,0x00157d0e
  00157d02: ldr.w r0,[r4,#0xf0]
  00157d06: pop.w {r4,r6,r7,lr}
  00157d0a: b.w 0x001ac2a8
  00157d0e: pop {r4,r6,r7,pc}
```
