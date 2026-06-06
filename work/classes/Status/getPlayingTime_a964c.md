# Status::getPlayingTime
elf 0xa964c body 6
Sig: undefined __stdcall getPlayingTime(void)

## decompile
```c

/* Status::getPlayingTime() */

undefined8 Status::getPlayingTime(void)

{
  int in_r0;
  
  return *(undefined8 *)(in_r0 + 0x1b8);
}

```
## target disasm
```
  000b964c: ldrd r0,r1,[r0,#0x1b8]
  000b9650: bx lr
```
