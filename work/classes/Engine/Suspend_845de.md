# Engine::Suspend
elf 0x845de body 16
Sig: undefined __thiscall Suspend(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::Suspend() */

undefined4 __thiscall AbyssEngine::Engine::Suspend(Engine *this)

{
  PaintCanvas::Suspend((PaintCanvas *)**(undefined4 **)(this + 0x30));
  return 1;
}

```

## target disasm
```
  000945de: push {r7,lr}
  000945e0: mov r7,sp
  000945e2: ldr r0,[r0,#0x30]
  000945e4: ldr r0,[r0,#0x0]
  000945e6: blx 0x00070540
  000945ea: movs r0,#0x1
  000945ec: pop {r7,pc}
```
