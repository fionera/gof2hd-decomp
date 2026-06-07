# NFC::isPad
elf 0x69ccc body 72
Sig: undefined __stdcall isPad(void)

## decompile
```c

/* NFC::isPad() */

bool NFC::isPad(void)

{
  _jmethodID *p_Var1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar4 = *(undefined4 **)(DAT_00079d14 + 0x79cd6);
  p_Var1 = (_jmethodID *)
           (**(code **)(*(int *)*puVar4 + 0x18))((int *)*puVar4,DAT_00079d18 + 0x79ce2);
  uVar2 = (**(code **)(*(int *)*puVar4 + 0x1c4))
                    ((int *)*puVar4,p_Var1,DAT_00079d1c + 0x79cf4,DAT_00079d20 + 0x79cf6);
  iVar3 = _JNIEnv::CallStaticBooleanMethod((_jclass *)*puVar4,p_Var1,uVar2);
  FUN_00079d5c(*puVar4);
  return iVar3 != 0;
}

```

## target disasm
```
  00079ccc: push {r4,r5,r7,lr}
  00079cce: add r7,sp,#0x8
  00079cd0: ldr r0,[0x00079d14]
  00079cd2: add r0,pc
  00079cd4: ldr r5,[r0,#0x0]
  00079cd6: ldr r0,[r5,#0x0]
  00079cd8: ldr r2,[r0,#0x0]
  00079cda: ldr r1,[0x00079d18]
  00079cdc: ldr r2,[r2,#0x18]
  00079cde: add r1,pc
  00079ce0: blx r2
  00079ce2: mov r4,r0
  00079ce4: ldr r0,[r5,#0x0]
  00079ce6: ldr r1,[r0,#0x0]
  00079ce8: ldr r2,[0x00079d1c]
  00079cea: ldr r3,[0x00079d20]
  00079cec: ldr.w r12,[r1,#0x1c4]
  00079cf0: add r2,pc
  00079cf2: add r3,pc
  00079cf4: mov r1,r4
  00079cf6: blx r12
  00079cf8: mov r2,r0
  00079cfa: ldr r0,[r5,#0x0]
  00079cfc: mov r1,r4
  00079cfe: blx 0x0006e830
  00079d02: mov r4,r0
  00079d04: ldr r0,[r5,#0x0]
  00079d06: bl 0x00079d5c
  00079d0a: cmp r4,#0x0
  00079d0c: it ne
  00079d0e: mov.ne r4,#0x1
  00079d10: mov r0,r4
  00079d12: pop {r4,r5,r7,pc}
```
