# Array<TouchButton*>::Array
elf 0xd7004 body 28
Sig: undefined __stdcall Array(void)

## decompile
```c

/* Array<TouchButton*>::Array() */

void Array<TouchButton*>::Array(void)

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
  000e7004: push {r4,r6,r7,lr}
  000e7006: add r7,sp,#0x8
  000e7008: mov r4,r0
  000e700a: movs r0,#0x4
  000e700c: blx 0x0006ec08
  000e7010: movs r2,#0x0
  000e7012: movs r1,#0x1
  000e7014: str r2,[r0,#0x0]
  000e7016: strd r2,r0,[r4,#0x0]
  000e701a: mov r0,r4
  000e701c: str r1,[r4,#0x8]
  000e701e: pop {r4,r6,r7,pc}
```
