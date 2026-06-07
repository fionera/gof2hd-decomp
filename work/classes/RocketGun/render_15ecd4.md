# RocketGun::render
elf 0x15ecd4 body 14
Sig: undefined __stdcall render(void)

## decompile
```c

/* RocketGun::render() */

void RocketGun::render(void)

{
  (*(code *)(DAT_001ac614 + 0x1ac618))();
  return;
}

```

## target disasm
```
  0016ecd4: b.w 0x001ac608
  001ac608: bx pc
  001ac60c: ldr r12,[0x1ac614]
  001ac610: add pc,r12,pc
```
