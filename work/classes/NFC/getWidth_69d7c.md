# NFC::getWidth
elf 0x69d7c body 66
Sig: undefined __stdcall getWidth(void)

## decompile
```c

/* NFC::getWidth() */

undefined4 NFC::getWidth(void)

{
  _jmethodID *p_Var1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  puVar3 = *(undefined4 **)(DAT_00079dc0 + 0x79d86);
  p_Var1 = (_jmethodID *)
           (**(code **)(*(int *)*puVar3 + 0x18))((int *)*puVar3,DAT_00079dc4 + 0x79d92);
  uVar2 = (**(code **)(*(int *)*puVar3 + 0x1c4))
                    ((int *)*puVar3,p_Var1,DAT_00079dc8 + 0x79da4,DAT_00079dcc + 0x79da6);
  uVar2 = _JNIEnv::CallStaticIntMethod((_jclass *)*puVar3,p_Var1,uVar2);
  FUN_00079d5c(*puVar3);
  return uVar2;
}

```

## target disasm
```
  00079d7c: push {r4,r5,r7,lr}
  00079d7e: add r7,sp,#0x8
  00079d80: ldr r0,[0x00079dc0]
  00079d82: add r0,pc
  00079d84: ldr r5,[r0,#0x0]
  00079d86: ldr r0,[r5,#0x0]
  00079d88: ldr r2,[r0,#0x0]
  00079d8a: ldr r1,[0x00079dc4]
  00079d8c: ldr r2,[r2,#0x18]
  00079d8e: add r1,pc
  00079d90: blx r2
  00079d92: mov r4,r0
  00079d94: ldr r0,[r5,#0x0]
  00079d96: ldr r1,[r0,#0x0]
  00079d98: ldr r2,[0x00079dc8]
  00079d9a: ldr r3,[0x00079dcc]
  00079d9c: ldr.w r12,[r1,#0x1c4]
  00079da0: add r2,pc
  00079da2: add r3,pc
  00079da4: mov r1,r4
  00079da6: blx r12
  00079da8: mov r2,r0
  00079daa: ldr r0,[r5,#0x0]
  00079dac: mov r1,r4
  00079dae: blx 0x0006e83c
  00079db2: mov r4,r0
  00079db4: ldr r0,[r5,#0x0]
  00079db6: bl 0x00079d5c
  00079dba: mov r0,r4
  00079dbc: pop {r4,r5,r7,pc}
```
