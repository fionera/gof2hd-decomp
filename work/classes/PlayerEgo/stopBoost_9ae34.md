# PlayerEgo::stopBoost
elf 0x9ae34 body 68
Sig: undefined __stdcall stopBoost(void)

## decompile
```c

/* PlayerEgo::stopBoost() */

void PlayerEgo::stopBoost(void)

{
  int iVar1;
  int in_r0;
  undefined4 *puVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  iVar1 = DAT_000aae7c;
  puVar2 = (undefined4 *)(DAT_000aae78 + 0xaae42);
  *(undefined1 *)(in_r0 + 0x13c) = 0;
  puVar2 = (undefined4 *)*puVar2;
  *(undefined4 *)(in_r0 + 0xb8) = 0x40000000;
  UNRECOVERED_JUMPTABLE = *(code **)(iVar1 + 0xaae48);
  (*UNRECOVERED_JUMPTABLE)(*puVar2,0x27);
  (*UNRECOVERED_JUMPTABLE)(*puVar2,0x26);
  (*UNRECOVERED_JUMPTABLE)(*puVar2,0x29);
  (*UNRECOVERED_JUMPTABLE)(*puVar2,0x28);
                    /* WARNING: Could not recover jumptable at 0x000aae76. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(*puVar2,0x44e);
  return;
}

```

## target disasm
```
  000aae34: push {r4,r5,r7,lr}
  000aae36: add r7,sp,#0x8
  000aae38: ldr r1,[0x000aae78]
  000aae3a: movs r3,#0x0
  000aae3c: ldr r2,[0x000aae7c]
  000aae3e: add r1,pc
  000aae40: strb.w r3,[r0,#0x13c]
  000aae44: add r2,pc
  000aae46: ldr r4,[r1,#0x0]
  000aae48: mov.w r1,#0x40000000
  000aae4c: str.w r1,[r0,#0xb8]
  000aae50: movs r1,#0x27
  000aae52: ldr r5,[r2,#0x0]
  000aae54: ldr r0,[r4,#0x0]
  000aae56: blx r5
  000aae58: ldr r0,[r4,#0x0]
  000aae5a: movs r1,#0x26
  000aae5c: blx r5
  000aae5e: ldr r0,[r4,#0x0]
  000aae60: movs r1,#0x29
  000aae62: blx r5
  000aae64: ldr r0,[r4,#0x0]
  000aae66: movs r1,#0x28
  000aae68: blx r5
  000aae6a: ldr r0,[r4,#0x0]
  000aae6c: movw r1,#0x44e
  000aae70: mov r2,r5
  000aae72: pop.w {r4,r5,r7,lr}
  000aae76: bx r2
```
