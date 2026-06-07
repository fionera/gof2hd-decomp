# IParticleSystem::update
elf 0x181d54 body 66
Sig: undefined __thiscall update(IParticleSystem * this, int param_1)

## decompile
```c

/* IParticleSystem::update(int) */

void __thiscall IParticleSystem::update(IParticleSystem *this,int param_1)

{
  int iVar1;
  uint in_fpscr;
  undefined4 uVar2;
  
  if (this[0xe] != (IParticleSystem)0x0) {
    iVar1 = 0;
    uVar2 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    for (; iVar1 < *(int *)(this + 0x48); iVar1 = iVar1 + 1) {
      if (*(int *)(*(int *)(this + 0x68) + iVar1 * 4) != -1) {
        (**(code **)(*(int *)this + 0x14))(this,iVar1,uVar2);
      }
    }
  }
  return;
}

```

## target disasm
```
  00191d54: push {r4,r5,r6,r7,lr}
  00191d56: add r7,sp,#0xc
  00191d58: push.w r11
  00191d5c: mov r4,r0
  00191d5e: ldrb r0,[r0,#0xe]
  00191d60: cbz r0,0x00191d90
  00191d62: vmov s0,r1
  00191d66: movs r5,#0x0
  00191d68: vcvt.f32.s32 s0,s0
  00191d6c: vmov r6,s0
  00191d70: b 0x00191d8a
  00191d72: ldr r0,[r4,#0x68]
  00191d74: ldr.w r0,[r0,r5,lsl #0x2]
  00191d78: adds r0,#0x1
  00191d7a: beq 0x00191d88
  00191d7c: ldr r0,[r4,#0x0]
  00191d7e: mov r1,r5
  00191d80: mov r2,r6
  00191d82: ldr r3,[r0,#0x14]
  00191d84: mov r0,r4
  00191d86: blx r3
  00191d88: adds r5,#0x1
  00191d8a: ldr r0,[r4,#0x48]
  00191d8c: cmp r5,r0
  00191d8e: blt 0x00191d72
  00191d90: pop.w r11
  00191d94: pop {r4,r5,r6,r7,pc}
```
