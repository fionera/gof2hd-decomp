# PlayerEgo::boosting
elf 0x9af32 body 6
Sig: undefined __stdcall boosting(void)

## decompile
```c

/* PlayerEgo::boosting() */

undefined1 PlayerEgo::boosting(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x13c);
}

```

## target disasm
```
  000aaf32: ldrb.w r0,[r0,#0x13c]
  000aaf36: bx lr
```
