# Route::waypointDefined
elf 0x11bee8 body 10
Sig: undefined __thiscall waypointDefined(Route * this)

## decompile
```c

/* Route::waypointDefined() */

bool __thiscall Route::waypointDefined(Route *this)

{
  return *(int *)(this + 0xc) != 0;
}

```

## target disasm
```
  0012bee8: ldr r0,[r0,#0xc]
  0012beea: cmp r0,#0x0
  0012beec: it ne
  0012beee: mov.ne r0,#0x1
  0012bef0: bx lr
```
