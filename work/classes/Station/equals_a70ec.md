# Station::equals
elf 0xa70ec body 20
Sig: undefined __thiscall equals(Station * this, Station * param_1)

## decompile
```c

/* Station::equals(Station*) */

bool __thiscall Station::equals(Station *this,Station *param_1)

{
  if (param_1 != (Station *)0x0) {
    return *(int *)(this + 0xc) == *(int *)(param_1 + 0xc);
  }
  return false;
}

```

## target disasm
```
  000b70ec: cbz r1,0x000b70fc
  000b70ee: ldr r1,[r1,#0xc]
  000b70f0: ldr r0,[r0,#0xc]
  000b70f2: subs r0,r0,r1
  000b70f4: clz r0,r0
  000b70f8: lsrs r0,r0,#0x5
  000b70fa: bx lr
  000b70fc: movs r0,#0x0
  000b70fe: bx lr
```
