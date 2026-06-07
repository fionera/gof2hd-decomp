# BoundingSphere::update
elf 0x151e70 body 14
Sig: undefined __stdcall update(float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingSphere::update(float, float, float) */

void BoundingSphere::update(float param_1,float param_2,float param_3)

{
                    /* WARNING: Could not recover jumptable at 0x001ab9a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab9a4 + 0x1ab9a8))();
  return;
}

```

## target disasm
```
  00161e70: b.w 0x001ab998
  001ab998: bx pc
  001ab99c: ldr r12,[0x1ab9a4]
  001ab9a0: add pc,r12,pc
```
