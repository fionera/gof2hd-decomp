# ParticleSettingsRef::initialize
elf 0xe3964 body 36
Sig: undefined __stdcall initialize(void)

## decompile
```c

/* ParticleSettingsRef::initialize() */

void ParticleSettingsRef::initialize(void)

{
  EVP_PKEY_CTX *in_r1;
  EVP_PKEY_CTX *ctx;
  
  ParticleSettings::init(*(ParticleSettings **)(DAT_000f3988 + 0xf396e),in_r1);
  ParticleSettings::init(*(ParticleSettings **)(DAT_000f398c + 0xf3978),ctx);
  **(undefined4 **)(DAT_000f3990 + 0xf3984) = 0x2a;
  return;
}

```

## target disasm
```
  000f3964: push {r7,lr}
  000f3966: mov r7,sp
  000f3968: ldr r0,[0x000f3988]
  000f396a: add r0,pc
  000f396c: ldr r0,[r0,#0x0]
  000f396e: blx 0x00075d9c
  000f3972: ldr r0,[0x000f398c]
  000f3974: add r0,pc
  000f3976: ldr r0,[r0,#0x0]
  000f3978: blx 0x00075d9c
  000f397c: ldr r0,[0x000f3990]
  000f397e: movs r1,#0x2a
  000f3980: add r0,pc
  000f3982: ldr r0,[r0,#0x0]
  000f3984: str r1,[r0,#0x0]
  000f3986: pop {r7,pc}
```
