# ListItem::ListItem
elf 0xa645a body 144
Sig: undefined __thiscall ListItem(ListItem * this, ListItem * param_1)

## decompile
```c

/* ListItem::ListItem(ListItem*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,ListItem *param_1)

{
  String *pSVar1;
  String *pSVar2;
  undefined8 uVar3;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  uVar3 = *(undefined8 *)(param_1 + 0xc);
  *(undefined8 *)(this + 4) = *(undefined8 *)(param_1 + 4);
  *(undefined8 *)(this + 0xc) = uVar3;
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x14);
  this[0x24] = param_1[0x24];
  pSVar2 = *(String **)(param_1 + 0x1c);
  if (pSVar2 == (String *)0x0) {
    *(undefined4 *)(this + 0x1c) = 0;
  }
  else {
    pSVar1 = operator_new(0xc);
    AbyssEngine::String::String(pSVar1,pSVar2,false);
    *(String **)(this + 0x1c) = pSVar1;
  }
  pSVar2 = *(String **)(param_1 + 0x20);
  if (pSVar2 == (String *)0x0) {
    *(undefined4 *)(this + 0x20) = 0;
  }
  else {
    pSVar1 = operator_new(0xc);
    AbyssEngine::String::String(pSVar1,pSVar2,false);
    *(String **)(this + 0x20) = pSVar1;
  }
  uVar3 = *(undefined8 *)(param_1 + 0x30);
  *(undefined8 *)(this + 0x28) = *(undefined8 *)(param_1 + 0x28);
  *(undefined8 *)(this + 0x30) = uVar3;
  this[0x38] = param_1[0x38];
  *(undefined4 *)(this + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(this + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x18);
  return this;
}

```

## target disasm
```
  000b645a: push {r4,r5,r6,r7,lr}
  000b645c: add r7,sp,#0xc
  000b645e: push.w r8
  000b6462: mov r5,r1
  000b6464: mov r4,r0
  000b6466: blx 0x000733cc
  000b646a: adds r0,r5,#0x4
  000b646c: vld1.32 {d16,d17},[r0]
  000b6470: adds r0,r4,#0x4
  000b6472: vst1.32 {d16,d17},[r0]
  000b6476: ldr r0,[r5,#0x14]
  000b6478: str r0,[r4,#0x14]
  000b647a: ldrb.w r0,[r5,#0x24]
  000b647e: strb.w r0,[r4,#0x24]
  000b6482: ldr r6,[r5,#0x1c]
  000b6484: cbz r6,0x000b649c
  000b6486: movs r0,#0xc
  000b6488: blx 0x0006eb24
  000b648c: mov r8,r0
  000b648e: mov r1,r6
  000b6490: movs r2,#0x0
  000b6492: blx 0x0006f028
  000b6496: str.w r8,[r4,#0x1c]
  000b649a: b 0x000b64a0
  000b649c: movs r0,#0x0
  000b649e: str r0,[r4,#0x1c]
  000b64a0: ldr r6,[r5,#0x20]
  000b64a2: cbz r6,0x000b64ba
  000b64a4: movs r0,#0xc
  000b64a6: blx 0x0006eb24
  000b64aa: mov r8,r0
  000b64ac: mov r1,r6
  000b64ae: movs r2,#0x0
  000b64b0: blx 0x0006f028
  000b64b4: str.w r8,[r4,#0x20]
  000b64b8: b 0x000b64be
  000b64ba: movs r0,#0x0
  000b64bc: str r0,[r4,#0x20]
  000b64be: add.w r0,r5,#0x28
  000b64c2: vld1.32 {d16,d17},[r0]
  000b64c6: add.w r0,r4,#0x28
  000b64ca: vst1.32 {d16,d17},[r0]
  000b64ce: ldrb.w r0,[r5,#0x38]
  000b64d2: strb.w r0,[r4,#0x38]
  000b64d6: ldr r0,[r5,#0x3c]
  000b64d8: str r0,[r4,#0x3c]
  000b64da: ldr r0,[r5,#0x40]
  000b64dc: str r0,[r4,#0x40]
  000b64de: ldr r0,[r5,#0x18]
  000b64e0: str r0,[r4,#0x18]
  000b64e2: mov r0,r4
  000b64e4: pop.w r8
  000b64e8: pop {r4,r5,r6,r7,pc}
```
