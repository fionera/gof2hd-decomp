# AEGeometry::getScaling
elf 0xa4608 body 14
Sig: undefined __stdcall getScaling(void)

## decompile
```c

/* AEGeometry::getScaling() */

void AEGeometry::getScaling(void)

{
  undefined4 *in_r0;
  int in_r1;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(in_r1 + 0x40);
  uVar1 = *(undefined4 *)(in_r1 + 0x44);
  *in_r0 = *(undefined4 *)(in_r1 + 0x3c);
  in_r0[1] = uVar2;
  in_r0[2] = uVar1;
  return;
}

```

## target disasm
```
  000b4608: ldrd r2,r3,[r1,#0x3c]
  000b460c: ldr r1,[r1,#0x44]
  000b460e: strd r2,r3,[r0,#0x0]
  000b4612: str r1,[r0,#0x8]
  000b4614: bx lr
```
