# AbyssEngine::TransformRelease deferred reconstruction

Target: `_ZN11AbyssEngine16TransformReleaseEPNS_6EngineEPPNS_9TransformE`

- Original: `0x6c314`, 78 bytes.
- Current: 78 bytes, 97.1% structural match.
- TU: `src/engine/core/AbyssEngine.cpp`.
- Verified `Transform` arrays: meshes count/data at `+0x3c/+0x40`;
  children count/data at `+0x4c/+0x50`.

The sole executable mismatch is:

```text
original                         current
ldr r0, [r1]                     ldr r0, [r1]
mov r4, r1                       cbz r0, <return>
cbz r0, <return>                 mov r4, r1
```

The `slot` pointer saved in `r4` is read only on the non-null path, where it
reloads `*slot` after recursive release calls. It is never live on the null
edge. Pointer aliases, references, assignment-in-condition, and branch
inversion all reduce to equivalent SSA and remain valid MachineSink
candidates.

A pre-branch requirement would need an artificial null-edge use, volatile
storage, assembly, or another compiler barrier, all of which are prohibited
or semantically wrong. Treat this as compiler/TU scheduling until new source
evidence appears.
