# KIPlayer::render
elf 0xa632c body 6
Sig: undefined __stdcall render(void)

## decompile
```c

/* KIPlayer::render() */

void KIPlayer::render(void)

{
  int in_r0;
  
  (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(in_r0 + 8));
  return;
}

```

## target disasm
```
  000b632c: ldr r0,[r0,#0x8]
  000b632e: b.w 0x001abdc8
```
