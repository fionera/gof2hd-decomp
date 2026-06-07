# FileRead::loadNamesBinary
elf 0x120dd0 body 654
Sig: undefined __thiscall loadNamesBinary(FileRead * this, int param_1, bool param_2, bool param_3)

## decompile
```c

/* FileRead::loadNamesBinary(int, bool, bool) */

void __thiscall FileRead::loadNamesBinary(FileRead *this,int param_1,bool param_2,bool param_3)

{
  Array *pAVar1;
  String *this_00;
  int iVar2;
  char *pcVar3;
  int *piVar4;
  uint local_4c;
  uint local_48 [3];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar4 = *(int **)(DAT_001310dc + 0x130de6);
  local_24 = *piVar4;
  AbyssEngine::String::String(aSStack_30,(char *)(DAT_001310e0 + 0x130dfc),false);
  switch(param_1) {
  case 0:
    pcVar3 = (char *)(DAT_001310ec + 0x130e24);
    if (!param_2) {
      pcVar3 = (char *)(DAT_001310e4 + 0x130e20);
    }
    if (!param_3) {
      pcVar3 = (char *)(DAT_001310e8 + 0x130e22);
    }
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 1:
    pcVar3 = (char *)(DAT_001310f8 + 0x130e4a);
    if (!param_3) {
      pcVar3 = (char *)(DAT_001310f4 + 0x130e48);
    }
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 2:
    pcVar3 = (char *)(DAT_00131100 + 0x130e6a);
    if (!param_3) {
      pcVar3 = (char *)(DAT_001310fc + 0x130e68);
    }
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 3:
    if (param_3) {
      iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00131104 + 0x130e86));
      pcVar3 = (char *)(DAT_0013110c + 0x130e9c);
      if (iVar2 != 0) {
        pcVar3 = (char *)(DAT_00131108 + 0x130e9a);
      }
      AbyssEngine::String::String(aSStack_3c,pcVar3,false);
      AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    }
    else {
      iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00131104 + 0x130e86));
      pcVar3 = (char *)(DAT_00131114 + 0x130f6e);
      if (iVar2 != 0) {
        pcVar3 = (char *)(DAT_00131110 + 0x130f6c);
      }
      AbyssEngine::String::String(aSStack_3c,pcVar3,false);
      AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    }
    break;
  case 4:
    pcVar3 = (char *)(DAT_0013111c + 0x130ebc);
    if (!param_3) {
      pcVar3 = (char *)(DAT_00131118 + 0x130eba);
    }
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 5:
    if (!param_3) goto switchD_00130e04_default;
    AbyssEngine::String::String(aSStack_3c,(char *)(DAT_00131120 + 0x130edc),false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 6:
    pcVar3 = (char *)(DAT_00131128 + 0x130ef8);
    if (!param_3) {
      pcVar3 = (char *)(DAT_00131124 + 0x130ef6);
    }
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 7:
    if (!param_3) goto switchD_00130e04_default;
    AbyssEngine::String::String(aSStack_3c,(char *)(DAT_0013112c + 0x130f18),false);
    AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    break;
  case 8:
    if (param_3) {
      iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00131130 + 0x130f30));
      pcVar3 = (char *)(DAT_00131138 + 0x130f46);
      if (iVar2 != 0) {
        pcVar3 = (char *)(DAT_00131134 + 0x130f44);
      }
      AbyssEngine::String::String(aSStack_3c,pcVar3,false);
      AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    }
    else {
      iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00131130 + 0x130f30));
      pcVar3 = (char *)(DAT_00131140 + 0x130f96);
      if (iVar2 != 0) {
        pcVar3 = (char *)(DAT_0013113c + 0x130f94);
      }
      AbyssEngine::String::String(aSStack_3c,pcVar3,false);
      AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
    }
    break;
  default:
    goto switchD_00130e04_default;
  }
  AbyssEngine::String::~String(aSStack_3c);
  AbyssEngine::String::String((String *)local_48,(char *)(DAT_001310f0 + 0x130fb4),false);
  AbyssEngine::operator+(aSStack_3c,(String *)local_48);
  AbyssEngine::String::operator=(aSStack_30,aSStack_3c);
  AbyssEngine::String::~String(aSStack_3c);
  AbyssEngine::String::~String((String *)local_48);
  iVar2 = AEFile::FileExist(aSStack_30);
  if (iVar2 != 0) {
    AEFile::OpenRead(aSStack_30,local_48);
    AEFile::ReadSwitched((int *)&local_4c,local_48[0]);
    pAVar1 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    ArraySetLength<AbyssEngine::String*>(local_4c,pAVar1);
    AbyssEngine::String::String(aSStack_3c);
    for (iVar2 = 0; iVar2 < (int)local_4c; iVar2 = iVar2 + 1) {
      AEFile::ReadSwitched(aSStack_3c,local_48[0]);
      this_00 = operator_new(0xc);
      AbyssEngine::String::String(this_00,aSStack_3c,false);
      *(String **)(*(int *)(pAVar1 + 4) + iVar2 * 4) = this_00;
    }
    AEFile::Close(local_48[0]);
    AbyssEngine::String::~String(aSStack_3c);
  }
switchD_00130e04_default:
  AbyssEngine::String::~String(aSStack_30);
  iVar2 = *piVar4 - local_24;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  00130dd0: push {r4,r5,r6,r7,lr}
  00130dd2: add r7,sp,#0xc
  00130dd4: push {r8,r9,r10}
  00130dd8: sub sp,#0x30
  00130dda: ldr r0,[0x001310dc]
  00130ddc: mov r6,r1
  00130dde: mov r8,r2
  00130de0: movs r2,#0x0
  00130de2: add r0,pc
  00130de4: mov r4,r3
  00130de6: mov.w r10,#0x0
  00130dea: ldr.w r9,[r0,#0x0]
  00130dee: ldr.w r0,[r9,#0x0]
  00130df2: ldr r1,[0x001310e0]
  00130df4: str r0,[sp,#0x2c]
  00130df6: add r0,sp,#0x20
  00130df8: add r1,pc
  00130dfa: blx 0x0006ee18
  00130dfe: cmp r6,#0x8
  00130e00: bhi.w 0x0013104a
  00130e04: tbb [pc,r6]
  00130e12: ldr r0,[0x001310e4]
  00130e14: cmp.w r8,#0x0
  00130e18: ldr r2,[0x001310e8]
  00130e1a: ldr r1,[0x001310ec]
  00130e1c: add r0,pc
  00130e1e: add r2,pc
  00130e20: add r1,pc
  00130e22: it eq
  00130e24: mov.eq r1,r0
  00130e26: cmp r4,#0x0
  00130e28: it eq
  00130e2a: mov.eq r1,r2
  00130e2c: add r0,sp,#0x14
  00130e2e: movs r2,#0x0
  00130e30: blx 0x0006ee18
  00130e34: add r0,sp,#0x20
  00130e36: add r1,sp,#0x14
  00130e38: blx 0x0006f2b0
  00130e3c: b 0x00130fa8
  00130e3e: ldr r0,[0x001310f4]
  00130e40: cmp r4,#0x0
  00130e42: ldr r1,[0x001310f8]
  00130e44: add r0,pc
  00130e46: add r1,pc
  00130e48: it eq
  00130e4a: mov.eq r1,r0
  00130e4c: add r0,sp,#0x14
  00130e4e: movs r2,#0x0
  00130e50: blx 0x0006ee18
  00130e54: add r0,sp,#0x20
  00130e56: add r1,sp,#0x14
  00130e58: blx 0x0006f2b0
  00130e5c: b 0x00130fa8
  00130e5e: ldr r0,[0x001310fc]
  00130e60: cmp r4,#0x0
  00130e62: ldr r1,[0x00131100]
  00130e64: add r0,pc
  00130e66: add r1,pc
  00130e68: it eq
  00130e6a: mov.eq r1,r0
  00130e6c: add r0,sp,#0x14
  00130e6e: movs r2,#0x0
  00130e70: blx 0x0006ee18
  00130e74: add r0,sp,#0x20
  00130e76: add r1,sp,#0x14
  00130e78: blx 0x0006f2b0
  00130e7c: b 0x00130fa8
  00130e7e: ldr r0,[0x00131104]
  00130e80: cmp r4,#0x0
  00130e82: add r0,pc
  00130e84: ldr r0,[r0,#0x0]
  00130e86: beq 0x00130f5a
  00130e88: ldr r0,[r0,#0x0]
  00130e8a: movs r1,#0x2
  00130e8c: blx 0x00071848
  00130e90: ldr r2,[0x00131108]
  00130e92: cmp r0,#0x0
  00130e94: ldr r1,[0x0013110c]
  00130e96: add r2,pc
  00130e98: add r1,pc
  00130e9a: it ne
  00130e9c: mov.ne r1,r2
  00130e9e: add r0,sp,#0x14
  00130ea0: movs r2,#0x0
  00130ea2: blx 0x0006ee18
  00130ea6: add r0,sp,#0x20
  00130ea8: add r1,sp,#0x14
  00130eaa: blx 0x0006f2b0
  00130eae: b 0x00130fa8
  00130eb0: ldr r0,[0x00131118]
  00130eb2: cmp r4,#0x0
  00130eb4: ldr r1,[0x0013111c]
  00130eb6: add r0,pc
  00130eb8: add r1,pc
  00130eba: it eq
  00130ebc: mov.eq r1,r0
  00130ebe: add r0,sp,#0x14
  00130ec0: movs r2,#0x0
  00130ec2: blx 0x0006ee18
  00130ec6: add r0,sp,#0x20
  00130ec8: add r1,sp,#0x14
  00130eca: blx 0x0006f2b0
  00130ece: b 0x00130fa8
  00130ed0: cmp r4,#0x0
  00130ed2: beq.w 0x00131046
  00130ed6: ldr r1,[0x00131120]
  00130ed8: add r1,pc
  00130eda: add r0,sp,#0x14
  00130edc: movs r2,#0x0
  00130ede: blx 0x0006ee18
  00130ee2: add r0,sp,#0x20
  00130ee4: add r1,sp,#0x14
  00130ee6: blx 0x0006f2b0
  00130eea: b 0x00130fa8
  00130eec: ldr r0,[0x00131124]
  00130eee: cmp r4,#0x0
  00130ef0: ldr r1,[0x00131128]
  00130ef2: add r0,pc
  00130ef4: add r1,pc
  00130ef6: it eq
  00130ef8: mov.eq r1,r0
  00130efa: add r0,sp,#0x14
  00130efc: movs r2,#0x0
  00130efe: blx 0x0006ee18
  00130f02: add r0,sp,#0x20
  00130f04: add r1,sp,#0x14
  00130f06: blx 0x0006f2b0
  00130f0a: b 0x00130fa8
  00130f0c: cmp r4,#0x0
  00130f0e: beq.w 0x00131046
  00130f12: ldr r1,[0x0013112c]
  00130f14: add r1,pc
  00130f16: add r0,sp,#0x14
  00130f18: movs r2,#0x0
  00130f1a: blx 0x0006ee18
  00130f1e: add r0,sp,#0x20
  00130f20: add r1,sp,#0x14
  00130f22: blx 0x0006f2b0
  00130f26: b 0x00130fa8
  00130f28: ldr r0,[0x00131130]
  00130f2a: cmp r4,#0x0
  00130f2c: add r0,pc
  00130f2e: ldr r0,[r0,#0x0]
  00130f30: beq 0x00130f82
  00130f32: ldr r0,[r0,#0x0]
  00130f34: movs r1,#0x2
  00130f36: blx 0x00071848
  00130f3a: ldr r2,[0x00131134]
  00130f3c: cmp r0,#0x0
  00130f3e: ldr r1,[0x00131138]
  00130f40: add r2,pc
  00130f42: add r1,pc
  00130f44: it ne
  00130f46: mov.ne r1,r2
  00130f48: add r0,sp,#0x14
  00130f4a: movs r2,#0x0
  00130f4c: blx 0x0006ee18
  00130f50: add r0,sp,#0x20
  00130f52: add r1,sp,#0x14
  00130f54: blx 0x0006f2b0
  00130f58: b 0x00130fa8
  00130f5a: ldr r0,[r0,#0x0]
  00130f5c: movs r1,#0x2
  00130f5e: blx 0x00071848
  00130f62: ldr r2,[0x00131110]
  00130f64: cmp r0,#0x0
  00130f66: ldr r1,[0x00131114]
  00130f68: add r2,pc
  00130f6a: add r1,pc
  00130f6c: it ne
  00130f6e: mov.ne r1,r2
  00130f70: add r0,sp,#0x14
  00130f72: movs r2,#0x0
  00130f74: blx 0x0006ee18
  00130f78: add r0,sp,#0x20
  00130f7a: add r1,sp,#0x14
  00130f7c: blx 0x0006f2b0
  00130f80: b 0x00130fa8
  00130f82: ldr r0,[r0,#0x0]
  00130f84: movs r1,#0x2
  00130f86: blx 0x00071848
  00130f8a: ldr r2,[0x0013113c]
  00130f8c: cmp r0,#0x0
  00130f8e: ldr r1,[0x00131140]
  00130f90: add r2,pc
  00130f92: add r1,pc
  00130f94: it ne
  00130f96: mov.ne r1,r2
  00130f98: add r0,sp,#0x14
  00130f9a: movs r2,#0x0
  00130f9c: blx 0x0006ee18
  00130fa0: add r0,sp,#0x20
  00130fa2: add r1,sp,#0x14
  00130fa4: blx 0x0006f2b0
  00130fa8: add r0,sp,#0x14
  00130faa: blx 0x0006ee30
  00130fae: ldr r1,[0x001310f0]
  00130fb0: add r1,pc
  00130fb2: add r0,sp,#0x8
  00130fb4: movs r2,#0x0
  00130fb6: blx 0x0006ee18
  00130fba: add r0,sp,#0x14
  00130fbc: add r1,sp,#0x8
  00130fbe: add r2,sp,#0x20
  00130fc0: blx 0x0006ef98
  00130fc4: add r0,sp,#0x20
  00130fc6: add r1,sp,#0x14
  00130fc8: blx 0x0006f2b0
  00130fcc: add r0,sp,#0x14
  00130fce: blx 0x0006ee30
  00130fd2: add r0,sp,#0x8
  00130fd4: blx 0x0006ee30
  00130fd8: add r0,sp,#0x20
  00130fda: blx 0x0006fff4
  00130fde: cbz r0,0x00131046
  00130fe0: add r0,sp,#0x20
  00130fe2: add r1,sp,#0x8
  00130fe4: blx 0x0006f34c
  00130fe8: ldr r1,[sp,#0x8]
  00130fea: add r0,sp,#0x4
  00130fec: blx 0x0007630c
  00130ff0: movs r0,#0xc
  00130ff2: blx 0x0006eb24
  00130ff6: mov r10,r0
  00130ff8: blx 0x0006f628
  00130ffc: ldr r0,[sp,#0x4]
  00130ffe: mov r1,r10
  00131000: blx 0x0006fe08
  00131004: add r0,sp,#0x14
  00131006: blx 0x0006efbc
  0013100a: add r4,sp,#0x14
  0013100c: movs r5,#0x0
  0013100e: b 0x00131030
  00131010: mov r0,r4
  00131012: blx 0x000762f4
  00131016: movs r0,#0xc
  00131018: blx 0x0006eb24
  0013101c: mov r6,r0
  0013101e: mov r1,r4
  00131020: movs r2,#0x0
  00131022: blx 0x0006f028
  00131026: ldr.w r0,[r10,#0x4]
  0013102a: str.w r6,[r0,r5,lsl #0x2]
  0013102e: adds r5,#0x1
  00131030: ldrd r0,r1,[sp,#0x4]
  00131034: cmp r5,r0
  00131036: blt 0x00131010
  00131038: mov r0,r1
  0013103a: blx 0x0006ece0
  0013103e: add r0,sp,#0x14
  00131040: blx 0x0006ee30
  00131044: b 0x0013104a
  00131046: mov.w r10,#0x0
  0013104a: add r0,sp,#0x20
  0013104c: blx 0x0006ee30
  00131050: ldr r0,[sp,#0x2c]
  00131052: ldr.w r1,[r9,#0x0]
  00131056: subs r0,r1,r0
  00131058: itttt eq
  0013105a: mov.eq r0,r10
  0013105c: add.eq sp,#0x30
  0013105e: pop.eq.w {r8,r9,r10}
  00131062: pop.eq {r4,r5,r6,r7,pc}
  00131064: blx 0x0006e824
```
