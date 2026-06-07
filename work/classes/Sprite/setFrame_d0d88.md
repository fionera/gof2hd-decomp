# Sprite::setFrame
elf 0xd0d88 body 72
Sig: undefined __thiscall setFrame(Sprite * this, int param_1)

## decompile
```c

/* Sprite::setFrame(int) */

void __thiscall Sprite::setFrame(Sprite *this,int param_1)

{
  int iVar1;
  int extraout_r1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (param_1 < 0) {
    param_1 = -param_1;
  }
  *(int *)(this + 0x38) = param_1;
  if (*(int *)(this + 0x3c) <= param_1) {
    __aeabi_idivmod(param_1);
    *(int *)(this + 0x38) = extraout_r1;
    param_1 = extraout_r1;
  }
  iVar3 = *(int *)(this + 0x18);
  iVar4 = *(int *)(this + 0x1c);
  iVar2 = *(int *)(this + 0x30);
  iVar1 = __aeabi_idiv(param_1,iVar2);
  *(int *)(this + 0x28) = iVar3 * (param_1 - iVar1 * iVar2);
  *(int *)(this + 0x2c) = iVar4 * iVar1;
  return;
}

```

## target disasm
```
  000e0d88: push {r4,r5,r6,r7,lr}
  000e0d8a: add r7,sp,#0xc
  000e0d8c: push {r8,r9,r11}
  000e0d90: mov r5,r1
  000e0d92: ldr r1,[r0,#0x3c]
  000e0d94: mov r4,r0
  000e0d96: cmp r5,#0x0
  000e0d98: bpl 0x000e0d9c
  000e0d9a: rsbs r5,r5
  000e0d9c: cmp r5,r1
  000e0d9e: str r5,[r4,#0x38]
  000e0da0: blt 0x000e0dac
  000e0da2: mov r0,r5
  000e0da4: blx 0x0006f514
  000e0da8: mov r5,r1
  000e0daa: str r1,[r4,#0x38]
  000e0dac: ldrd r8,r9,[r4,#0x18]
  000e0db0: mov r0,r5
  000e0db2: ldr r6,[r4,#0x30]
  000e0db4: mov r1,r6
  000e0db6: blx 0x0007198c
  000e0dba: mls r1,r0,r6,r5
  000e0dbe: mul r0,r9,r0
  000e0dc2: mul r1,r8,r1
  000e0dc6: strd r1,r0,[r4,#0x28]
  000e0dca: pop.w {r8,r9,r11}
  000e0dce: pop {r4,r5,r6,r7,pc}
```
