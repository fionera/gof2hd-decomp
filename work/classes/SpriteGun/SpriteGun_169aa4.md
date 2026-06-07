# SpriteGun::SpriteGun
elf 0x169aa4 body 16
Sig: undefined __thiscall SpriteGun(SpriteGun * this, Gun * param_1, int param_2)

## decompile
```c

/* SpriteGun::SpriteGun(Gun*, int) */

void __thiscall SpriteGun::SpriteGun(SpriteGun *this,Gun *param_1,int param_2)

{
  *(int *)this = *(int *)(DAT_00179ab4 + 0x179aac) + 8;
  *(undefined4 *)(this + 4) = 0;
  return;
}

```

## target disasm
```
  00179aa4: ldr r1,[0x00179ab4]
  00179aa6: movs r2,#0x0
  00179aa8: add r1,pc
  00179aaa: ldr r1,[r1,#0x0]
  00179aac: adds r1,#0x8
  00179aae: strd r1,r2,[r0,#0x0]
  00179ab2: bx lr
```
