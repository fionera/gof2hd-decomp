# AEGeometry::getRotation
elf 0xa45fa body 14
Sig: undefined __stdcall getRotation(void)

## decompile
```c

/* AEGeometry::getRotation() */

void AEGeometry::getRotation(void)

{
  undefined4 *in_r0;
  int in_r1;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(in_r1 + 0x34);
  uVar1 = *(undefined4 *)(in_r1 + 0x38);
  *in_r0 = *(undefined4 *)(in_r1 + 0x30);
  in_r0[1] = uVar2;
  in_r0[2] = uVar1;
  return;
}

```

## target disasm
```
  000b45fa: ldrd r2,r3,[r1,#0x30]
  000b45fe: ldr r1,[r1,#0x38]
  000b4600: strd r2,r3,[r0,#0x0]
  000b4604: str r1,[r0,#0x8]
  000b4606: bx lr
```
