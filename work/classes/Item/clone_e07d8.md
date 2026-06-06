# Item::clone
elf 0xe07d8 body 56
Sig: int __thiscall clone(Item * this, __fn * __fn, void * __child_stack, int __flags, void * __arg, ...)

## decompile
```c

/* Item::clone() */

int __thiscall Item::clone(Item *this,__fn *__fn,void *__child_stack,int __flags,void *__arg,...)

{
  EVP_PKEY_CTX *ctx;
  undefined4 uVar1;
  undefined4 uVar2;
  
  ctx = operator_new(0x48);
  uVar1 = *(undefined4 *)(this + 0x2c);
  uVar2 = *(undefined4 *)(this + 0x30);
  *(undefined4 *)(ctx + 0x28) = *(undefined4 *)(this + 0x28);
  *(undefined4 *)(ctx + 0x2c) = uVar1;
  *(undefined4 *)(ctx + 0x30) = uVar2;
  init(ctx);
  *(undefined4 *)(ctx + 0x18) = *(undefined4 *)(this + 0x18);
  *(Item *)(ctx + 0x44) = this[0x44];
  uVar1 = *(undefined4 *)(this + 0x38);
  *(undefined4 *)(ctx + 0x34) = *(undefined4 *)(this + 0x34);
  *(undefined4 *)(ctx + 0x38) = uVar1;
  return (int)ctx;
}

```
## target disasm
```
  000f07d8: push {r4,r5,r7,lr}
  000f07da: add r7,sp,#0x8
  000f07dc: mov r4,r0
  000f07de: movs r0,#0x48
  000f07e0: blx 0x0006eb24
  000f07e4: add.w r2,r4,#0x28
  000f07e8: mov r5,r0
  000f07ea: add.w r3,r5,#0x28
  000f07ee: ldmia r2,{r0,r1,r2}
  000f07f0: stmia r3!,{r0,r1,r2}
  000f07f2: mov r0,r5
  000f07f4: blx 0x00075a24
  000f07f8: ldr r0,[r4,#0x18]
  000f07fa: str r0,[r5,#0x18]
  000f07fc: ldrb.w r0,[r4,#0x44]
  000f0800: strb.w r0,[r5,#0x44]
  000f0804: ldrd r0,r1,[r4,#0x34]
  000f0808: strd r0,r1,[r5,#0x34]
  000f080c: mov r0,r5
  000f080e: pop {r4,r5,r7,pc}
```
