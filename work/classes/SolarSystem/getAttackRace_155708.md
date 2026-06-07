# SolarSystem::getAttackRace
elf 0x155708 body 20
Sig: undefined __thiscall getAttackRace(SolarSystem * this)

## decompile
```c

/* SolarSystem::getAttackRace() */

undefined4 __thiscall SolarSystem::getAttackRace(SolarSystem *this)

{
  if (*(uint *)(this + 0x20) < 4) {
    return *(undefined4 *)(DAT_0016571c + 0x165718 + *(uint *)(this + 0x20) * 4);
  }
  return 8;
}

```

## target disasm
```
  00165708: ldr r0,[r0,#0x20]
  0016570a: cmp r0,#0x3
  0016570c: itt hi
  0016570e: mov.hi r0,#0x8
  00165710: bx.hi lr
  00165712: ldr r1,[0x0016571c]
  00165714: add r1,pc
  00165716: ldr.w r0,[r1,r0,lsl #0x2]
  0016571a: bx lr
```
