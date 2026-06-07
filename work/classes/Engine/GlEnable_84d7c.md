# Engine::GlEnable
elf 0x84d7c body 566
Sig: undefined __stdcall GlEnable(uint param_1, bool param_2)

## decompile
```c

/* WARNING: Removing unreachable block (ram,0x00094df6) */
/* AbyssEngine::Engine::GlEnable(unsigned int, bool) */

void AbyssEngine::Engine::GlEnable(uint param_1,bool param_2)

{
  uint uVar1;
  int in_r2;
  
  uVar1 = (uint)param_2;
  if (**(char **)(DAT_00094ff8 + 0x94d82) == '\0') {
    if (uVar1 == 0x1000000) {
      uVar1 = 0xbc0;
    }
    if (in_r2 == 0) {
      (*(code *)(DAT_001ab394 + 0x1ab398))(uVar1);
      return;
    }
    (*(code *)(DAT_001ab634 + 0x1ab638))();
    return;
  }
  switch(uVar1) {
  case 0x1100000:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffffb;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 4;
    }
    break;
  case 0x1100001:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffff7;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 8;
    }
    break;
  case 0x1100002:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffffef;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x10;
    }
    break;
  case 0x1100003:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffffdf;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x20;
    }
    break;
  case 0x1100004:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffff7f;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x80;
    }
    break;
  case 0x1100005:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffffbf;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x40;
    }
    break;
  case 0x1100006:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffeff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x100;
    }
    break;
  case 0x1100007:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffdff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x200;
    }
    break;
  case 0x1100008:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffbff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x400;
    }
    break;
  case 0x1100009:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffefff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x1000;
    }
    break;
  case 0x110000a:
  case 0x110000b:
  case 0x110000c:
  case 0x110000d:
  case 0x110000e:
  case 0x110000f:
  case 0x110001a:
  case 0x110001b:
  case 0x110001c:
  case 0x110001d:
  case 0x110001e:
  case 0x110001f:
    goto switchD_00094d90_caseD_110000a;
  case 0x1100010:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffdfff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x2000;
    }
    break;
  case 0x1100011:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffffbfff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x4000;
    }
    break;
  case 0x1100012:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffff7fff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x8000;
    }
    break;
  case 0x1100013:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffeffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x10000;
    }
    break;
  case 0x1100014:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffdffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x20000;
    }
    break;
  case 0x1100015:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfffbffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x40000;
    }
    break;
  case 0x1100016:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfff7ffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x80000;
    }
    break;
  case 0x1100017:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffefffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x100000;
    }
    break;
  case 0x1100018:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffdfffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x200000;
    }
    break;
  case 0x1100019:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xffbfffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x400000;
    }
    break;
  case 0x1100020:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xff7fffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x800000;
    }
    break;
  case 0x1100021:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfeffffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x1000000;
    }
    break;
  case 0x1100022:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfdffffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x2000000;
    }
    break;
  case 0x1100023:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xfbffffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x4000000;
    }
    break;
  case 0x1100024:
    if (in_r2 == 0) {
      uVar1 = *(uint *)(param_1 + 0x420) & 0xf7ffffff;
    }
    else {
      uVar1 = *(uint *)(param_1 + 0x420) | 0x8000000;
    }
    break;
  default:
    if (uVar1 == 0xde1) {
      if (in_r2 == 0) {
        uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffffe;
      }
      else {
        uVar1 = *(uint *)(param_1 + 0x420) | 1;
      }
    }
    else {
      if (uVar1 != 0x1000000) {
        return;
      }
      if (in_r2 == 0) {
        uVar1 = *(uint *)(param_1 + 0x420) & 0xfffffffd;
      }
      else {
        uVar1 = *(uint *)(param_1 + 0x420) | 2;
      }
    }
  }
  *(uint *)(param_1 + 0x420) = uVar1;
switchD_00094d90_caseD_110000a:
  return;
}

```

