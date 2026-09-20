# Smoke tests

Requires VS 2026 with C++ tools, a **Debug x64** solution build, and OpenGL 4.5 with 32 fragment texture units. Run from the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tests\Run-SmokeTests.ps1
```

The runner detects VS 2026 (including Insiders), or accepts `-VisualStudioPath`. Generated files go to `bin/SmokeTests`.

Checks cover scene roundtrips and errors, existing example scenes, texture batching, framebuffer picking/resizing, and layer/renderer cleanup. Graphics checks use a hidden window. Invalid scene fixtures intentionally log errors; failed checks return a nonzero exit code. Verify editor UI interactions manually.
