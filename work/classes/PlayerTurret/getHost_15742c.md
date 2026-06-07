# PlayerTurret::getHost
elf 0x15742c body 6
Sig: undefined __thiscall getHost(PlayerTurret * this)

## decompile
```c

/* PlayerTurret::getHost() */

undefined4 __thiscall PlayerTurret::getHost(PlayerTurret *this)

{
  return *(undefined4 *)(this + 0x154);
}

```

## target disasm
```
  0016742c: ldr.w r0,[r0,#0x154]
  00167430: bx lr
```
