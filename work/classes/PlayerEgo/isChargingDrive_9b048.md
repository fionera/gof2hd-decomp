# PlayerEgo::isChargingDrive
elf 0x9b048 body 6
Sig: undefined __stdcall isChargingDrive(void)

## decompile
```c

/* PlayerEgo::isChargingDrive() */

undefined1 PlayerEgo::isChargingDrive(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x204);
}

```

## target disasm
```
  000ab048: ldrb.w r0,[r0,#0x204]
  000ab04c: bx lr
```
