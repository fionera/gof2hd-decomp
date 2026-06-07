# BombGun::~BombGun
elf 0x147824 body 48
Sig: undefined __thiscall ~BombGun(BombGun * this)

## decompile
```c

/* BombGun::~BombGun() */

void __thiscall BombGun::~BombGun(BombGun *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_00157854 + 0x157830) + 8;
  if (*(Explosion **)(this + 0xf0) != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(*(Explosion **)(this + 0xf0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xf0) = 0;
  (*(code *)(DAT_001ac494 + 0x1ac498))(this);
  return;
}

```

## target disasm
```
  00157824: push {r4,r6,r7,lr}
  00157826: add r7,sp,#0x8
  00157828: mov r4,r0
  0015782a: ldr r0,[0x00157854]
  0015782c: add r0,pc
  0015782e: ldr r1,[r0,#0x0]
  00157830: ldr.w r0,[r4,#0xf0]
  00157834: adds r1,#0x8
  00157836: str r1,[r4,#0x0]
  00157838: cbz r0,0x00157842
  0015783a: blx 0x00072334
  0015783e: blx 0x0006eb48
  00157842: movs r0,#0x0
  00157844: str.w r0,[r4,#0xf0]
  00157848: mov r0,r4
  0015784a: pop.w {r4,r6,r7,lr}
  0015784e: b.w 0x001ac488
  001ac488: bx pc
```
