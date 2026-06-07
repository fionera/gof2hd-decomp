# AERandom::AERandom
elf 0x719e0 body 32
Sig: undefined __thiscall AERandom(AERandom * this)

## decompile
```c

/* AbyssEngine::AERandom::AERandom() */

AERandom * __thiscall AbyssEngine::AERandom::AERandom(AERandom *this)

{
  uint uVar1;
  
  uVar1 = time(0);
  *(uint *)this = DAT_00081a00 ^ uVar1;
  *(uint *)(this + 4) = (ushort)((int)uVar1 >> 0x1f) ^ 5;
  return this;
}

```

## target disasm
```
  000819e0: push {r4,r6,r7,lr}
  000819e2: add r7,sp,#0x8
  000819e4: mov r4,r0
  000819e6: movs r0,#0x0
  000819e8: blx 0x0006f508
  000819ec: ldr r1,[0x00081a00]
  000819ee: eors r1,r0
  000819f0: asrs r0,r0,#0x1f
  000819f2: uxth r0,r0
  000819f4: eor r0,r0,#0x5
  000819f8: strd r1,r0,[r4,#0x0]
  000819fc: mov r0,r4
  000819fe: pop {r4,r6,r7,pc}
```
