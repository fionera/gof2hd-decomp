# Status::getGammaRayDamagePerSecond
elf 0xacdc0 body 74
Sig: undefined __stdcall getGammaRayDamagePerSecond(int param_1, int param_2)

## decompile
```c

/* Status::getGammaRayDamagePerSecond(int, int) */

undefined4 Status::getGammaRayDamagePerSecond(int param_1,int param_2)

{
  int in_r2;
  uint uVar1;
  undefined4 uVar2;
  
  uVar1 = param_2 - 0x6d;
  uVar2 = DAT_000bce0c;
  if (uVar1 < 5) {
    if (in_r2 < 0x6a) {
      if (uVar1 < 5) {
        return *(undefined4 *)(DAT_000bce10 + 0xbcdda + uVar1 * 4);
      }
    }
    else if (*(int *)(param_1 + 0x1e8) < 0x9e) {
      if (uVar1 < 5) {
        return *(undefined4 *)(DAT_000bce14 + 0xbcdf0 + uVar1 * 4);
      }
    }
    else if (param_2 == 0x6d) {
      uVar2 = 0x3f800000;
    }
  }
  return uVar2;
}

```
## target disasm
```
  000bcdc0: sub.w r3,r1,#0x6d
  000bcdc4: vldr.32 s0,[pc,#0x44]
  000bcdc8: cmp r3,#0x4
  000bcdca: bhi 0x000bce04
  000bcdcc: cmp r2,#0x69
  000bcdce: bgt 0x000bcdde
  000bcdd0: cmp r3,#0x5
  000bcdd2: bcs 0x000bce04
  000bcdd4: ldr r0,[0x000bce10]
  000bcdd6: add r0,pc
  000bcdd8: ldr.w r0,[r0,r3,lsl #0x2]
  000bcddc: bx lr
  000bcdde: ldr.w r0,[r0,#0x1e8]
  000bcde2: cmp r0,#0x9d
  000bcde4: bgt 0x000bcdf4
  000bcde6: cmp r3,#0x5
  000bcde8: bcs 0x000bce04
  000bcdea: ldr r0,[0x000bce14]
  000bcdec: add r0,pc
  000bcdee: ldr.w r0,[r0,r3,lsl #0x2]
  000bcdf2: bx lr
  000bcdf4: vmov.f32 s2,0x3f800000
  000bcdf8: vldr.32 s0,[pc,#0x10]
  000bcdfc: cmp r1,#0x6d
  000bcdfe: it eq
  000bce00: vmov.eq.f32 s0,s2
  000bce04: vmov r0,s0
  000bce08: bx lr
```
