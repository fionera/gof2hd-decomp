# Engine::DrawLine2D
elf 0x85e2c body 48
Sig: undefined __stdcall DrawLine2D(float * param_1, int param_2, bool param_3)

## decompile
```c

/* AbyssEngine::Engine::DrawLine2D(float*, int, bool) */

void AbyssEngine::Engine::DrawLine2D(float *param_1,int param_2,bool param_3)

{
  int iVar1;
  undefined4 uVar2;
  int in_r3;
  
  iVar1 = DAT_00095e5c;
  param_1[0xd2] = (float)param_2;
  ShaderSetActive((Engine *)param_1,**(int **)(iVar1 + 0x95e3e),(Mesh *)0x0);
  if (in_r3 == 0) {
    uVar2 = 1;
  }
  else {
    uVar2 = 2;
  }
  (*(code *)(DAT_001ab6c4 + 0x1ab6c8))(uVar2,0,param_3);
  return;
}

```

## target disasm
```
  00095e2c: push {r4,r5,r7,lr}
  00095e2e: add r7,sp,#0x8
  00095e30: mov r4,r2
  00095e32: ldr r2,[0x00095e5c]
  00095e34: str.w r1,[r0,#0x348]
  00095e38: mov r5,r3
  00095e3a: add r2,pc
  00095e3c: ldr r2,[r2,#0x0]
  00095e3e: ldr r1,[r2,#0x0]
  00095e40: movs r2,#0x0
  00095e42: blx 0x000707c8
  00095e46: cmp r5,#0x0
  00095e48: ite ne
  00095e4a: mov.ne r0,#0x2
  00095e4c: mov.eq r0,#0x1
  00095e4e: movs r1,#0x0
  00095e50: mov r2,r4
  00095e52: pop.w {r4,r5,r7,lr}
  00095e56: b.w 0x001ab6b8
  001ab6b8: bx pc
```
