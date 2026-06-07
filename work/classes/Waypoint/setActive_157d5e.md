# Waypoint::setActive
elf 0x157d5e body 6
Sig: undefined __stdcall setActive(bool param_1)

## decompile
```c

/* Waypoint::setActive(bool) */

void Waypoint::setActive(bool param_1)

{
  (*(code *)(DAT_001abad4 + 0x1abad8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  00167d5e: ldr r0,[r0,#0x4]
  00167d60: b.w 0x001abac8
```
