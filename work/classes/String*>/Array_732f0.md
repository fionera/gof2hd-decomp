# String*>::Array
elf 0x732f0 body 28
Sig: undefined __stdcall Array(void)

## decompile
```c

/* Array<AbyssEngine::String*>::Array() */

void Array<AbyssEngine::String*>::Array(void)

{
  undefined4 *in_r0;
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *in_r0 = 0;
  in_r0[1] = puVar1;
  in_r0[2] = 1;
  return;
}

```

## target disasm
```
  000832f0: push {r4,r6,r7,lr}
  000832f2: add r7,sp,#0x8
  000832f4: mov r4,r0
  000832f6: movs r0,#0x4
  000832f8: blx 0x0006ec08
  000832fc: movs r2,#0x0
  000832fe: movs r1,#0x1
  00083300: str r2,[r0,#0x0]
  00083302: strd r2,r0,[r4,#0x0]
  00083306: mov r0,r4
  00083308: str r1,[r4,#0x8]
  0008330a: pop {r4,r6,r7,pc}
```
