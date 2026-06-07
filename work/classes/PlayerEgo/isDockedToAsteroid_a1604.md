# PlayerEgo::isDockedToAsteroid
elf 0xa1604 body 26
Sig: undefined __stdcall isDockedToAsteroid(void)

## decompile
```c

/* PlayerEgo::isDockedToAsteroid() */

void PlayerEgo::isDockedToAsteroid(void)

{
  return;
}

```

## target disasm
```
  000b1604: ldrb.w r1,[r0,#0x1c0]
  000b1608: cmp r1,#0x0
  000b160a: iteee eq
  000b160c: mov.eq r0,#0x0
  000b160e: ldr.ne.w r0,[r0,#0x1c4]
  000b1612: sub.ne r0,#0x1
  000b1614: clz.ne r0,r0
  000b1618: it ne
  000b161a: lsr.ne r0,r0,#0x5
  000b161c: bx lr
```
