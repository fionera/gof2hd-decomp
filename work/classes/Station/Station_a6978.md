# Station::Station
elf 0xa6978 body 92
Sig: undefined __thiscall Station(Station * this)

## decompile
```c

/* Station::Station() */

void __thiscall Station::Station(Station *this)

{
  int iVar1;
  int *piVar2;
  Station *apSStack_20 [3];
  int local_14;
  
  piVar2 = *(int **)(DAT_000b69ec + 0xb6984);
  local_14 = *piVar2;
  apSStack_20[0] = this;
  AbyssEngine::String::String((String *)this);
  AbyssEngine::String::String((String *)apSStack_20,(char *)(DAT_000b69f0 + 0xb6994),false);
  AbyssEngine::String::operator=((String *)this,(String *)apSStack_20);
  AbyssEngine::String::~String((String *)apSStack_20);
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  this[0x1c] = (Station)0x0;
  this[0x24] = (Station)0x0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  iVar1 = *piVar2 - local_14;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000b6978: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000b697a: add r7,sp,#0x18
  000b697c: mov r4,r0
  000b697e: ldr r0,[0x000b69ec]
  000b6980: add r0,pc
  000b6982: ldr r5,[r0,#0x0]
  000b6984: ldr r0,[r5,#0x0]
  000b6986: str r0,[sp,#0xc]
  000b6988: mov r0,r4
  000b698a: blx 0x0006efbc
  000b698e: ldr r1,[0x000b69f0]
  000b6990: add r1,pc
  000b6992: mov r0,sp
  000b6994: movs r2,#0x0
  000b6996: blx 0x0006ee18
  000b699a: mov r1,sp
  000b699c: mov r0,r4
  000b699e: blx 0x0006f2b0
  000b69a2: mov r0,sp
  000b69a4: blx 0x0006ee30
  000b69a8: mov.w r0,#0xffffffff
  000b69ac: strd r0,r0,[r4,#0xc]
  000b69b0: movs r0,#0x0
  000b69b2: str r0,[r4,#0x20]
  000b69b4: str r0,[r4,#0x18]
  000b69b6: strb r0,[r4,#0x1c]
  000b69b8: strb.w r0,[r4,#0x24]
  000b69bc: strd r0,r0,[r4,#0x28]
  000b69c0: str r0,[r4,#0x30]
  000b69c2: ldr r0,[sp,#0xc]
  000b69c4: ldr r1,[r5,#0x0]
  000b69c6: subs r0,r1,r0
  000b69c8: ittt eq
  000b69ca: mov.eq r0,r4
  000b69cc: add.eq sp,#0x10
  000b69ce: pop.eq {r4,r5,r7,pc}
  000b69d0: blx 0x0006e824
```
