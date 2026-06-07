# PlayerEgo::ResumeEngineSound
elf 0x9a5e2 body 8
Sig: undefined __stdcall ResumeEngineSound(void)

## decompile
```c

/* PlayerEgo::ResumeEngineSound() */

void PlayerEgo::ResumeEngineSound(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001aba34 + 0x1aba38))(*in_r0,0);
  return;
}

```

## target disasm
```
  000aa5e2: ldr r0,[r0,#0x0]
  000aa5e4: movs r1,#0x0
  000aa5e6: b.w 0x001aba28
```
