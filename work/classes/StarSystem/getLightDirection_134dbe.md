# StarSystem::getLightDirection
elf 0x134dbe body 14
Sig: undefined __stdcall getLightDirection(void)

## decompile
```c

/* StarSystem::getLightDirection() */

void StarSystem::getLightDirection(void)

{
  undefined8 *in_r0;
  int in_r1;
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(in_r1 + 0x30);
  *(undefined4 *)(in_r0 + 1) = *(undefined4 *)(in_r1 + 0x38);
  *in_r0 = uVar1;
  return;
}

```

## target disasm
```
  00144dbe: vldr.64 d16,[r1,#0x30]
  00144dc2: ldr r1,[r1,#0x38]
  00144dc4: str r1,[r0,#0x8]
  00144dc6: vstr.64 d16,[r0]
  00144dca: bx lr
```
