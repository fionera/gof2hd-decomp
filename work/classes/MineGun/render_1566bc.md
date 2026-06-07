# MineGun::render
elf 0x1566bc body 46
Sig: undefined __thiscall render(MineGun * this)

## decompile
```c

/* MineGun::render() */

void __thiscall MineGun::render(MineGun *this)

{
  uint uVar1;
  
  ObjectGun::render((ObjectGun *)this);
  for (uVar1 = 0; uVar1 < *(uint *)(*(int *)(this + 8) + 8); uVar1 = uVar1 + 1) {
    if (*(char *)(*(int *)(*(int *)(this + 8) + 0x40) + uVar1) != '\0') {
      Explosion::render();
    }
  }
  return;
}

```

## target disasm
```
  001666bc: push {r4,r5,r7,lr}
  001666be: add r7,sp,#0x8
  001666c0: mov r4,r0
  001666c2: blx 0x000775f0
  001666c6: movs r5,#0x0
  001666c8: b 0x001666e0
  001666ca: ldr r0,[r0,#0x40]
  001666cc: ldrb r0,[r0,r5]
  001666ce: cbz r0,0x001666de
  001666d0: ldr.w r0,[r4,#0xb4]
  001666d4: ldr r0,[r0,#0x4]
  001666d6: ldr.w r0,[r0,r5,lsl #0x2]
  001666da: blx 0x00072d18
  001666de: adds r5,#0x1
  001666e0: ldr r0,[r4,#0x8]
  001666e2: ldr r1,[r0,#0x8]
  001666e4: cmp r5,r1
  001666e6: bcc 0x001666ca
  001666e8: pop {r4,r5,r7,pc}
```
