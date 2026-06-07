# ImageFactory::~ImageFactory
elf 0x11c6b0 body 26
Sig: undefined __thiscall ~ImageFactory(ImageFactory * this)

## decompile
```c

/* ImageFactory::~ImageFactory() */

ImageFactory * __thiscall ImageFactory::~ImageFactory(ImageFactory *this)

{
  void *pvVar1;
  
  if (*(Sprite **)this != (Sprite *)0x0) {
    pvVar1 = (void *)Sprite::~Sprite(*(Sprite **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  0012c6b0: push {r4,r6,r7,lr}
  0012c6b2: add r7,sp,#0x8
  0012c6b4: mov r4,r0
  0012c6b6: ldr r0,[r0,#0x0]
  0012c6b8: cbz r0,0x0012c6c2
  0012c6ba: blx 0x0007600c
  0012c6be: blx 0x0006eb48
  0012c6c2: movs r0,#0x0
  0012c6c4: str r0,[r4,#0x0]
  0012c6c6: mov r0,r4
  0012c6c8: pop {r4,r6,r7,pc}
```
