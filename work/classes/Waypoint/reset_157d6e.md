# Waypoint::reset
elf 0x157d6e body 26
Sig: undefined __thiscall reset(Waypoint * this)

## decompile
```c

/* Waypoint::reset() */

void __thiscall Waypoint::reset(Waypoint *this)

{
  this[0x130] = (Waypoint)0x0;
  (*(code *)(DAT_001abad4 + 0x1abad8))(*(undefined4 *)(this + 4),0);
  return;
}

```

## target disasm
```
  00167d6e: ldr r1,[r0,#0x4]
  00167d70: movs r2,#0x0
  00167d72: strb.w r2,[r0,#0x130]
  00167d76: mov r0,r1
  00167d78: movs r1,#0x0
  00167d7a: b.w 0x001abac8
  001abac8: bx pc
  001abacc: ldr r12,[0x1abad4]
  001abad0: add pc,r12,pc
```
