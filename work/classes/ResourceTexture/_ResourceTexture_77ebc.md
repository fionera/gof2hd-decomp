# ResourceTexture::~ResourceTexture
elf 0x77ebc body 20
Sig: undefined __thiscall ~ResourceTexture(ResourceTexture * this)

## decompile
```c

/* AbyssEngine::ResourceTexture::~ResourceTexture() */

ResourceTexture * __thiscall AbyssEngine::ResourceTexture::~ResourceTexture(ResourceTexture *this)

{
  operator_delete__(*(void **)this);
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  00087ebc: push {r4,r6,r7,lr}
  00087ebe: add r7,sp,#0x8
  00087ec0: mov r4,r0
  00087ec2: ldr r0,[r0,#0x0]
  00087ec4: blx 0x0006ebfc
  00087ec8: movs r0,#0x0
  00087eca: str r0,[r4,#0x0]
  00087ecc: mov r0,r4
  00087ece: pop {r4,r6,r7,pc}
```
