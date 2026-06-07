# LevelScript::render3D
elf 0x145d84 body 126
Sig: undefined __stdcall render3D(void)

## decompile
```c

/* LevelScript::render3D() */

void LevelScript::render3D(void)

{
  int in_r0;
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  
  if (*(int *)(in_r0 + 0xdc) != 0) {
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xd8) != 0) {
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xb8) != 0) {
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xbc) != 0) {
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xc0) != 0) {
    AEGeometry::render();
  }
  if (*(int *)(in_r0 + 0xc4) != 0) {
    AEGeometry::render();
  }
  if ((*(int *)(in_r0 + 200) != 0) && (iVar1 = Explosion::isPlaying(), iVar1 != 0)) {
    Explosion::render();
  }
  if ((*(char *)(in_r0 + 0xa8) != '\0') && (*(int *)(in_r0 + 0xac) != 0)) {
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_00155e04 + 0x155dec);
    (*UNRECOVERED_JUMPTABLE)();
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(in_r0 + 0xb0));
                    /* WARNING: Could not recover jumptable at 0x00155dfe. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(in_r0 + 0xb4));
    return;
  }
  return;
}

```

## target disasm
```
  00155d84: push {r4,r5,r7,lr}
  00155d86: add r7,sp,#0x8
  00155d88: mov r4,r0
  00155d8a: ldr.w r0,[r0,#0xdc]
  00155d8e: cbz r0,0x00155d94
  00155d90: blx 0x00072238
  00155d94: ldr.w r0,[r4,#0xd8]
  00155d98: cbz r0,0x00155d9e
  00155d9a: blx 0x00072238
  00155d9e: ldr.w r0,[r4,#0xb8]
  00155da2: cbz r0,0x00155da8
  00155da4: blx 0x00072238
  00155da8: ldr.w r0,[r4,#0xbc]
  00155dac: cbz r0,0x00155db2
  00155dae: blx 0x00072238
  00155db2: ldr.w r0,[r4,#0xc0]
  00155db6: cbz r0,0x00155dbc
  00155db8: blx 0x00072238
  00155dbc: ldr.w r0,[r4,#0xc4]
  00155dc0: cbz r0,0x00155dc6
  00155dc2: blx 0x00072238
  00155dc6: ldr.w r0,[r4,#0xc8]
  00155dca: cbz r0,0x00155dda
  00155dcc: blx 0x000757fc
  00155dd0: cbz r0,0x00155dda
  00155dd2: ldr.w r0,[r4,#0xc8]
  00155dd6: blx 0x00072d18
  00155dda: ldrb.w r0,[r4,#0xa8]
  00155dde: cbz r0,0x00155e00
  00155de0: ldr.w r0,[r4,#0xac]
  00155de4: cbz r0,0x00155e00
  00155de6: ldr r1,[0x00155e04]
  00155de8: add r1,pc
  00155dea: ldr r5,[r1,#0x0]
  00155dec: blx r5
  00155dee: ldr.w r0,[r4,#0xb0]
  00155df2: blx r5
  00155df4: ldr.w r0,[r4,#0xb4]
  00155df8: mov r1,r5
  00155dfa: pop.w {r4,r5,r7,lr}
  00155dfe: bx r1
  00155e00: pop {r4,r5,r7,pc}
```
