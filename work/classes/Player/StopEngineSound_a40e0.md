# Player::StopEngineSound
elf 0xa40e0 body 36
Sig: undefined __stdcall StopEngineSound(void)

## decompile
```c

/* Player::StopEngineSound() */

void Player::StopEngineSound(void)

{
  int in_r0;
  
  if (*(Event **)(in_r0 + 0xf0) != (Event *)0x0) {
    FModSound::stop((FModSound *)**(undefined4 **)(DAT_000b4104 + 0xb40f2),*(Event **)(in_r0 + 0xf0)
                   );
    *(undefined1 *)(in_r0 + 0x108) = 0;
    *(undefined4 *)(in_r0 + 0xf0) = 0;
  }
  return;
}

```
## target disasm
```
  000b40e0: push {r4,r6,r7,lr}
  000b40e2: add r7,sp,#0x8
  000b40e4: ldr.w r1,[r0,#0xf0]
  000b40e8: mov r4,r0
  000b40ea: cbz r1,0x000b4102
  000b40ec: ldr r0,[0x000b4104]
  000b40ee: add r0,pc
  000b40f0: ldr r0,[r0,#0x0]
  000b40f2: ldr r0,[r0,#0x0]
  000b40f4: blx 0x00073018
  000b40f8: movs r0,#0x0
  000b40fa: strb.w r0,[r4,#0x108]
  000b40fe: str.w r0,[r4,#0xf0]
  000b4102: pop {r4,r6,r7,pc}
```
