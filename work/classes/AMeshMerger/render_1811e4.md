# AMeshMerger::render
elf 0x1811e4 body 12
Sig: undefined __thiscall render(AMeshMerger * this)

## decompile
```c

/* AMeshMerger::render() */

void __thiscall AMeshMerger::render(AMeshMerger *this)

{
  (*(code *)(DAT_001abda4 + 0x1abda8))(*(undefined4 *)(this + 0x14),*(undefined4 *)(this + 0x1c),0);
  return;
}

```

## target disasm
```
  001911e4: ldr r2,[r0,#0x14]
  001911e6: ldr r1,[r0,#0x1c]
  001911e8: mov r0,r2
  001911ea: movs r2,#0x0
  001911ec: b.w 0x001abd98
```
