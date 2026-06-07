# MissionsWindow::render3D
elf 0x1504b8 body 36
Sig: undefined __stdcall render3D(void)

## decompile
```c

/* MissionsWindow::render3D() */

void MissionsWindow::render3D(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x40) == 1) {
    WantedWindow::render3D(*(WantedWindow **)(in_r0 + 0x10));
  }
  if (*(char *)(in_r0 + 0x22) != '\0') {
    (*(code *)(DAT_001ac264 + 0x1ac268))(*(undefined4 *)(in_r0 + 8));
    return;
  }
  return;
}

```

## target disasm
```
  001604b8: push {r4,r6,r7,lr}
  001604ba: add r7,sp,#0x8
  001604bc: mov r4,r0
  001604be: ldr r0,[r0,#0x40]
  001604c0: cmp r0,#0x1
  001604c2: itt eq
  001604c4: ldr.eq r0,[r4,#0x10]
  001604c6: blx.eq 0x000772b4
  001604ca: ldrb.w r0,[r4,#0x22]
  001604ce: cbz r0,0x001604da
  001604d0: ldr r0,[r4,#0x8]
  001604d2: pop.w {r4,r6,r7,lr}
  001604d6: b.w 0x001ac258
  001604da: pop {r4,r6,r7,pc}
```
