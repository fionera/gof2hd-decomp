# Engine::GetDeviceInfo
elf 0x6ddd6 body 26
Sig: undefined __thiscall GetDeviceInfo(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::GetDeviceInfo() */

void __thiscall AbyssEngine::Engine::GetDeviceInfo(Engine *this)

{
  Engine EVar1;
  undefined4 uVar2;
  
  EVar1 = (Engine)NFC::isPad();
  this[8] = EVar1;
  uVar2 = NFC::getWidth();
  *(undefined4 *)this = uVar2;
  uVar2 = NFC::getHeight();
  *(undefined4 *)(this + 4) = uVar2;
  return;
}

```

## target disasm
```
  0007ddd6: push {r4,r6,r7,lr}
  0007ddd8: add r7,sp,#0x8
  0007ddda: mov r4,r0
  0007dddc: blx 0x0006eee4
  0007dde0: strb r0,[r4,#0x8]
  0007dde2: blx 0x0006eef0
  0007dde6: str r0,[r4,#0x0]
  0007dde8: blx 0x0006eefc
  0007ddec: str r0,[r4,#0x4]
  0007ddee: pop {r4,r6,r7,pc}
```
