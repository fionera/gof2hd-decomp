# Engine::ShaderSetActive
elf 0x85e60 body 178
Sig: undefined __thiscall ShaderSetActive(Engine * this, int param_1, Mesh * param_2)

## decompile
```c

/* AbyssEngine::Engine::ShaderSetActive(int, AbyssEngine::Mesh*) */

void __thiscall AbyssEngine::Engine::ShaderSetActive(Engine *this,int param_1,Mesh *param_2)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  bool bVar4;
  
  while (param_1 == -1) {
    param_1 = **(int **)(DAT_00095f14 + 0x95e74);
    if (-1 < (int)((uint)(byte)*param_2 << 0x1e)) {
      param_1 = **(int **)(DAT_00095f18 + 0x95e78);
    }
  }
  if (param_2 == (Mesh *)0x0) {
    bVar4 = false;
  }
  else {
    bVar4 = param_2[0x85] != (Mesh)0x0;
  }
  piVar1 = *(int **)(*(int *)(this + 0x514) + param_1 * 4);
  if (piVar1 != (int *)0x0) {
    **(undefined1 **)(DAT_00095f1c + 0x95eb0) = 1;
    if (piVar1[1] != *(int *)(this + 0x3e4)) {
      (**(code **)(*piVar1 + 0x28))(piVar1,bVar4);
      piVar1 = *(int **)(*(int *)(this + 0x514) + param_1 * 4);
      piVar3 = *(int **)(DAT_00095f20 + 0x95ece);
      *(int *)(this + 0x3e4) = piVar1[1];
      *piVar3 = param_1;
    }
    (**(code **)(*piVar1 + 0x28))(piVar1,bVar4);
    piVar1 = *(int **)(*(int *)(this + 0x514) + param_1 * 4);
    (**(code **)(*piVar1 + 0xc))(piVar1,param_2,this);
    if (param_2 != (Mesh *)0x0) {
      iVar2 = __aeabi_uidiv(*(undefined2 *)(param_2 + 0x28),3);
      *(int *)(*(int *)(this + 0x3dc) + param_1 * 4) =
           iVar2 + *(int *)(*(int *)(this + 0x3dc) + param_1 * 4);
    }
  }
  return;
}

```

## target disasm
```
  00095e60: push {r4,r5,r6,r7,lr}
  00095e62: add r7,sp,#0xc
  00095e64: push.w r8
  00095e68: mov r8,r0
  00095e6a: ldr r0,[0x00095f14]
  00095e6c: mov r4,r1
  00095e6e: ldr r1,[0x00095f18]
  00095e70: add r0,pc
  00095e72: mov r6,r2
  00095e74: add r1,pc
  00095e76: ldr r0,[r0,#0x0]
  00095e78: ldr r1,[r1,#0x0]
  00095e7a: ldr r0,[r0,#0x0]
  00095e7c: ldr r1,[r1,#0x0]
  00095e7e: b 0x00095e8a
  00095e80: ldrb r2,[r6,#0x0]
  00095e82: mov r4,r0
  00095e84: lsls r2,r2,#0x1e
  00095e86: it pl
  00095e88: mov.pl r4,r1
  00095e8a: adds r2,r4,#0x1
  00095e8c: beq 0x00095e80
  00095e8e: cbz r6,0x00095e9c
  00095e90: ldrb.w r5,[r6,#0x85]
  00095e94: cmp r5,#0x0
  00095e96: it ne
  00095e98: mov.ne r5,#0x1
  00095e9a: b 0x00095e9e
  00095e9c: movs r5,#0x0
  00095e9e: ldr.w r0,[r8,#0x514]
  00095ea2: ldr.w r0,[r0,r4,lsl #0x2]
  00095ea6: cbz r0,0x00095f0c
  00095ea8: ldr r1,[0x00095f1c]
  00095eaa: movs r2,#0x1
  00095eac: add r1,pc
  00095eae: ldr r1,[r1,#0x0]
  00095eb0: strb r2,[r1,#0x0]
  00095eb2: ldr.w r1,[r8,#0x3e4]
  00095eb6: ldr r2,[r0,#0x4]
  00095eb8: cmp r2,r1
  00095eba: beq 0x00095eda
  00095ebc: ldr r1,[r0,#0x0]
  00095ebe: ldr r2,[r1,#0x28]
  00095ec0: mov r1,r5
  00095ec2: blx r2
  00095ec4: ldr.w r0,[r8,#0x514]
  00095ec8: ldr r1,[0x00095f20]
  00095eca: add r1,pc
  00095ecc: ldr.w r0,[r0,r4,lsl #0x2]
  00095ed0: ldr r1,[r1,#0x0]
  00095ed2: ldr r2,[r0,#0x4]
  00095ed4: str.w r2,[r8,#0x3e4]
  00095ed8: str r4,[r1,#0x0]
  00095eda: ldr r1,[r0,#0x0]
  00095edc: ldr r2,[r1,#0x28]
  00095ede: mov r1,r5
  00095ee0: blx r2
  00095ee2: ldr.w r0,[r8,#0x514]
  00095ee6: mov r2,r8
  00095ee8: ldr.w r0,[r0,r4,lsl #0x2]
  00095eec: ldr r1,[r0,#0x0]
  00095eee: ldr r3,[r1,#0xc]
  00095ef0: mov r1,r6
  00095ef2: blx r3
  00095ef4: cbz r6,0x00095f0c
  00095ef6: ldrh r0,[r6,#0x28]
  00095ef8: movs r1,#0x3
  00095efa: blx 0x0006ec2c
  00095efe: ldr.w r1,[r8,#0x3dc]
  00095f02: ldr.w r2,[r1,r4,lsl #0x2]
  00095f06: add r0,r2
  00095f08: str.w r0,[r1,r4,lsl #0x2]
  00095f0c: pop.w r8
  00095f10: pop {r4,r5,r6,r7,pc}
```
