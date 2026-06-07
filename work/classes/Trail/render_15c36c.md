# Trail::render
elf 0x15c36c body 16
Sig: undefined __stdcall render(void)

## decompile
```c

/* Trail::render() */

void Trail::render(void)

{
  int in_r0;
  
  (*(code *)(DAT_001abda4 + 0x1abda8))
            (**(undefined4 **)(DAT_0016c37c + 0x16c372),*(undefined4 *)(in_r0 + 0x14),0);
  return;
}

```

## target disasm
```
  0016c36c: ldr r1,[0x0016c37c]
  0016c36e: add r1,pc
  0016c370: ldr r2,[r1,#0x0]
  0016c372: ldr r1,[r0,#0x14]
  0016c374: ldr r0,[r2,#0x0]
  0016c376: movs r2,#0x0
  0016c378: b.w 0x001abd98
```
