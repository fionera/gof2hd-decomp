# Radio::isShowingMessage
elf 0x1555d0 body 10
Sig: undefined __stdcall isShowingMessage(void)

## decompile
```c

/* Radio::isShowingMessage() */

bool Radio::isShowingMessage(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 4) != 0;
}

```

## target disasm
```
  001655d0: ldr r0,[r0,#0x4]
  001655d2: cmp r0,#0x0
  001655d4: it ne
  001655d6: mov.ne r0,#0x1
  001655d8: bx lr
```
