# Mission::isInstantActionMission
elf 0x15bb78 body 6
Sig: undefined __stdcall isInstantActionMission(void)

## decompile
```c

/* Mission::isInstantActionMission() */

undefined1 Mission::isInstantActionMission(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x5c);
}

```

## target disasm
```
  0016bb78: ldrb.w r0,[r0,#0x5c]
  0016bb7c: bx lr
```
