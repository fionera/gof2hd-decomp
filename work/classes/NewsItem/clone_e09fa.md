# NewsItem::clone
elf 0xe09fa body 74
Sig: int __thiscall clone(NewsItem * this, __fn * __fn, void * __child_stack, int __flags, void * __arg, ...)

## decompile
```c

/* NewsItem::clone() */

int __thiscall
NewsItem::clone(NewsItem *this,__fn *__fn,void *__child_stack,int __flags,void *__arg,...)

{
  void *pvVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  uint uVar7;
  
  uVar7 = *(uint *)(this + 0xc);
  pvVar1 = operator_new__(uVar7 | (int)uVar7 >> 0x1f);
  for (iVar2 = 0; iVar2 < (int)uVar7; iVar2 = iVar2 + 1) {
    *(undefined1 *)((int)pvVar1 + iVar2) = *(undefined1 *)(*(int *)(this + 8) + iVar2);
  }
  puVar3 = operator_new(0x1c);
  uVar4 = *(undefined4 *)(this + 0x10);
  uVar5 = *(undefined4 *)(this + 0x14);
  uVar6 = *(undefined4 *)this;
  *(NewsItem *)(puVar3 + 1) = this[4];
  *puVar3 = uVar6;
  puVar3[2] = pvVar1;
  puVar3[3] = uVar7;
  puVar3[4] = uVar4;
  puVar3[5] = uVar5;
  *(undefined1 *)(puVar3 + 6) = 0;
  return (int)puVar3;
}

```

## target disasm
```
  000f09fa: push {r4,r5,r6,r7,lr}
  000f09fc: add r7,sp,#0xc
  000f09fe: push.w r11
  000f0a02: ldr r6,[r0,#0xc]
  000f0a04: mov r4,r0
  000f0a06: orr.w r0,r6,r6, asr #0x1f
  000f0a0a: blx 0x0006ec08
  000f0a0e: mov r5,r0
  000f0a10: movs r0,#0x0
  000f0a12: b 0x000f0a1c
  000f0a14: ldr r1,[r4,#0x8]
  000f0a16: ldrb r1,[r1,r0]
  000f0a18: strb r1,[r5,r0]
  000f0a1a: adds r0,#0x1
  000f0a1c: cmp r0,r6
  000f0a1e: blt 0x000f0a14
  000f0a20: movs r0,#0x1c
  000f0a22: blx 0x0006eb24
  000f0a26: ldrd r1,r2,[r4,#0x10]
  000f0a2a: ldr r3,[r4,#0x0]
  000f0a2c: ldrb r4,[r4,#0x4]
  000f0a2e: strb r4,[r0,#0x4]
  000f0a30: str r3,[r0,#0x0]
  000f0a32: strd r5,r6,[r0,#0x8]
  000f0a36: strd r1,r2,[r0,#0x10]
  000f0a3a: movs r1,#0x0
  000f0a3c: strb r1,[r0,#0x18]
  000f0a3e: pop.w r11
  000f0a42: pop {r4,r5,r6,r7,pc}
```
