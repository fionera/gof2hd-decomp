# BoundingAAB::collide
elf 0x9819c body 54
Sig: undefined __thiscall collide(BoundingAAB * this, float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingAAB::collide(float, float, float) */

undefined4 __thiscall
BoundingAAB::collide(BoundingAAB *this,float param_1,float param_2,float param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (**(code **)(*(int *)this + 0xc))(this);
  if (iVar1 == 0) {
    return 0;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab990. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar2 = (*(code *)(DAT_001ab994 + 0x1ab998))(this);
  return uVar2;
}

```

## target disasm
```
  000a819c: push {r4,r5,r6,r7,lr}
  000a819e: add r7,sp,#0xc
  000a81a0: push {r8,r9,r11}
  000a81a4: mov r4,r0
  000a81a6: ldr r0,[r0,#0x0]
  000a81a8: mov r8,r3
  000a81aa: mov r9,r2
  000a81ac: mov r6,r1
  000a81ae: ldr r5,[r0,#0xc]
  000a81b0: mov r0,r4
  000a81b2: blx r5
  000a81b4: cbz r0,0x000a81ca
  000a81b6: mov r0,r4
  000a81b8: mov r1,r6
  000a81ba: mov r2,r9
  000a81bc: mov r3,r8
  000a81be: pop.w {r8,r9,r11}
  000a81c2: pop.w {r4,r5,r6,r7,lr}
  000a81c6: b.w 0x001ab988
  000a81ca: movs r0,#0x0
  000a81cc: pop.w {r8,r9,r11}
  000a81d0: pop {r4,r5,r6,r7,pc}
```
