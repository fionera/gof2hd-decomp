# Station::clone
elf 0xa7078 body 88
Sig: int __thiscall clone(Station * this, __fn * __fn, void * __child_stack, int __flags, void * __arg, ...)

## decompile
```c

/* Station::clone() */

int __thiscall
Station::clone(Station *this,__fn *__fn,void *__child_stack,int __flags,void *__arg,...)

{
  Station *pSVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000b70e8 + 0xb708a);
  local_1c = *piVar2;
  pSVar1 = operator_new(0x34);
  AbyssEngine::String::String(aSStack_28,this,false);
  Station(pSVar1,aSStack_28,*(undefined4 *)(this + 0xc),*(undefined4 *)(this + 0x10),
          *(undefined4 *)(this + 0x20),*(undefined4 *)(this + 0x18));
  AbyssEngine::String::~String(aSStack_28);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return (int)pSVar1;
}

```

## target disasm
```
  000b7078: push {r4,r5,r6,r7,lr}
  000b707a: add r7,sp,#0xc
  000b707c: push.w r11
  000b7080: sub sp,#0x18
  000b7082: mov r5,r0
  000b7084: ldr r0,[0x000b70e8]
  000b7086: add r0,pc
  000b7088: ldr r6,[r0,#0x0]
  000b708a: ldr r0,[r6,#0x0]
  000b708c: str r0,[sp,#0x14]
  000b708e: movs r0,#0x34
  000b7090: blx 0x0006eb24
  000b7094: mov r4,r0
  000b7096: add r0,sp,#0x8
  000b7098: mov r1,r5
  000b709a: movs r2,#0x0
  000b709c: blx 0x0006f028
  000b70a0: ldrd r2,r3,[r5,#0xc]
  000b70a4: ldr r0,[r5,#0x18]
  000b70a6: ldr r1,[r5,#0x20]
  000b70a8: strd r1,r0,[sp,#0x0]
  000b70ac: add r1,sp,#0x8
  000b70ae: mov r0,r4
  000b70b0: blx 0x000734f8
  000b70b4: add r0,sp,#0x8
  000b70b6: blx 0x0006ee30
  000b70ba: ldr r0,[sp,#0x14]
  000b70bc: ldr r1,[r6,#0x0]
  000b70be: subs r0,r1,r0
  000b70c0: itttt eq
  000b70c2: mov.eq r0,r4
  000b70c4: add.eq sp,#0x18
  000b70c6: pop.eq.w r11
  000b70ca: pop.eq {r4,r5,r6,r7,pc}
  000b70cc: blx 0x0006e824
```
