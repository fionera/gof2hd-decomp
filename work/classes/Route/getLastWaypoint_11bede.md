# Route::getLastWaypoint
elf 0x11bede body 12
Sig: undefined __thiscall getLastWaypoint(Route * this)

## decompile
```c

/* Route::getLastWaypoint() */

void __thiscall Route::getLastWaypoint(Route *this)

{
  (*(code *)(DAT_001ac334 + 0x1ac338))(this,**(int **)(this + 0xc) + -1);
  return;
}

```

## target disasm
```
  0012bede: ldr r1,[r0,#0xc]
  0012bee0: ldr r1,[r1,#0x0]
  0012bee2: subs r1,#0x1
  0012bee4: b.w 0x001ac328
  001ac328: bx pc
```
