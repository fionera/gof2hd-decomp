# AbyssEngine::MeshConvertToVBO
elf 0x6bbac body 58
Sig: undefined __stdcall MeshConvertToVBO(Mesh * param_1)

## decompile
```c

/* AbyssEngine::MeshConvertToVBO(AbyssEngine::Mesh*) */

undefined4 AbyssEngine::MeshConvertToVBO(Mesh *param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0xfffffffc;
  if ((param_1 != (Mesh *)0x0) && (**(char **)(DAT_0007bbe8 + 0x7bbbe) != '\0')) {
    if ((param_1[0x5c] != (Mesh)0x0) || (param_1[0x84] == (Mesh)0x0)) {
      return 0xfffffffc;
    }
    MeshConvertToVBOIntern(param_1);
    TransformConvertToVBO(*(Transform **)(param_1 + 0x34));
    uVar1 = 1;
  }
  return uVar1;
}

```

## target disasm
```
  0007bbac: push {r4,r6,r7,lr}
  0007bbae: add r7,sp,#0x8
  0007bbb0: mov r4,r0
  0007bbb2: mvn r0,#0x3
  0007bbb6: cbz r4,0x0007bbe4
  0007bbb8: ldr r1,[0x0007bbe8]
  0007bbba: add r1,pc
  0007bbbc: ldr r1,[r1,#0x0]
  0007bbbe: ldrb r1,[r1,#0x0]
  0007bbc0: cbz r1,0x0007bbe4
  0007bbc2: ldrb.w r0,[r4,#0x5c]
  0007bbc6: cbz r0,0x0007bbce
  0007bbc8: mvn r0,#0x3
  0007bbcc: pop {r4,r6,r7,pc}
  0007bbce: ldrb.w r0,[r4,#0x84]
  0007bbd2: cmp r0,#0x0
  0007bbd4: beq 0x0007bbc8
  0007bbd6: mov r0,r4
  0007bbd8: blx 0x0006eb6c
  0007bbdc: ldr r0,[r4,#0x34]
  0007bbde: blx 0x0006eb78
  0007bbe2: movs r0,#0x1
  0007bbe4: pop {r4,r6,r7,pc}
```
