# AEPakFile::Skip
elf 0x700a2 body 46
Sig: undefined __thiscall Skip(AEPakFile * this, uint param_1)

## decompile
```c

/* AEPakFile::Skip(unsigned int) */

undefined4 __thiscall AEPakFile::Skip(AEPakFile *this,uint param_1)

{
  void *pvVar1;
  
  pvVar1 = operator_new__(param_1);
  (**(code **)(*(int *)this + 0xc))(this,param_1,pvVar1);
  operator_delete__(pvVar1);
  return 1;
}

```

## target disasm
```
  000800a2: push {r4,r5,r6,r7,lr}
  000800a4: add r7,sp,#0xc
  000800a6: push.w r11
  000800aa: mov r5,r0
  000800ac: mov r0,r1
  000800ae: mov r4,r1
  000800b0: blx 0x0006ec08
  000800b4: mov r6,r0
  000800b6: ldr r0,[r5,#0x0]
  000800b8: mov r1,r4
  000800ba: mov r2,r6
  000800bc: ldr r3,[r0,#0xc]
  000800be: mov r0,r5
  000800c0: blx r3
  000800c2: mov r0,r6
  000800c4: blx 0x0006ebfc
  000800c8: movs r0,#0x1
  000800ca: pop.w r11
  000800ce: pop {r4,r5,r6,r7,pc}
```
