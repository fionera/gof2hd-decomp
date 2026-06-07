# Station::visit
elf 0xa6c2c body 50
Sig: undefined __stdcall visit(void)

## decompile
```c

/* Station::visit() */

void Station::visit(void)

{
  int iVar1;
  Station *in_r0;
  int iVar2;
  
  iVar2 = isDiscovered(in_r0);
  iVar1 = DAT_000b6c5c;
  if (iVar2 == 0) {
    in_r0[0x1c] = (Station)0x1;
    Status::visitStation((Status *)**(undefined4 **)(iVar1 + 0xb6c44));
    (*(code *)(DAT_001abea4 + 0x1abea8))
              (**(undefined4 **)(DAT_000b6c60 + 0xb6c52),*(undefined4 *)(in_r0 + 0xc));
    return;
  }
  return;
}

```

## target disasm
```
  000b6c2c: push {r4,r6,r7,lr}
  000b6c2e: add r7,sp,#0x8
  000b6c30: mov r4,r0
  000b6c32: blx 0x0007348c
  000b6c36: cbz r0,0x000b6c3a
  000b6c38: pop {r4,r6,r7,pc}
  000b6c3a: ldr r0,[0x000b6c5c]
  000b6c3c: movs r1,#0x1
  000b6c3e: strb r1,[r4,#0x1c]
  000b6c40: add r0,pc
  000b6c42: ldr r0,[r0,#0x0]
  000b6c44: ldr r0,[r0,#0x0]
  000b6c46: blx 0x00073498
  000b6c4a: ldr r0,[0x000b6c60]
  000b6c4c: ldr r1,[r4,#0xc]
  000b6c4e: add r0,pc
  000b6c50: ldr r0,[r0,#0x0]
  000b6c52: ldr r0,[r0,#0x0]
  000b6c54: pop.w {r4,r6,r7,lr}
  000b6c58: b.w 0x001abe98
  001abe98: bx pc
```
