# String::SubString
elf 0x72ed8 body 144
Sig: undefined __stdcall SubString(uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::String::SubString(unsigned int, unsigned int) */

void AbyssEngine::String::SubString(uint param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  void *pvVar4;
  uint in_r2;
  uint in_r3;
  int iVar5;
  int *piVar6;
  
  piVar6 = *(int **)(DAT_00082f78 + 0x82eec);
  iVar1 = *piVar6;
  if (in_r2 < in_r3) {
    iVar5 = in_r3 - in_r2;
    uVar2 = iVar5 + 1;
    uVar3 = uVar2 * 2;
    if (CARRY4(uVar2,uVar2)) {
      uVar3 = 0xffffffff;
    }
    pvVar4 = operator_new__(uVar3);
    __aeabi_memcpy(pvVar4,*(int *)(param_2 + 4) + in_r2 * 2,iVar5 * 2);
    *(undefined2 *)((int)pvVar4 + iVar5 * 2) = 0;
    String((String *)param_1,(String *)&stack0xffffffd0,false);
  }
  else {
    *(int *)param_1 = *(int *)(DAT_00082f7c + 0x82ef0) + 8;
    *(undefined4 *)(param_1 + 4) = 0;
    *(undefined4 *)(param_1 + 8) = 0;
  }
  ~String((String *)&stack0xffffffd0);
  iVar5 = *piVar6;
  iVar1 = iVar5 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar5);
  }
  return;
}

```

## target disasm
```
  00082ed8: push {r4,r5,r6,r7,lr}
  00082eda: add r7,sp,#0xc
  00082edc: push {r4,r5,r6,r7,r8,r9,r10}
  00082ee0: mov r10,r0
  00082ee2: ldr r0,[0x00082f78]
  00082ee4: mov r8,r1
  00082ee6: ldr r1,[0x00082f7c]
  00082ee8: add r0,pc
  00082eea: mov r5,r2
  00082eec: add r1,pc
  00082eee: cmp r3,r2
  00082ef0: ldr.w r9,[r0,#0x0]
  00082ef4: ldr r1,[r1,#0x0]
  00082ef6: ldr.w r0,[r9,#0x0]
  00082efa: add.w r1,r1,#0x8
  00082efe: str r0,[sp,#0xc]
  00082f00: mov.w r0,#0x0
  00082f04: str r0,[sp,#0x4]
  00082f06: str r1,[sp,#0x0]
  00082f08: str r0,[sp,#0x8]
  00082f0a: bls 0x00082f46
  00082f0c: subs r4,r3,r5
  00082f0e: str r0,[sp,#0x4]
  00082f10: adds r0,r4,#0x1
  00082f12: adds r0,r0,r0
  00082f14: it cs
  00082f16: mov.cs.w r0,#0xffffffff
  00082f1a: blx 0x0006ec08
  00082f1e: str r0,[sp,#0x4]
  00082f20: mov r6,r0
  00082f22: ldr.w r0,[r8,#0x4]
  00082f26: lsls r2,r4,#0x1
  00082f28: add.w r1,r0,r5, lsl #0x1
  00082f2c: mov r0,r6
  00082f2e: blx 0x0006f1e4
  00082f32: movs r0,#0x0
  00082f34: str r4,[sp,#0x8]
  00082f36: strh.w r0,[r6,r4,lsl #0x1]
  00082f3a: mov r1,sp
  00082f3c: mov r0,r10
  00082f3e: movs r2,#0x0
  00082f40: blx 0x0006f5b0
  00082f44: b 0x00082f4e
  00082f46: strd r1,r0,[r10,#0x0]
  00082f4a: str.w r0,[r10,#0x8]
  00082f4e: mov r0,sp
  00082f50: blx 0x0006f580
  00082f54: ldr r0,[sp,#0xc]
  00082f56: ldr.w r1,[r9,#0x0]
  00082f5a: subs r0,r1,r0
  00082f5c: itt eq
  00082f5e: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00082f62: pop.eq {r4,r5,r6,r7,pc}
  00082f64: blx 0x0006e824
```
