# PlayerAsteroid::~PlayerAsteroid
elf 0xe2980 body 46
Sig: undefined __thiscall ~PlayerAsteroid(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::~PlayerAsteroid() */

void __thiscall PlayerAsteroid::~PlayerAsteroid(PlayerAsteroid *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_000f29b0 + 0xf298c) + 8;
  if (*(Explosion **)(this + 300) != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(*(Explosion **)(this + 300));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 300) = 0;
  (*(code *)(DAT_001ac1a4 + 0x1ac1a8))(this);
  return;
}

```

## target disasm
```
  000f2980: push {r4,r6,r7,lr}
  000f2982: add r7,sp,#0x8
  000f2984: mov r4,r0
  000f2986: ldr r0,[0x000f29b0]
  000f2988: add r0,pc
  000f298a: ldr r1,[r0,#0x0]
  000f298c: ldr.w r0,[r4,#0x12c]
  000f2990: adds r1,#0x8
  000f2992: str r1,[r4,#0x0]
  000f2994: cbz r0,0x000f299e
  000f2996: blx 0x00072334
  000f299a: blx 0x0006eb48
  000f299e: movs r0,#0x0
  000f29a0: str.w r0,[r4,#0x12c]
  000f29a4: mov r0,r4
  000f29a6: pop.w {r4,r6,r7,lr}
  000f29aa: b.w 0x001ac198
```
