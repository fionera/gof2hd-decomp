# AERandom::AERandom
elf 0x71a18 body 16
Sig: undefined __thiscall AERandom(AERandom * this, longlong param_1)

## decompile
```c

/* AbyssEngine::AERandom::AERandom(long long) */

void __thiscall AbyssEngine::AERandom::AERandom(AERandom *this,longlong param_1)

{
  *(uint *)this = DAT_00081a28 ^ (uint)param_1;
  *(uint *)(this + 4) = (ushort)((ulonglong)param_1 >> 0x20) ^ 5;
  return;
}

```

## target disasm
```
  00081a18: uxth r3,r3
  00081a1a: ldr r1,[0x00081a28]
  00081a1c: eor r3,r3,#0x5
  00081a20: eors r1,r2
  00081a22: strd r1,r3,[r0,#0x0]
  00081a26: bx lr
```
