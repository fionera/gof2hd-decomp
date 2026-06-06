# Status::incPirateKills
elf 0xac6ec body 24
Sig: undefined __stdcall incPirateKills(void)

## decompile
```c

/* Status::incPirateKills() */

void Status::incPirateKills(void)

{
  int in_r0;
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(DAT_000bc704 + 0xbc6f6);
  *(int *)(in_r0 + 0x1d8) = *(int *)(in_r0 + 0x1d8) + 1;
  (*(code *)(DAT_001abf84 + 0x1abf88))(*(undefined4 *)*puVar1);
  return;
}

```
## target disasm
```
  000bc6ec: ldr r1,[0x000bc704]
  000bc6ee: ldr.w r2,[r0,#0x1d8]
  000bc6f2: add r1,pc
  000bc6f4: adds r2,#0x1
  000bc6f6: str.w r2,[r0,#0x1d8]
  000bc6fa: ldr r1,[r1,#0x0]
  000bc6fc: ldr r0,[r1,#0x0]
  000bc6fe: b.w 0x001abf78
  001abf78: bx pc
```
