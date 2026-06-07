# PlayerJumpgate::update
elf 0xa51f0 body 42
Sig: undefined __thiscall update(PlayerJumpgate * this, int param_1)

## decompile
```c

/* PlayerJumpgate::update(int) */

void __thiscall PlayerJumpgate::update(PlayerJumpgate *this,int param_1)

{
  undefined4 uVar1;
  
  if (this[0xf5] != (PlayerJumpgate)0x0) {
    uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000b521c + 0xb5204));
    AbyssEngine::Transform::Update(CONCAT44(1,uVar1),SUB41(param_1,0));
  }
  return;
}

```

## target disasm
```
  000b51f0: push {r2,r3,r4,r6,r7,lr}
  000b51f2: add r7,sp,#0x10
  000b51f4: mov r4,r1
  000b51f6: ldrb.w r1,[r0,#0xf5]
  000b51fa: cbz r1,0x000b5218
  000b51fc: ldr r1,[0x000b521c]
  000b51fe: ldr r0,[r0,#0x8]
  000b5200: add r1,pc
  000b5202: ldr r2,[r1,#0x0]
  000b5204: ldr r1,[r0,#0xc]
  000b5206: ldr r0,[r2,#0x0]
  000b5208: blx 0x00072088
  000b520c: movs r1,#0x1
  000b520e: asrs r3,r4,#0x1f
  000b5210: mov r2,r4
  000b5212: str r1,[sp,#0x0]
  000b5214: blx 0x0006f7cc
  000b5218: pop {r2,r3,r4,r6,r7,pc}
```