## target disasm
```
  00094d7c: ldr r3,[0x00094ff8]
  00094d7e: add r3,pc
  00094d80: ldr r3,[r3,#0x0]
  00094d82: ldrb r3,[r3,#0x0]
  00094d84: cmp r3,#0x0
  00094d86: beq 0x00094df0
  00094d88: sub.w r3,r1,#0x1100000
  00094d8c: cmp r3,#0x24
  00094d8e: bhi 0x00094e0e
  00094d90: tbh [pc,r3]
  00094dde: ldr.w r1,[r0,#0x420]
  00094de2: cmp r2,#0x0
  00094de4: ite eq
  00094de6: bic.eq r1,r1,#0x4
  00094dea: orr.ne r1,r1,#0x4
  00094dee: b 0x00094ff2
  00094df0: lsrs r0,r1,#0x14
  00094df2: cmp r0,#0x10
  00094df4: it hi
  00094df6: bx.hi lr
  00094df8: cmp.w r1,#0x1000000
  00094dfc: it eq
  00094dfe: mov.eq.w r1,#0xbc0
  00094e02: mov r0,r1
  00094e04: cbz r2,0x00094e0a
  00094e06: b.w 0x001ab628
  00094e0a: b.w 0x001ab388
  00094e0e: movw r3,#0xde1
  00094e12: cmp r1,r3
  00094e14: beq.w 0x00094fe2
  00094e18: cmp.w r1,#0x1000000
  00094e1c: bne.w 0x00094ff6
  00094e20: ldr.w r1,[r0,#0x420]
  00094e24: cmp r2,#0x0
  00094e26: ite eq
  00094e28: bic.eq r1,r1,#0x2
  00094e2c: orr.ne r1,r1,#0x2
  00094e30: b 0x00094ff2
  00094e32: ldr.w r1,[r0,#0x420]
  00094e36: cmp r2,#0x0
  00094e38: ite eq
  00094e3a: bic.eq r1,r1,#0x8
  00094e3e: orr.ne r1,r1,#0x8
  00094e42: b 0x00094ff2
  00094e44: ldr.w r1,[r0,#0x420]
  00094e48: cmp r2,#0x0
  00094e4a: ite eq
  00094e4c: bic.eq r1,r1,#0x10
  00094e50: orr.ne r1,r1,#0x10
  00094e54: b 0x00094ff2
  00094e56: ldr.w r1,[r0,#0x420]
  00094e5a: cmp r2,#0x0
  00094e5c: ite eq
  00094e5e: bic.eq r1,r1,#0x20
  00094e62: orr.ne r1,r1,#0x20
  00094e66: b 0x00094ff2
  00094e68: ldr.w r1,[r0,#0x420]
  00094e6c: cmp r2,#0x0
  00094e6e: ite eq
  00094e70: bic.eq r1,r1,#0x80
  00094e74: orr.ne r1,r1,#0x80
  00094e78: b 0x00094ff2
  00094e7a: ldr.w r1,[r0,#0x420]
  00094e7e: cmp r2,#0x0
  00094e80: ite eq
  00094e82: bic.eq r1,r1,#0x40
  00094e86: orr.ne r1,r1,#0x40
  00094e8a: b 0x00094ff2
  00094e8c: ldr.w r1,[r0,#0x420]
  00094e90: cmp r2,#0x0
  00094e92: ite eq
  00094e94: bic.eq r1,r1,#0x100
  00094e98: orr.ne r1,r1,#0x100
  00094e9c: b 0x00094ff2
  00094e9e: ldr.w r1,[r0,#0x420]
  00094ea2: cmp r2,#0x0
  00094ea4: ite eq
  00094ea6: bic.eq r1,r1,#0x200
  00094eaa: orr.ne r1,r1,#0x200
  00094eae: b 0x00094ff2
  00094eb0: ldr.w r1,[r0,#0x420]
  00094eb4: cmp r2,#0x0
  00094eb6: ite eq
  00094eb8: bic.eq r1,r1,#0x400
  00094ebc: orr.ne r1,r1,#0x400
  00094ec0: b 0x00094ff2
  00094ec2: ldr.w r1,[r0,#0x420]
  00094ec6: cmp r2,#0x0
  00094ec8: ite eq
  00094eca: bic.eq r1,r1,#0x1000
  00094ece: orr.ne r1,r1,#0x1000
  00094ed2: b 0x00094ff2
  00094ed4: ldr.w r1,[r0,#0x420]
  00094ed8: cmp r2,#0x0
  00094eda: ite eq
  00094edc: bic.eq r1,r1,#0x2000
  00094ee0: orr.ne r1,r1,#0x2000
  00094ee4: b 0x00094ff2
  00094ee6: ldr.w r1,[r0,#0x420]
  00094eea: cmp r2,#0x0
  00094eec: ite eq
  00094eee: bic.eq r1,r1,#0x4000
  00094ef2: orr.ne r1,r1,#0x4000
  00094ef6: b 0x00094ff2
  00094ef8: ldr.w r1,[r0,#0x420]
  00094efc: cmp r2,#0x0
  00094efe: ite eq
  00094f00: bic.eq r1,r1,#0x8000
  00094f04: orr.ne r1,r1,#0x8000
  00094f08: b 0x00094ff2
  00094f0a: ldr.w r1,[r0,#0x420]
  00094f0e: cmp r2,#0x0
  00094f10: ite eq
  00094f12: bic.eq r1,r1,#0x10000
  00094f16: orr.ne r1,r1,#0x10000
  00094f1a: b 0x00094ff2
  00094f1c: ldr.w r1,[r0,#0x420]
  00094f20: cmp r2,#0x0
  00094f22: ite eq
  00094f24: bic.eq r1,r1,#0x20000
  00094f28: orr.ne r1,r1,#0x20000
  00094f2c: b 0x00094ff2
  00094f2e: ldr.w r1,[r0,#0x420]
  00094f32: cmp r2,#0x0
  00094f34: ite eq
  00094f36: bic.eq r1,r1,#0x40000
  00094f3a: orr.ne r1,r1,#0x40000
  00094f3e: b 0x00094ff2
  00094f40: ldr.w r1,[r0,#0x420]
  00094f44: cmp r2,#0x0
  00094f46: ite eq
  00094f48: bic.eq r1,r1,#0x80000
  00094f4c: orr.ne r1,r1,#0x80000
  00094f50: b 0x00094ff2
  00094f52: ldr.w r1,[r0,#0x420]
  00094f56: cmp r2,#0x0
  00094f58: ite eq
  00094f5a: bic.eq r1,r1,#0x100000
  00094f5e: orr.ne r1,r1,#0x100000
  00094f62: b 0x00094ff2
  00094f64: ldr.w r1,[r0,#0x420]
  00094f68: cmp r2,#0x0
  00094f6a: ite eq
  00094f6c: bic.eq r1,r1,#0x200000
  00094f70: orr.ne r1,r1,#0x200000
  00094f74: b 0x00094ff2
  00094f76: ldr.w r1,[r0,#0x420]
  00094f7a: cmp r2,#0x0
  00094f7c: ite eq
  00094f7e: bic.eq r1,r1,#0x400000
  00094f82: orr.ne r1,r1,#0x400000
  00094f86: b 0x00094ff2
  00094f88: ldr.w r1,[r0,#0x420]
  00094f8c: cmp r2,#0x0
  00094f8e: ite eq
  00094f90: bic.eq r1,r1,#0x800000
  00094f94: orr.ne r1,r1,#0x800000
  00094f98: b 0x00094ff2
  00094f9a: ldr.w r1,[r0,#0x420]
  00094f9e: cmp r2,#0x0
  00094fa0: ite eq
  00094fa2: bic.eq r1,r1,#0x1000000
  00094fa6: orr.ne r1,r1,#0x1000000
  00094faa: b 0x00094ff2
  00094fac: ldr.w r1,[r0,#0x420]
  00094fb0: cmp r2,#0x0
  00094fb2: ite eq
  00094fb4: bic.eq r1,r1,#0x2000000
  00094fb8: orr.ne r1,r1,#0x2000000
  00094fbc: b 0x00094ff2
  00094fbe: ldr.w r1,[r0,#0x420]
  00094fc2: cmp r2,#0x0
  00094fc4: ite eq
  00094fc6: bic.eq r1,r1,#0x4000000
  00094fca: orr.ne r1,r1,#0x4000000
  00094fce: b 0x00094ff2
  00094fd0: ldr.w r1,[r0,#0x420]
  00094fd4: cmp r2,#0x0
  00094fd6: ite eq
  00094fd8: bic.eq r1,r1,#0x8000000
  00094fdc: orr.ne r1,r1,#0x8000000
  00094fe0: b 0x00094ff2
  00094fe2: ldr.w r1,[r0,#0x420]
  00094fe6: cmp r2,#0x0
  00094fe8: ite eq
  00094fea: bic.eq r1,r1,#0x1
  00094fee: orr.ne r1,r1,#0x1
  00094ff2: str.w r1,[r0,#0x420]
  00094ff6: bx lr
  001ab388: bx pc
  001ab628: bx pc
```
