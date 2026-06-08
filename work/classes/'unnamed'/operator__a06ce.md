# 'unnamed'::operator=
elf 0xa06ce body 30
Sig: undefined __thiscall operator=({unnamed_type#1} * this, {unnamed * param_1)

## decompile
```c

/* ParticleSettings::{unnamed type#1}::TEMPNAMEPLACEHOLDERVALUE({unnamed type#1} const&) */

_unnamed_type_1_ * __thiscall
ParticleSettings::{unnamed_type#1}::operator=(_unnamed_type_1_ *this,_unnamed *param_1)

{
  AbyssEngine::String::operator=((String *)this,param_1);
  __aeabi_memcpy4(this + 0xc,param_1 + 0xc,0x94);
  return this;
}

```

## target disasm
```
  000b06ce: push {r4,r5,r7,lr}
  000b06d0: add r7,sp,#0x8
  000b06d2: mov r4,r1
  000b06d4: mov r5,r0
  000b06d6: blx 0x0006f2b0
  000b06da: add.w r0,r5,#0xc
  000b06de: add.w r1,r4,#0xc
  000b06e2: movs r2,#0x94
  000b06e4: blx 0x0006f6a0
  000b06e8: mov r0,r5
  000b06ea: pop {r4,r5,r7,pc}
```
