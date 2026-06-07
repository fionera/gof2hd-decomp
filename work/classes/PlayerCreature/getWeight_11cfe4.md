# PlayerCreature::getWeight
elf 0x11cfe4 body 14
Sig: undefined __thiscall getWeight(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::getWeight() */

undefined4 __thiscall PlayerCreature::getWeight(PlayerCreature *this)

{
  return *(undefined4 *)(DAT_0012cff4 + 0x12cfee + *(int *)(this + 0xac) * 4);
}

```

## target disasm
```
  0012cfe4: ldr r1,[0x0012cff4]
  0012cfe6: ldr.w r0,[r0,#0xac]
  0012cfea: add r1,pc
  0012cfec: ldr.w r0,[r1,r0,lsl #0x2]
  0012cff0: bx lr
```
