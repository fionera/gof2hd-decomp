# Sprite::~Sprite
elf 0xd0e30 body 22
Sig: undefined __thiscall ~Sprite(Sprite * this)

## decompile
```c

/* Sprite::~Sprite() */

Sprite * __thiscall Sprite::~Sprite(Sprite *this)

{
  if (*(void **)this != (void *)0x0) {
    operator_delete__(*(void **)this);
  }
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  000e0e30: push {r4,r6,r7,lr}
  000e0e32: add r7,sp,#0x8
  000e0e34: mov r4,r0
  000e0e36: ldr r0,[r0,#0x0]
  000e0e38: cbz r0,0x000e0e3e
  000e0e3a: blx 0x0006ebfc
  000e0e3e: movs r0,#0x0
  000e0e40: str r0,[r4,#0x0]
  000e0e42: mov r0,r4
  000e0e44: pop {r4,r6,r7,pc}
```
