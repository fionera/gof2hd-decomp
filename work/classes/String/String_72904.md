# String::String
elf 0x72904 body 30
Sig: undefined __thiscall String(String * this, longlong param_1)

## decompile
```c

/* AbyssEngine::String::String(long long) */

String * __thiscall AbyssEngine::String::String(String *this,longlong param_1)

{
  *(int *)this = *(int *)(DAT_00082924 + 0x82912) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(ZEXT48(this));
  return this;
}

```

## target disasm
```
  00082904: push {r4,r6,r7,lr}
  00082906: add r7,sp,#0x8
  00082908: mov r4,r0
  0008290a: ldr r0,[0x00082924]
  0008290c: movs r1,#0x0
  0008290e: add r0,pc
  00082910: ldr r0,[r0,#0x0]
  00082912: adds r0,#0x8
  00082914: strd r0,r1,[r4,#0x0]
  00082918: mov r0,r4
  0008291a: blx 0x0006efd4
  0008291e: mov r0,r4
  00082920: pop {r4,r6,r7,pc}
```
