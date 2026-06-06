# Level::collideStation
elf 0xc4594 body 80
Sig: undefined __stdcall collideStation(Vector param_1)

## decompile
```c

/* Level::collideStation(AbyssEngine::AEMath::Vector) */

undefined4
Level::collideStation(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  
  if (((*(int *)(param_1 + 0x100) != 0) && (**(int **)(*(int *)(param_1 + 0x100) + 4) != 0)) &&
     (iVar1 = Status::inEmptyOrbit((Status *)**(undefined4 **)(DAT_000d45e4 + 0xd45b6)), iVar1 == 0)
     ) {
    piVar2 = (int *)**(undefined4 **)(*(int *)(param_1 + 0x100) + 4);
                    /* WARNING: Could not recover jumptable at 0x000d45e2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (**(code **)(*piVar2 + 0x38))(piVar2,param_2,param_3,param_4);
    return uVar3;
  }
  return 0;
}

```
## target disasm
```
  000d4594: push {r4,r5,r6,r7,lr}
  000d4596: add r7,sp,#0xc
  000d4598: push.w r8
  000d459c: mov r4,r0
  000d459e: ldr.w r0,[r0,#0x100]
  000d45a2: mov r8,r3
  000d45a4: mov r5,r2
  000d45a6: mov r6,r1
  000d45a8: cbz r0,0x000d45be
  000d45aa: ldr r0,[r0,#0x4]
  000d45ac: ldr r0,[r0,#0x0]
  000d45ae: cbz r0,0x000d45be
  000d45b0: ldr r0,[0x000d45e4]
  000d45b2: add r0,pc
  000d45b4: ldr r0,[r0,#0x0]
  000d45b6: ldr r0,[r0,#0x0]
  000d45b8: blx 0x00073d20
  000d45bc: cbz r0,0x000d45c6
  000d45be: movs r0,#0x0
  000d45c0: pop.w r8
  000d45c4: pop {r4,r5,r6,r7,pc}
  000d45c6: ldr.w r0,[r4,#0x100]
  000d45ca: mov r2,r5
  000d45cc: mov r3,r8
  000d45ce: ldr r0,[r0,#0x4]
  000d45d0: ldr r0,[r0,#0x0]
  000d45d2: ldr r1,[r0,#0x0]
  000d45d4: ldr.w r12,[r1,#0x38]
  000d45d8: mov r1,r6
  000d45da: pop.w r8
  000d45de: pop.w {r4,r5,r6,r7,lr}
  000d45e2: bx r12
```
