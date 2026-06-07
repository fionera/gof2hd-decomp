# Mission::getTargetStationName
elf 0x15bb90 body 14
Sig: undefined __stdcall getTargetStationName(void)

## decompile
```c

/* Mission::getTargetStationName() */

void Mission::getTargetStationName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x40),false);
  return;
}

```

## target disasm
```
  0016bb90: push {r7,lr}
  0016bb92: mov r7,sp
  0016bb94: adds r1,#0x40
  0016bb96: movs r2,#0x0
  0016bb98: blx 0x0006f028
  0016bb9c: pop {r7,pc}
```
