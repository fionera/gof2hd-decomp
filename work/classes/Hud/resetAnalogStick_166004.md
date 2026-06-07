# Hud::resetAnalogStick
elf 0x166004 body 10
Sig: undefined __stdcall resetAnalogStick(void)

## decompile
```c

/* Hud::resetAnalogStick() */

void Hud::resetAnalogStick(void)

{
  int in_r0;
  
  *(undefined4 *)(in_r0 + 0x41e) = *(undefined4 *)(in_r0 + 0x424);
  return;
}

```

## target disasm
```
  00176004: ldr.w r1,[r0,#0x424]
  00176008: str.w r1,[r0,#0x41e]
  0017600c: bx lr
```
