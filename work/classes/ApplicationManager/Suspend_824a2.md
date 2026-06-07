# ApplicationManager::Suspend
elf 0x824a2 body 46
Sig: undefined __thiscall Suspend(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::Suspend() */

void __thiscall AbyssEngine::ApplicationManager::Suspend(ApplicationManager *this)

{
  undefined4 uVar1;
  
  if (*(int *)(this + 0x3c) - 3U < 2) {
    return;
  }
  if (*(int **)(this + 0x18) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x18) + 0x3c))();
    if (*(Engine **)(this + 0xa8) != (Engine *)0x0) {
      Engine::Suspend(*(Engine **)(this + 0xa8));
    }
    uVar1 = *(undefined4 *)(this + 0x3c);
    *(undefined4 *)(this + 0x3c) = 3;
    *(undefined4 *)(this + 0x40) = uVar1;
  }
  return;
}

```

## target disasm
```
  000924a2: push {r4,r6,r7,lr}
  000924a4: add r7,sp,#0x8
  000924a6: mov r4,r0
  000924a8: ldr r0,[r0,#0x3c]
  000924aa: subs r0,#0x3
  000924ac: cmp r0,#0x2
  000924ae: it cc
  000924b0: pop.cc {r4,r6,r7,pc}
  000924b2: ldr r0,[r4,#0x18]
  000924b4: cbz r0,0x000924ce
  000924b6: ldr r1,[r0,#0x0]
  000924b8: ldr r1,[r1,#0x3c]
  000924ba: blx r1
  000924bc: ldr.w r0,[r4,#0xa8]
  000924c0: cbz r0,0x000924c6
  000924c2: blx 0x000703cc
  000924c6: ldr r0,[r4,#0x3c]
  000924c8: movs r1,#0x3
  000924ca: strd r1,r0,[r4,#0x3c]
  000924ce: pop {r4,r6,r7,pc}
```
