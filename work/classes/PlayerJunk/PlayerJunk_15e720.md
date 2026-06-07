# PlayerJunk::PlayerJunk
elf 0x15e720 body 66
Sig: undefined __thiscall PlayerJunk(PlayerJunk * this, int param_1, Player * param_2, AEGeometry * param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float) */

void __thiscall
PlayerJunk::PlayerJunk
          (PlayerJunk *this,int param_1,Player *param_2,AEGeometry *param_3,float param_4,
          float param_5,float param_6)

{
  int iVar1;
  int *piVar2;
  float in_stack_00000000;
  float in_stack_00000004;
  
  piVar2 = (int *)KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,param_2,param_3,in_stack_00000000,
                                     param_5,in_stack_00000004,SUB41(in_stack_00000000,0));
  iVar1 = DAT_0016e764;
  *(undefined1 *)((int)piVar2 + 0x3d) = 1;
  *piVar2 = *(int *)(iVar1 + 0x16e75a) + 8;
  return;
}

```

## target disasm
```
  0016e720: push {r1,r2,r3,r4,r5,r6,r7,lr}
  0016e722: add r7,sp,#0x18
  0016e724: mov r12,r2
  0016e726: vldr.32 s0,[r7,#0x8]
  0016e72a: vldr.32 s2,[r7,#0xc]
  0016e72e: movs r2,#0x0
  0016e730: vldr.32 s4,[r7,#0x10]
  0016e734: str r2,[sp,#0x10]
  0016e736: mov.w r2,#0xffffffff
  0016e73a: str r3,[sp,#0x0]
  0016e73c: mov r3,r12
  0016e73e: vstr.32 s0,[sp,#0x4]
  0016e742: vstr.32 s2,[sp,#0x8]
  0016e746: vstr.32 s4,[sp,#0xc]
  0016e74a: blx 0x00075790
  0016e74e: ldr r1,[0x0016e764]
  0016e750: movs r2,#0x1
  0016e752: strb.w r2,[r0,#0x3d]
  0016e756: add r1,pc
  0016e758: ldr r1,[r1,#0x0]
  0016e75a: adds r1,#0x8
  0016e75c: str r1,[r0,#0x0]
  0016e75e: add sp,#0x18
  0016e760: pop {r7,pc}
```
