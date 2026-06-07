# ShaderBaseStruct::ES2LoadShader
elf 0x8e4f0 body 144
Sig: undefined __thiscall ES2LoadShader(ShaderBaseStruct * this, uint param_1, char * param_2)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::ES2LoadShader(unsigned int, char const*) */

void __thiscall
AbyssEngine::ShaderBaseStruct::ES2LoadShader(ShaderBaseStruct *this,uint param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  int local_28;
  int local_24;
  char *local_20;
  int local_1c;
  
  piVar4 = *(int **)(DAT_0009e580 + 0x9e502);
  local_1c = *piVar4;
  local_20 = param_2;
  iVar2 = glCreateShader(param_1);
  if (iVar2 != 0) {
    glShaderSource(iVar2,1,&local_20,0);
    glCompileShader(iVar2);
    glGetShaderiv(iVar2,0x8b81,&local_24);
    if (local_24 == 0) {
      local_28 = 0;
      glGetShaderiv(iVar2,0x8b84,&local_28);
      iVar1 = local_28;
      if (1 < local_28) {
        uVar3 = malloc(local_28);
        glGetShaderInfoLog(iVar2,iVar1,0,uVar3);
        free(uVar3);
      }
      glDeleteShader(iVar2);
    }
  }
  iVar2 = *piVar4 - local_1c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0009e4f0: push {r4,r5,r6,r7,lr}
  0009e4f2: add r7,sp,#0xc
  0009e4f4: push.w r8
  0009e4f8: sub sp,#0x10
  0009e4fa: mov r0,r1
  0009e4fc: ldr r1,[0x0009e580]
  0009e4fe: add r1,pc
  0009e500: ldr r6,[r1,#0x0]
  0009e502: ldr r1,[r6,#0x0]
  0009e504: str r2,[sp,#0x8]
  0009e506: str r1,[sp,#0xc]
  0009e508: blx 0x00070fb4
  0009e50c: mov r4,r0
  0009e50e: cbz r0,0x0009e568
  0009e510: add r2,sp,#0x8
  0009e512: mov r0,r4
  0009e514: movs r1,#0x1
  0009e516: movs r3,#0x0
  0009e518: movs r5,#0x0
  0009e51a: blx 0x00070fc0
  0009e51e: mov r0,r4
  0009e520: blx 0x00070fcc
  0009e524: add r2,sp,#0x4
  0009e526: mov r0,r4
  0009e528: movw r1,#0x8b81
  0009e52c: blx 0x00070fd8
  0009e530: ldr r0,[sp,#0x4]
  0009e532: cbnz r0,0x0009e56a
  0009e534: mov r2,sp
  0009e536: mov r0,r4
  0009e538: movw r1,#0x8b84
  0009e53c: str r5,[sp,#0x0]
  0009e53e: blx 0x00070fd8
  0009e542: ldr r5,[sp,#0x0]
  0009e544: cmp r5,#0x2
  0009e546: blt 0x0009e562
  0009e548: mov r0,r5
  0009e54a: blx 0x0006e9a4
  0009e54e: mov r8,r0
  0009e550: mov r0,r4
  0009e552: mov r1,r5
  0009e554: movs r2,#0x0
  0009e556: mov r3,r8
  0009e558: blx 0x00070fe4
  0009e55c: mov r0,r8
  0009e55e: blx 0x0006eff8
  0009e562: mov r0,r4
  0009e564: blx 0x00070ff0
  0009e568: movs r4,#0x0
  0009e56a: ldr r0,[sp,#0xc]
  0009e56c: ldr r1,[r6,#0x0]
  0009e56e: subs r0,r1,r0
  0009e570: itttt eq
  0009e572: mov.eq r0,r4
  0009e574: add.eq sp,#0x10
  0009e576: pop.eq.w r8
  0009e57a: pop.eq {r4,r5,r6,r7,pc}
  0009e57c: blx 0x0006e824
```
