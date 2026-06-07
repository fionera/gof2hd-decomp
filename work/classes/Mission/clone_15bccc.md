# Mission::clone
elf 0x15bccc body 110
Sig: int __thiscall clone(Mission * this, __fn * __fn, void * __child_stack, int __flags, void * __arg, ...)

## decompile
```c

/* Mission::clone() */

int __thiscall
Mission::clone(Mission *this,__fn *__fn,void *__child_stack,int __flags,void *__arg,...)

{
  int iVar1;
  Mission *pMVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_0016bd54 + 0x16bcdc);
  iVar1 = *piVar4;
  pMVar2 = operator_new(0x78);
  uVar3 = *(undefined4 *)(this + 0xc);
  AbyssEngine::String::String((String *)&stack0xffffffd0,this + 0x10,false);
  Mission(pMVar2,uVar3,&stack0xffffffd0,*(undefined4 *)(this + 0x28),*(undefined4 *)(this + 0x2c),
          *(undefined4 *)(this + 0x30),*(undefined4 *)(this + 0x3c),*(undefined4 *)(this + 0x58));
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  pMVar2[0x5c] = this[0x5c];
  if (*piVar4 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return (int)pMVar2;
}

```

## target disasm
```
  0016bccc: push {r4,r5,r6,r7,lr}
  0016bcce: add r7,sp,#0xc
  0016bcd0: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0016bcd4: mov r5,r0
  0016bcd6: ldr r0,[0x0016bd54]
  0016bcd8: add r0,pc
  0016bcda: ldr.w r9,[r0,#0x0]
  0016bcde: ldr.w r0,[r9,#0x0]
  0016bce2: str r0,[sp,#0x1c]
  0016bce4: movs r0,#0x78
  0016bce6: blx 0x0006eb24
  0016bcea: ldr.w r8,[r5,#0xc]
  0016bcee: mov r4,r0
  0016bcf0: add.w r1,r5,#0x10
  0016bcf4: add r0,sp,#0x10
  0016bcf6: movs r2,#0x0
  0016bcf8: blx 0x0006f028
  0016bcfc: ldrd r3,r0,[r5,#0x28]
  0016bd00: ldr r1,[r5,#0x30]
  0016bd02: ldr r2,[r5,#0x3c]
  0016bd04: ldr r6,[r5,#0x58]
  0016bd06: stm sp,{r0,r1,r2,r6}
  0016bd0a: add r2,sp,#0x10
  0016bd0c: mov r0,r4
  0016bd0e: mov r1,r8
  0016bd10: blx 0x000777b8
  0016bd14: add r0,sp,#0x10
  0016bd16: blx 0x0006ee30
  0016bd1a: ldrb.w r0,[r5,#0x5c]
  0016bd1e: strb.w r0,[r4,#0x5c]
  0016bd22: ldr r0,[sp,#0x1c]
  0016bd24: ldr.w r1,[r9,#0x0]
  0016bd28: subs r0,r1,r0
  0016bd2a: itttt eq
  0016bd2c: mov.eq r0,r4
  0016bd2e: add.eq sp,#0x20
  0016bd30: pop.eq.w {r8,r9,r11}
  0016bd34: pop.eq {r4,r5,r6,r7,pc}
  0016bd36: blx 0x0006e824
```
