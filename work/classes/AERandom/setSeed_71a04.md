# AERandom::setSeed
elf 0x71a04 body 16
Sig: undefined __stdcall setSeed(longlong param_1)

## decompile
```c

/* AbyssEngine::AERandom::setSeed(long long) */

void AbyssEngine::AERandom::setSeed(longlong param_1)

{
  uint in_r2;
  ushort in_r3;
  
  *(uint *)param_1 = DAT_00081a14 ^ in_r2;
  ((uint *)param_1)[1] = in_r3 ^ 5;
  return;
}

```

## target disasm
```
  00081a04: uxth r3,r3
  00081a06: ldr r1,[0x00081a14]
  00081a08: eor r3,r3,#0x5
  00081a0c: eors r1,r2
  00081a0e: strd r1,r3,[r0,#0x0]
  00081a12: bx lr
```
