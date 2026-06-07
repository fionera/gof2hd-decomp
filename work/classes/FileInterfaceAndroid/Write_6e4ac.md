# FileInterfaceAndroid::Write
elf 0x6e4ac body 162
Sig: undefined __thiscall Write(FileInterfaceAndroid * this, uint param_1, void * param_2)

## decompile
```c

/* FileInterfaceAndroid::Write(unsigned int, void const*) */

bool __thiscall FileInterfaceAndroid::Write(FileInterfaceAndroid *this,uint param_1,void *param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  bool bVar4;
  undefined4 *puVar5;
  
  if (*(int *)(this + 8) == 0) {
    if (*(int *)(this + 0x10) == 0) {
      bVar4 = true;
    }
    else {
      puVar5 = *(undefined4 **)(DAT_0007e550 + 0x7e4e0);
      uVar2 = (**(code **)(*(int *)*puVar5 + 0x2c0))((int *)*puVar5,param_1);
      (**(code **)(*(int *)*puVar5 + 0x340))((int *)*puVar5,uVar2,0,param_1,param_2);
      _JNIEnv::CallVoidMethod
                ((_jobject *)*puVar5,*(_jmethodID **)(this + 0x10),
                 **(undefined4 **)(DAT_0007e554 + 0x7e510),uVar2);
      iVar3 = (**(code **)(*(int *)*puVar5 + 0x3c))();
      bVar4 = iVar3 == 0;
      if (!bVar4) {
        (**(code **)(*(int *)*puVar5 + 0x44))();
      }
      (**(code **)(*(int *)*puVar5 + 0x5c))((int *)*puVar5,uVar2);
    }
    return bVar4;
  }
  uVar1 = fwrite(param_2,1,param_1);
  return uVar1 == param_1;
}

```

## target disasm
```
  0007e4ac: push {r4,r5,r6,r7,lr}
  0007e4ae: add r7,sp,#0xc
  0007e4b0: push {r6,r7,r8,r9,r11}
  0007e4b4: ldr r3,[r0,#0x8]
  0007e4b6: mov r4,r2
  0007e4b8: mov r5,r1
  0007e4ba: mov r6,r0
  0007e4bc: cbz r3,0x0007e4d6
  0007e4be: mov r0,r4
  0007e4c0: movs r1,#0x1
  0007e4c2: mov r2,r5
  0007e4c4: blx 0x0006efe0
  0007e4c8: subs r0,r0,r5
  0007e4ca: clz r0,r0
  0007e4ce: lsrs r0,r0,#0x5
  0007e4d0: pop.w {r2,r3,r8,r9,r11}
  0007e4d4: pop {r4,r5,r6,r7,pc}
  0007e4d6: ldr r0,[r6,#0x10]
  0007e4d8: cbz r0,0x0007e534
  0007e4da: ldr r0,[0x0007e550]
  0007e4dc: add r0,pc
  0007e4de: ldr.w r9,[r0,#0x0]
  0007e4e2: ldr.w r0,[r9,#0x0]
  0007e4e6: ldr r1,[r0,#0x0]
  0007e4e8: ldr.w r2,[r1,#0x2c0]
  0007e4ec: mov r1,r5
  0007e4ee: blx r2
  0007e4f0: mov r8,r0
  0007e4f2: ldr.w r0,[r9,#0x0]
  0007e4f6: movs r2,#0x0
  0007e4f8: mov r3,r5
  0007e4fa: ldr r1,[r0,#0x0]
  0007e4fc: ldr.w r12,[r1,#0x340]
  0007e500: mov r1,r8
  0007e502: str r4,[sp,#0x0]
  0007e504: blx r12
  0007e506: ldr r0,[0x0007e554]
  0007e508: mov r3,r8
  0007e50a: ldr r1,[r6,#0x10]
  0007e50c: add r0,pc
  0007e50e: ldr r0,[r0,#0x0]
  0007e510: ldr r2,[r0,#0x0]
  0007e512: ldr.w r0,[r9,#0x0]
  0007e516: blx 0x0006efec
  0007e51a: ldr.w r0,[r9,#0x0]
  0007e51e: ldr r1,[r0,#0x0]
  0007e520: ldr r1,[r1,#0x3c]
  0007e522: blx r1
  0007e524: cbz r0,0x0007e538
  0007e526: ldr.w r0,[r9,#0x0]
  0007e52a: ldr r1,[r0,#0x0]
  0007e52c: ldr r1,[r1,#0x44]
  0007e52e: blx r1
  0007e530: movs r4,#0x0
  0007e532: b 0x0007e53a
  0007e534: movs r4,#0x1
  0007e536: b 0x0007e546
  0007e538: movs r4,#0x1
  0007e53a: ldr.w r0,[r9,#0x0]
  0007e53e: ldr r1,[r0,#0x0]
  0007e540: ldr r2,[r1,#0x5c]
  0007e542: mov r1,r8
  0007e544: blx r2
  0007e546: mov r0,r4
  0007e548: pop.w {r2,r3,r8,r9,r11}
  0007e54c: pop {r4,r5,r6,r7,pc}
```
