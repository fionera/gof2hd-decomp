# Station::getName
elf 0xa6bce body 12
Sig: undefined __stdcall getName(void)

## decompile
```c

/* Station::getName() */

void Station::getName(void)

{
  String *in_r0;
  String *in_r1;
  
  AbyssEngine::String::String(in_r0,in_r1,false);
  return;
}

```

## target disasm
```
  000b6bce: push {r7,lr}
  000b6bd0: mov r7,sp
  000b6bd2: movs r2,#0x0
  000b6bd4: blx 0x0006f028
  000b6bd8: pop {r7,pc}
```
