# AERandom::reset
elf 0x71a30 body 30
Sig: undefined __stdcall reset(void)

## decompile
```c

/* AbyssEngine::AERandom::reset() */

void AbyssEngine::AERandom::reset(void)

{
  uint *in_r0;
  uint uVar1;
  
  uVar1 = time(0);
  *in_r0 = DAT_00081a50 ^ uVar1;
  in_r0[1] = (ushort)((int)uVar1 >> 0x1f) ^ 5;
  return;
}

```

## target disasm
```
  00081a30: push {r4,r6,r7,lr}
  00081a32: add r7,sp,#0x8
  00081a34: mov r4,r0
  00081a36: movs r0,#0x0
  00081a38: blx 0x0006f508
  00081a3c: ldr r1,[0x00081a50]
  00081a3e: eors r1,r0
  00081a40: asrs r0,r0,#0x1f
  00081a42: uxth r0,r0
  00081a44: eor r0,r0,#0x5
  00081a48: strd r1,r0,[r4,#0x0]
  00081a4c: pop {r4,r6,r7,pc}
```
