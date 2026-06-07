# PlayerEgo::PauseEngineSound
elf 0x9a5dc body 6
Sig: undefined __stdcall PauseEngineSound(void)

## decompile
```c

/* PlayerEgo::PauseEngineSound() */

void PlayerEgo::PauseEngineSound(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001aba24 + 0x1aba28))(*in_r0);
  return;
}

```

## target disasm
```
  000aa5dc: ldr r0,[r0,#0x0]
  000aa5de: b.w 0x001aba18
```
