# Radar::getTurretScopeWidth
elf 0x13104e body 8
Sig: undefined __thiscall getTurretScopeWidth(Radar * this)

## decompile
```c

/* Radar::getTurretScopeWidth() */

int __thiscall Radar::getTurretScopeWidth(Radar *this)

{
  return *(int *)(this + 300) << 1;
}

```

## target disasm
```
  0014104e: ldr.w r0,[r0,#0x12c]
  00141052: lsls r0,r0,#0x1
  00141054: bx lr
```
