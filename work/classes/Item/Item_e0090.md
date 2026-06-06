# Item::Item
elf 0xe0090 body 22
Sig: undefined __thiscall Item(Item * this, Array * param_1, Array * param_2, Array * param_3)

## decompile
```c

/* Item::Item(Array<int>*, Array<int>*, Array<int>*) */

Item * __thiscall Item::Item(Item *this,Array *param_1,Array *param_2,Array *param_3)

{
  *(Array **)(this + 0x28) = param_1;
  *(Array **)(this + 0x2c) = param_2;
  *(Array **)(this + 0x30) = param_3;
  init((EVP_PKEY_CTX *)this);
  return this;
}

```
## target disasm
```
  000f0090: push {r4,r6,r7,lr}
  000f0092: add r7,sp,#0x8
  000f0094: add.w r12,r0,#0x28
  000f0098: mov r4,r0
  000f009a: stm.w r12!,{r1,r2,r3}
  000f009e: blx 0x00075a24
  000f00a2: mov r0,r4
  000f00a4: pop {r4,r6,r7,pc}
```
