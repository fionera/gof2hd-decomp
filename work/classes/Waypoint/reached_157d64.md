# Waypoint::reached
elf 0x157d64 body 10
Sig: undefined __thiscall reached(Waypoint * this)

## decompile
```c

/* Waypoint::reached() */

void __thiscall Waypoint::reached(Waypoint *this)

{
  *(undefined2 *)(this + 0x130) = 0x101;
  return;
}

```

## target disasm
```
  00167d64: movw r1,#0x101
  00167d68: strh.w r1,[r0,#0x130]
  00167d6c: bx lr
```
