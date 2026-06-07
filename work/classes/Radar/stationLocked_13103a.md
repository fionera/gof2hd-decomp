# Radar::stationLocked
elf 0x13103a body 20
Sig: undefined __thiscall stationLocked(Radar * this)

## decompile
```c

/* Radar::stationLocked() */

bool __thiscall Radar::stationLocked(Radar *this)

{
  if (*(int *)(this + 0x24) != 0) {
    return *(char *)(*(int *)(this + 0x24) + 0x71) != '\0';
  }
  return false;
}

```

## target disasm
```
  0014103a: ldr r0,[r0,#0x24]
  0014103c: cbz r0,0x0014104a
  0014103e: ldrb.w r0,[r0,#0x71]
  00141042: cmp r0,#0x0
  00141044: it ne
  00141046: mov.ne r0,#0x1
  00141048: bx lr
  0014104a: movs r0,#0x0
  0014104c: bx lr
```
