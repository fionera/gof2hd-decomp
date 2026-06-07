# BoundingAAB::update
elf 0x9827a body 16
Sig: undefined __stdcall update(float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingAAB::update(float, float, float) */

void BoundingAAB::update(float param_1,float param_2,float param_3)

{
                    /* WARNING: Could not recover jumptable at 0x001ab9a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab9a4 + 0x1ab9a8))();
  return;
}

```

## target disasm
```
  00071f80: adr r12,0x171f88
  00071f84: add r12,r12,#0xaf000
  00071f88: ldr pc,[r12,#0x990]!
  000a827a: b.w 0x001ab998
```
