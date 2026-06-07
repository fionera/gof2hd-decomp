# BoundingSphere::collide
elf 0x151d68 body 76
Sig: undefined __thiscall collide(BoundingSphere * this, float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingSphere::collide(float, float, float) */

undefined4 __thiscall
BoundingSphere::collide(BoundingSphere *this,float param_1,float param_2,float param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (**(code **)(*(int *)this + 0xc))(this);
  if (iVar1 == 0) {
                    /* WARNING: Could not recover jumptable at 0x001ab990. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (*(code *)(DAT_001ab994 + 0x1ab998))(this);
    return uVar2;
  }
  return 1;
}

```

## target disasm
```
  00071f74: adr r12,0x171f7c
  00071f78: add r12,r12,#0xaf000
  00071f7c: ldr pc,[r12,#0x998]!
  00161d68: push {r4,r5,r6,r7,lr}
  00161d6a: add r7,sp,#0xc
  00161d6c: push {r8,r9,r11}
  00161d70: mov r4,r0
  00161d72: ldr r0,[r0,#0x0]
  00161d74: mov r8,r3
  00161d76: mov r9,r2
  00161d78: mov r6,r1
  00161d7a: ldr r5,[r0,#0xc]
  00161d7c: mov r0,r4
  00161d7e: blx r5
  00161d80: cbz r0,0x00161d8a
  00161d82: movs r0,#0x1
  00161d84: pop.w {r8,r9,r11}
  00161d88: pop {r4,r5,r6,r7,pc}
  00161d8a: mov r0,r4
  00161d8c: mov r1,r6
  00161d8e: mov r2,r9
  00161d90: mov r3,r8
  00161d92: pop.w {r8,r9,r11}
  00161d96: pop.w {r4,r5,r6,r7,lr}
  00161d9a: b.w 0x001ab988
  001ab988: bx pc
  001ab98c: ldr r12,[0x1ab994]
  001ab990: add pc,r12,pc
```
