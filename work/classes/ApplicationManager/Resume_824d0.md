# ApplicationManager::Resume
elf 0x824d0 body 50
Sig: undefined __stdcall Resume(bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::Resume(bool) */

void AbyssEngine::ApplicationManager::Resume(bool param_1)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  if (*(int *)(uVar1 + 0x3c) != 3) {
    return;
  }
  if (*(int **)(uVar1 + 0x18) != (int *)0x0) {
    (**(code **)(**(int **)(uVar1 + 0x18) + 0x40))();
    if (*(Engine **)(uVar1 + 0xa8) != (Engine *)0x0) {
      Engine::Resume(*(Engine **)(uVar1 + 0xa8));
    }
    *(undefined4 *)(uVar1 + 0xa0) = 0;
    *(undefined4 *)(uVar1 + 0xa4) = 0;
    *(undefined4 *)(uVar1 + 0x80) = 0;
    *(undefined4 *)(uVar1 + 0x84) = 0;
    *(undefined4 *)(uVar1 + 0x3c) = 4;
  }
  return;
}

```

## target disasm
```
  000924d0: push {r4,r6,r7,lr}
  000924d2: add r7,sp,#0x8
  000924d4: mov r4,r0
  000924d6: ldr r0,[r0,#0x3c]
  000924d8: cmp r0,#0x3
  000924da: it ne
  000924dc: pop.ne {r4,r6,r7,pc}
  000924de: ldr r0,[r4,#0x18]
  000924e0: cbz r0,0x00092500
  000924e2: ldr r1,[r0,#0x0]
  000924e4: ldr r1,[r1,#0x40]
  000924e6: blx r1
  000924e8: ldr.w r0,[r4,#0xa8]
  000924ec: cbz r0,0x000924f2
  000924ee: blx 0x000703d8
  000924f2: movs r0,#0x0
  000924f4: strd r0,r0,[r4,#0xa0]
  000924f8: strd r0,r0,[r4,#0x80]
  000924fc: movs r0,#0x4
  000924fe: str r0,[r4,#0x3c]
  00092500: pop {r4,r6,r7,pc}
```
