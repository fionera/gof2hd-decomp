# PlayerEgo::resetGunDelay
elf 0xa15f4 body 10
Sig: undefined __stdcall resetGunDelay(void)

## decompile
```c

/* PlayerEgo::resetGunDelay() */

void PlayerEgo::resetGunDelay(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001abc04 + 0x1abc08))(*in_r0,0);
  return;
}

```

## target disasm
```
  000b15f4: ldr r0,[r0,#0x0]
  000b15f6: movs r1,#0x0
  000b15f8: b.w 0x001abbf8
  001abbf8: bx pc
```
