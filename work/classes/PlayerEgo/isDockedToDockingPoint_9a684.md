# PlayerEgo::isDockedToDockingPoint
elf 0x9a684 body 26
Sig: undefined __stdcall isDockedToDockingPoint(void)

## decompile
```c

/* PlayerEgo::isDockedToDockingPoint() */

void PlayerEgo::isDockedToDockingPoint(void)

{
  return;
}

```

## target disasm
```
  000aa684: ldrb.w r1,[r0,#0x356]
  000aa688: cmp r1,#0x0
  000aa68a: iteee eq
  000aa68c: mov.eq r0,#0x0
  000aa68e: ldr.ne.w r0,[r0,#0x1c4]
  000aa692: sub.ne r0,#0x1
  000aa694: clz.ne r0,r0
  000aa698: it ne
  000aa69a: lsr.ne r0,r0,#0x5
  000aa69c: bx lr
```
