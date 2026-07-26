# `PaintCanvas::CameraCreate` constructor-ABI deferral

## Target

- TU: `src/engine/render/PaintCanvas.cpp`
- `_ZN11AbyssEngine11PaintCanvas12CameraCreateERj`
- original `0x7b6a8`, current `0x92c50`
- 97.6%, 124/124 bytes

## Residual

The body and exception cleanup are structurally identical except the Camera
constructor alias:

```text
original 0x7b6f0  blx Camera::Camera(float,float,float,float,float) C2
current            blx Camera::Camera(float,float,float,float,float) C1
```

The original PLT entry at `0x5fd6c` resolves to a weak C2-only constructor at
`0x7b724`. The current PLT entry at `0x66d60` resolves to the strong C1/C2
alias at `0x92ccc`. Both calls pass `8000.0f`, `256.0f`, `16384.0f`, the
floating canvas width, and the floating canvas height. Both landing pads
delete the allocated Camera storage and resume unwinding; neither calls a
destructor after failed construction.

An ordinary `new Camera(...)` must request complete-object construction and
therefore selects C1 with the current strong out-of-line definition. A local
PaintCanvas spelling cannot legally select C2.

## Requeue condition

Requeue only as an atomic Camera header/source ownership pass after draining
the fleet: confirm and restore the constructor's inline/weak provenance,
remove the strong out-of-line definition, and gate constructor exports,
`CameraCreate`, and symbol parity. The Camera constructor itself also
constructs a Matrix and retains the Matrix C2/C1 mismatch documented in
`AEMath-MatrixGetInverse.md`, so coordinate the two ownership passes.

Only `CameraCreate` directly constructs Camera in the current tree, but the
export and nested Matrix blast radius make this unsuitable for an isolated
function patch. Confidence is high for the ABI and call evidence and
medium-high for inline-header provenance.
