# PlayerFighter::awake
elf 0xdff2c body 48
Sig: undefined __thiscall awake(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::awake() */

void __thiscall PlayerFighter::awake(PlayerFighter *this)

{
  int iVar1;
  
  *(undefined4 *)(this + 0x88) = 1;
  Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
  setExhaustVisible(SUB41(this,0));
  iVar1 = *(int *)(this + 0xc);
  this[0xf5] = (PlayerFighter)0x1;
  if (iVar1 == 0) {
    iVar1 = *(int *)(this + 8);
  }
  (*(code *)(DAT_001abde4 + 0x1abde8))(iVar1,1);
  return;
}

```

## target disasm
```
  000eff2c: push {r4,r5,r7,lr}
  000eff2e: add r7,sp,#0x8
  000eff30: mov r4,r0
  000eff32: ldr r0,[r0,#0x4]
  000eff34: movs r5,#0x1
  000eff36: movs r1,#0x1
  000eff38: str.w r5,[r4,#0x88]
  000eff3c: blx 0x00072580
  000eff40: mov r0,r4
  000eff42: movs r1,#0x1
  000eff44: blx 0x000721f0
  000eff48: ldr r0,[r4,#0xc]
  000eff4a: strb.w r5,[r4,#0xf5]
  000eff4e: cbnz r0,0x000eff52
  000eff50: ldr r0,[r4,#0x8]
  000eff52: movs r1,#0x1
  000eff54: pop.w {r4,r5,r7,lr}
  000eff58: b.w 0x001abdd8
```
