# MeshMerger::render
elf 0x17406c body 12
Sig: undefined __thiscall render(MeshMerger * this)

## decompile
```c

/* MeshMerger::render() */

void __thiscall MeshMerger::render(MeshMerger *this)

{
  (*(code *)(DAT_001abda4 + 0x1abda8))(*(undefined4 *)(this + 0xc),*(undefined4 *)(this + 0x14),0);
  return;
}

```

## target disasm
```
  0018406c: ldr r2,[r0,#0xc]
  0018406e: ldr r1,[r0,#0x14]
  00184070: mov r0,r2
  00184072: movs r2,#0x0
  00184074: b.w 0x001abd98
```
