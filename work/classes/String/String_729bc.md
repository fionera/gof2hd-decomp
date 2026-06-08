# String::String
elf 0x729bc body 30
Sig: undefined __thiscall String(String * this, float param_1)

## decompile
```c

/* AbyssEngine::String::String(float) */

String * __thiscall AbyssEngine::String::String(String *this,float param_1)

{
  *(int *)this = *(int *)(DAT_000829dc + 0x829ca) + 8;
  *(undefined4 *)(this + 4) = 0;
  Set(this,param_1);
  return this;
}

```

## target disasm
```
  000829bc: push {r4,r6,r7,lr}
  000829be: add r7,sp,#0x8
  000829c0: mov r4,r0
  000829c2: ldr r0,[0x000829dc]
  000829c4: movs r2,#0x0
  000829c6: add r0,pc
  000829c8: ldr r0,[r0,#0x0]
  000829ca: adds r0,#0x8
  000829cc: strd r0,r2,[r4,#0x0]
  000829d0: mov r0,r4
  000829d2: blx 0x0006f5e0
  000829d6: mov r0,r4
  000829d8: pop {r4,r6,r7,pc}
```
