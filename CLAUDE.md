# YR_RN_Mission_Editor — Project Guidance

## Build System

- MSVC 2022 (v143 toolset), C++20 (`stdcpp20`), MultiByte charset
- Solution: `MissionEditor.sln`
- Main app: `MissionEditor/MissionEditor.vcxproj` — MFC-based, dynamic MFC linking (`_AFXDLL`)
- Static lib: `MissionEditorPackLib/MissionEditorPackLib.vcxproj` — no MFC dependency
- Unit tests: `UnitTest/UnitTest.vcxproj` — GoogleTest
- Build output: `build/output/$(Configuration)-$(Platform)/`
- Build intermediates: `build/intermediate/$(Configuration)-$(Platform)/$(ProjectName)/`
- Property sheets: `MissionEditor/PropertySheets/Common.props`
- **Default build configuration**: `FinalAlertDebug YR|x64` (use unless otherwise specified)

## Plans

- Always stored in `docs/plans/<plan-name>.md` inside the project directory
- Never write plans to `~/.claude/plans/`

## Coding Style

### Bracing

Opening brace on the **same line** as the control-flow keyword. Never single-line clauses (always use braces).

```cpp
// Correct
if (condition) {
    doThing();
}

for (auto const& item : items) {
    process(item);
}

// Wrong
if (condition) doThing();       // no braces
if (condition)                  // brace on next line
{
    doThing();
}
```

### Namespacing

- New non-MFC code in `DB` namespace or project-specific namespace
- Legacy MFC code in global namespace with Hungarian notation
- New DLL code uses its own namespaces, not global

### Headers

- `#pragma once` (modern files) preferred over `#ifndef` guards (legacy files)
- Precompiled header: `stdafx.h` (used in MissionEditor project)

### C++ Features

- C++20 features in use: `concepts`, `requires`, `std::span`, `std::endian`, `std::ranges`, `constexpr`/`consteval`
- `auto` return types and trailing return types in modern code
- Range-based for loops preferred
