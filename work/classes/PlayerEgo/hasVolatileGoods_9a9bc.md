# PlayerEgo::hasVolatileGoods
elf 0x9a9bc body 6
Sig: undefined __stdcall hasVolatileGoods(void)

## decompile
```c

/* PlayerEgo::hasVolatileGoods() */

undefined1 PlayerEgo::hasVolatileGoods(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x398);
}

```

## target disasm
```
  000aa9bc: ldrb.w r0,[r0,#0x398]
  000aa9c0: bx lr
```
