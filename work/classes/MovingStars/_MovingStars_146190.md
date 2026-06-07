# MovingStars::~MovingStars
elf 0x146190 body 54
Sig: undefined __thiscall ~MovingStars(MovingStars * this)

## decompile
```c

/* MovingStars::~MovingStars() */

MovingStars * __thiscall MovingStars::~MovingStars(MovingStars *this)

{
  if (*(void **)this != (void *)0x0) {
    operator_delete__(*(void **)this);
  }
  *(undefined4 *)this = 0;
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(void **)(this + 0xc) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0xc));
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(void **)(this + 0x10) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x10));
  }
  *(undefined4 *)(this + 0x10) = 0;
  return this;
}

```

## target disasm
```
  00156190: push {r4,r5,r7,lr}
  00156192: add r7,sp,#0x8
  00156194: mov r4,r0
  00156196: ldr r0,[r0,#0x0]
  00156198: cbz r0,0x0015619e
  0015619a: blx 0x0006ebfc
  0015619e: ldr r0,[r4,#0x4]
  001561a0: movs r5,#0x0
  001561a2: str r5,[r4,#0x0]
  001561a4: cbz r0,0x001561aa
  001561a6: blx 0x0006ebfc
  001561aa: ldr r0,[r4,#0xc]
  001561ac: str r5,[r4,#0x4]
  001561ae: cbz r0,0x001561b4
  001561b0: blx 0x0006ebfc
  001561b4: ldr r0,[r4,#0x10]
  001561b6: movs r5,#0x0
  001561b8: str r5,[r4,#0xc]
  001561ba: cbz r0,0x001561c0
  001561bc: blx 0x0006ebfc
  001561c0: mov r0,r4
  001561c2: str r5,[r4,#0x10]
  001561c4: pop {r4,r5,r7,pc}
```
