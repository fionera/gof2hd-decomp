# LevelScript::lookBehind
elf 0x145d24 body 78
Sig: undefined __thiscall lookBehind(LevelScript * this)

## decompile
```c

/* LevelScript::lookBehind() */

void __thiscall LevelScript::lookBehind(LevelScript *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_00155d80 + 0x155d3a);
  iVar1 = *piVar2;
  TargetFollowCamera::setTargetOffset((Vector *)this->m_pCamera);
  TargetFollowCamera::setCamOffset((Vector *)this->m_pCamera);
  if (*piVar2 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00155d24: push {r4,r5,r6,r7,lr}
  00155d26: add r7,sp,#0xc
  00155d28: push.w r11
  00155d2c: sub sp,#0x10
  00155d2e: mov r4,r0
  00155d30: ldr r0,[0x00155d80]
  00155d32: ldr r1,[0x00155d74]
  00155d34: movs r6,#0x0
  00155d36: add r0,pc
  00155d38: ldr r5,[r0,#0x0]
  00155d3a: ldr r0,[r5,#0x0]
  00155d3c: str r0,[sp,#0xc]
  00155d3e: ldr r0,[r4,#0x14]
  00155d40: strd r6,r6,[sp,#0x0]
  00155d44: str r1,[sp,#0x8]
  00155d46: mov r1,sp
  00155d48: blx 0x00072640
  00155d4c: ldr r2,[0x00155d7c]
  00155d4e: ldr r0,[r4,#0x14]
  00155d50: ldr r1,[0x00155d78]
  00155d52: strd r6,r2,[sp,#0x0]
  00155d56: str r1,[sp,#0x8]
  00155d58: mov r1,sp
  00155d5a: blx 0x00076d50
  00155d5e: ldr r0,[sp,#0xc]
  00155d60: ldr r1,[r5,#0x0]
  00155d62: subs r0,r1,r0
  00155d64: ittt eq
  00155d66: add.eq sp,#0x10
  00155d68: pop.eq.w r11
  00155d6c: pop.eq {r4,r5,r6,r7,pc}
  00155d6e: blx 0x0006e824
```
