# Status::setWingmen
elf 0xac7be body 108
Sig: undefined __thiscall setWingmen(Status * this, Array * param_1)

## decompile
```c

/* Status::setWingmen(Array<AbyssEngine::String*>*) */

void __thiscall Status::setWingmen(Status *this,Array *param_1)

{
  Array *pAVar1;
  String *this_00;
  uint uVar2;
  
  if (*(Array **)(this + 0x24) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x24));
  }
  if (param_1 == (Array *)0x0) {
    *(undefined4 *)(this + 0x24) = 0;
    *(undefined4 *)(this + 0x28) = 0;
    *(undefined4 *)(this + 0x30) = 0;
  }
  else {
    pAVar1 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    *(Array **)(this + 0x24) = pAVar1;
    ArraySetLength<AbyssEngine::String*>(*(uint *)param_1,pAVar1);
    for (uVar2 = 0; uVar2 < *(uint *)param_1; uVar2 = uVar2 + 1) {
      this_00 = operator_new(0xc);
      AbyssEngine::String::String(this_00,*(String **)(*(int *)(param_1 + 4) + uVar2 * 4),false);
      *(String **)(*(int *)(*(int *)(this + 0x24) + 4) + uVar2 * 4) = this_00;
    }
  }
  return;
}

```
## target disasm
```
  000bc7be: push {r4,r5,r6,r7,lr}
  000bc7c0: add r7,sp,#0xc
  000bc7c2: push.w r8
  000bc7c6: mov r8,r0
  000bc7c8: ldr r0,[r0,#0x24]
  000bc7ca: mov r5,r1
  000bc7cc: cbz r0,0x000bc7d2
  000bc7ce: blx 0x0006facc
  000bc7d2: cbz r5,0x000bc81a
  000bc7d4: movs r0,#0xc
  000bc7d6: blx 0x0006eb24
  000bc7da: mov r6,r0
  000bc7dc: blx 0x0006f628
  000bc7e0: str.w r6,[r8,#0x24]
  000bc7e4: mov r1,r6
  000bc7e6: ldr r0,[r5,#0x0]
  000bc7e8: blx 0x0006fe08
  000bc7ec: movs r4,#0x0
  000bc7ee: b 0x000bc812
  000bc7f0: movs r0,#0xc
  000bc7f2: blx 0x0006eb24
  000bc7f6: mov r6,r0
  000bc7f8: ldr r0,[r5,#0x4]
  000bc7fa: ldr.w r1,[r0,r4,lsl #0x2]
  000bc7fe: mov r0,r6
  000bc800: movs r2,#0x0
  000bc802: blx 0x0006f028
  000bc806: ldr.w r0,[r8,#0x24]
  000bc80a: ldr r0,[r0,#0x4]
  000bc80c: str.w r6,[r0,r4,lsl #0x2]
  000bc810: adds r4,#0x1
  000bc812: ldr r0,[r5,#0x0]
  000bc814: cmp r4,r0
  000bc816: bcc 0x000bc7f0
  000bc818: b 0x000bc824
  000bc81a: movs r0,#0x0
  000bc81c: strd r0,r0,[r8,#0x24]
  000bc820: str.w r0,[r8,#0x30]
  000bc824: pop.w r8
  000bc828: pop {r4,r5,r6,r7,pc}
```
