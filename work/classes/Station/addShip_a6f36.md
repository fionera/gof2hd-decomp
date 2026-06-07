# Station::addShip
elf 0xa6f36 body 98
Sig: undefined __stdcall addShip(Ship * param_1)

## decompile
```c

/* Station::addShip(Ship*) */

void Station::addShip(Ship *param_1)

{
  uint uVar1;
  int iVar2;
  Ship *in_r1;
  uint uVar3;
  Array<Ship*> *this;
  
  this = *(Array<Ship*> **)(param_1 + 0x2c);
  if (this == (Array<Ship*> *)0x0) {
    this = operator_new(0xc);
    Array<Ship*>::Array(this);
    *(Array<Ship*> **)(param_1 + 0x2c) = this;
  }
  else {
    uVar1 = *(uint *)this;
    if (uVar1 != 0) {
      for (uVar3 = 0; uVar3 < uVar1; uVar3 = uVar3 + 1) {
        iVar2 = Ship::equals(*(Ship **)(*(uint *)(this + 4) + uVar3 * 4),in_r1);
        if (iVar2 != 0) {
          if (-1 < (int)uVar3) {
            return;
          }
          this = *(Array<Ship*> **)(param_1 + 0x2c);
          break;
        }
        this = *(Array<Ship*> **)(param_1 + 0x2c);
        uVar1 = *(uint *)this;
      }
    }
  }
  (*(code *)(DAT_001abee4 + 0x1abee8))(in_r1,this);
  return;
}

```

## target disasm
```
  000b6f36: push {r4,r5,r6,r7,lr}
  000b6f38: add r7,sp,#0xc
  000b6f3a: push.w r8
  000b6f3e: ldr r6,[r0,#0x2c]
  000b6f40: mov r8,r1
  000b6f42: mov r5,r0
  000b6f44: cbz r6,0x000b6f68
  000b6f46: ldr r0,[r6,#0x0]
  000b6f48: cbz r0,0x000b6f86
  000b6f4a: movs r4,#0x0
  000b6f4c: b 0x000b6f62
  000b6f4e: ldr r0,[r6,#0x4]
  000b6f50: mov r1,r8
  000b6f52: ldr.w r0,[r0,r4,lsl #0x2]
  000b6f56: blx 0x000734ec
  000b6f5a: cbnz r0,0x000b6f78
  000b6f5c: ldr r6,[r5,#0x2c]
  000b6f5e: adds r4,#0x1
  000b6f60: ldr r0,[r6,#0x0]
  000b6f62: cmp r4,r0
  000b6f64: bcc 0x000b6f4e
  000b6f66: b 0x000b6f86
  000b6f68: movs r0,#0xc
  000b6f6a: blx 0x0006eb24
  000b6f6e: mov r6,r0
  000b6f70: blx 0x000719b0
  000b6f74: str r6,[r5,#0x2c]
  000b6f76: b 0x000b6f86
  000b6f78: cmp.w r4,#0xffffffff
  000b6f7c: itt gt
  000b6f7e: pop.gt.w r8
  000b6f82: pop.gt {r4,r5,r6,r7,pc}
  000b6f84: ldr r6,[r5,#0x2c]
  000b6f86: mov r0,r8
  000b6f88: mov r1,r6
  000b6f8a: pop.w r8
  000b6f8e: pop.w {r4,r5,r6,r7,lr}
  000b6f92: b.w 0x001abed8
  001abed8: bx pc
```
