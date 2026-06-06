# Status::incKills
elf 0xac6c4 body 24
Sig: undefined __thiscall incKills(Status * this)

## decompile
```c

/* Status::incKills() */

void __thiscall Status::incKills(Status *this)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(DAT_000bc6dc + 0xbc6ce);
  *(int *)(this + 0x1c0) = *(int *)(this + 0x1c0) + 1;
  (*(code *)(DAT_001abf74 + 0x1abf78))(*(undefined4 *)*puVar1);
  return;
}

```
## target disasm
```
  000bc6c4: ldr r1,[0x000bc6dc]
  000bc6c6: ldr.w r2,[r0,#0x1c0]
  000bc6ca: add r1,pc
  000bc6cc: adds r2,#0x1
  000bc6ce: str.w r2,[r0,#0x1c0]
  000bc6d2: ldr r1,[r1,#0x0]
  000bc6d4: ldr r0,[r1,#0x0]
  000bc6d6: b.w 0x001abf68
  001abf68: bx pc
```
