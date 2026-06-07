# PlayerEgo::goingToWaypoint
elf 0x9b8f6 body 6
Sig: undefined __stdcall goingToWaypoint(void)

## decompile
```c

/* PlayerEgo::goingToWaypoint() */

undefined1 PlayerEgo::goingToWaypoint(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x160);
}

```

## target disasm
```
  000ab8f6: ldrb.w r0,[r0,#0x160]
  000ab8fa: bx lr
```
