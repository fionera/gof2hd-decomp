# SentryGun::SentryGun
elf 0x15c380 body 54
Sig: undefined __thiscall SentryGun(SentryGun * this, Gun * param_1, int param_2, int param_3, int param_4, Level * param_5)

## decompile
```c

/* SentryGun::SentryGun(Gun*, int, int, int, Level*) */

void __thiscall
SentryGun::SentryGun
          (SentryGun *this,Gun *param_1,int param_2,int param_3,int param_4,Level *param_5)

{
  int *piVar1;
  
  piVar1 = (int *)ObjectGun::ObjectGun((ObjectGun *)this,param_3,param_1,param_2,0,param_5);
  *piVar1 = *(int *)(DAT_0016c3b8 + 0x16c3a0) + 8;
  piVar1[0x2c] = *(int *)(param_1 + 0x58) * 3 + -0x279;
  return;
}

```

## target disasm
```
  0016c380: push {r2,r3,r4,r6,r7,lr}
  0016c382: add r7,sp,#0x10
  0016c384: mov r4,r1
  0016c386: ldr r1,[r7,#0xc]
  0016c388: mov r12,r2
  0016c38a: movs r2,#0x0
  0016c38c: strd r2,r1,[sp,#0x0]
  0016c390: mov r1,r3
  0016c392: mov r2,r4
  0016c394: mov r3,r12
  0016c396: blx 0x0007759c
  0016c39a: ldr r1,[0x0016c3b8]
  0016c39c: add r1,pc
  0016c39e: ldr r1,[r1,#0x0]
  0016c3a0: adds r1,#0x8
  0016c3a2: str r1,[r0,#0x0]
  0016c3a4: ldr r1,[r4,#0x58]
  0016c3a6: add.w r1,r1,r1, lsl #0x1
  0016c3aa: subw r1,r1,#0x279
  0016c3ae: str.w r1,[r0,#0xb0]
  0016c3b2: add sp,#0x8
  0016c3b4: pop {r4,r6,r7,pc}
```
