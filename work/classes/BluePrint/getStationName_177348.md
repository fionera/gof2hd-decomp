# BluePrint::getStationName
elf 0x177348 body 14
Sig: undefined __stdcall getStationName(void)

## decompile
```c

/* BluePrint::getStationName() */

void BluePrint::getStationName(void)

{
  String *in_r0;
  int in_r1;
  
  AbyssEngine::String::String(in_r0,(String *)(in_r1 + 0x14),false);
  return;
}

```

## target disasm
```
  00187348: push {r7,lr}
  0018734a: mov r7,sp
  0018734c: adds r1,#0x14
  0018734e: movs r2,#0x0
  00187350: blx 0x0006f028
  00187354: pop {r7,pc}
```
