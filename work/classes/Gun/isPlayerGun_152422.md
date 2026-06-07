# Gun::isPlayerGun
elf 0x152422 body 6
Sig: undefined __stdcall isPlayerGun(void)

## decompile
```c

/* Gun::isPlayerGun() */

undefined1 Gun::isPlayerGun(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xf0);
}

```

## target disasm
```
  00162422: ldrb.w r0,[r0,#0xf0]
  00162426: bx lr
```
