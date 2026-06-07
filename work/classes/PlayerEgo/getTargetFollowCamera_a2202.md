# PlayerEgo::getTargetFollowCamera
elf 0xa2202 body 6
Sig: undefined __stdcall getTargetFollowCamera(void)

## decompile
```c

/* PlayerEgo::getTargetFollowCamera() */

undefined4 PlayerEgo::getTargetFollowCamera(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x88);
}

```

## target disasm
```
  000b2202: ldr.w r0,[r0,#0x88]
  000b2206: bx lr
```
