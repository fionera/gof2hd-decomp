# Station::hasAttackedFriends
elf 0xa6be4 body 6
Sig: undefined __stdcall hasAttackedFriends(void)

## decompile
```c

/* Station::hasAttackedFriends() */

undefined1 Station::hasAttackedFriends(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x24);
}

```

## target disasm
```
  000b6be4: ldrb.w r0,[r0,#0x24]
  000b6be8: bx lr
```
