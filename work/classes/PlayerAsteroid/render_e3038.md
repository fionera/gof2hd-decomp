# PlayerAsteroid::render
elf 0xe3038 body 54
Sig: undefined __thiscall render(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::render() */

void __thiscall PlayerAsteroid::render(PlayerAsteroid *this)

{
  if (this[0xf5] != (PlayerAsteroid)0x0) {
    if (*(int *)(this + 0x78) != 0) {
      AEGeometry::render();
    }
    if (*(int *)(this + 0x88) == 0) {
      (*(code *)(DAT_001abdd4 + 0x1abdd8))(*(undefined4 *)(this + 8));
      return;
    }
    if (*(int *)(this + 0x88) == 3) {
      (*(code *)(DAT_001ac2b4 + 0x1ac2b8))(*(undefined4 *)(this + 300));
      return;
    }
  }
  return;
}

```

## target disasm
```
  000f3038: push {r4,r6,r7,lr}
  000f303a: add r7,sp,#0x8
  000f303c: mov r4,r0
  000f303e: ldrb.w r0,[r0,#0xf5]
  000f3042: cbz r0,0x000f3062
  000f3044: ldr r0,[r4,#0x78]
  000f3046: cbz r0,0x000f304c
  000f3048: blx 0x00072238
  000f304c: ldr.w r0,[r4,#0x88]
  000f3050: cbz r0,0x000f3064
  000f3052: cmp r0,#0x3
  000f3054: bne 0x000f3062
  000f3056: ldr.w r0,[r4,#0x12c]
  000f305a: pop.w {r4,r6,r7,lr}
  000f305e: b.w 0x001ac2a8
  000f3062: pop {r4,r6,r7,pc}
  000f3064: ldr r0,[r4,#0x8]
  000f3066: pop.w {r4,r6,r7,lr}
  000f306a: b.w 0x001abdc8
```
