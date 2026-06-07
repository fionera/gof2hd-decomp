# PlayerStatic::render
elf 0xe09b8 body 6
Sig: undefined __stdcall render(void)

## decompile
```c

/* PlayerStatic::render() */

void PlayerStatic::render(void)

{
  int in_r0;
  
  (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(in_r0 + 8));
  return;
}

```

## target disasm
```
  000f09b8: ldr r0,[r0,#0x8]
  000f09ba: b.w 0x001abdc8
```
