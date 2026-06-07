# NFC::getHeight
elf 0x69e08 body 66
Sig: undefined __stdcall getHeight(void)

## decompile
```c

/* NFC::getHeight() */

undefined4 NFC::getHeight(void)

{
  _jmethodID *p_Var1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  puVar3 = *(undefined4 **)(DAT_00079e4c + 0x79e12);
  p_Var1 = (_jmethodID *)
           (**(code **)(*(int *)*puVar3 + 0x18))((int *)*puVar3,DAT_00079e50 + 0x79e1e);
  uVar2 = (**(code **)(*(int *)*puVar3 + 0x1c4))
                    ((int *)*puVar3,p_Var1,DAT_00079e54 + 0x79e30,DAT_00079e58 + 0x79e32);
  uVar2 = _JNIEnv::CallStaticIntMethod((_jclass *)*puVar3,p_Var1,uVar2);
  FUN_00079d5c(*puVar3);
  return uVar2;
}

```

## target disasm
```
  00079e08: push {r4,r5,r7,lr}
  00079e0a: add r7,sp,#0x8
  00079e0c: ldr r0,[0x00079e4c]
  00079e0e: add r0,pc
  00079e10: ldr r5,[r0,#0x0]
  00079e12: ldr r0,[r5,#0x0]
  00079e14: ldr r2,[r0,#0x0]
  00079e16: ldr r1,[0x00079e50]
  00079e18: ldr r2,[r2,#0x18]
  00079e1a: add r1,pc
  00079e1c: blx r2
  00079e1e: mov r4,r0
  00079e20: ldr r0,[r5,#0x0]
  00079e22: ldr r1,[r0,#0x0]
  00079e24: ldr r2,[0x00079e54]
  00079e26: ldr r3,[0x00079e58]
  00079e28: ldr.w r12,[r1,#0x1c4]
  00079e2c: add r2,pc
  00079e2e: add r3,pc
  00079e30: mov r1,r4
  00079e32: blx r12
  00079e34: mov r2,r0
  00079e36: ldr r0,[r5,#0x0]
  00079e38: mov r1,r4
  00079e3a: blx 0x0006e83c
  00079e3e: mov r4,r0
  00079e40: ldr r0,[r5,#0x0]
  00079e42: bl 0x00079d5c
  00079e46: mov r0,r4
  00079e48: pop {r4,r5,r7,pc}
```
