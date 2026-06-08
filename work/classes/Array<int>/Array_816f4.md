# Array<int>::Array
elf 0x816f4 body 28
Sig: undefined __stdcall Array(void)

## decompile
```c

/* Array<int>::Array() */

void Array<int>::Array(void)

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
  000916f4: push {r4,r6,r7,lr}
  000916f6: add r7,sp,#0x8
  000916f8: mov r4,r0
  000916fa: movs r0,#0x4
  000916fc: blx 0x0006ec08
  00091700: movs r2,#0x0
  00091702: movs r1,#0x1
  00091704: str r2,[r0,#0x0]
  00091706: strd r2,r0,[r4,#0x0]
  0009170a: mov r0,r4
  0009170c: str r1,[r4,#0x8]
  0009170e: pop {r4,r6,r7,pc}
```
