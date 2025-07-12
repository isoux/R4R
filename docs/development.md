# 🧰 Development and Toolchain Overview

The R4R operating system is developed from scratch using the **GCC compiler** with **inline Intel-style assembly**, targeting **32-bit i386/i486 processors**. The core intent is to maintain full control over hardware behavior, particularly across all four x86 protection rings.

### Toolchain

- **Compiler:** GCC (with support for Intel `__asm__` syntax)
- **Assembler:** Inline assembly (Intel syntax only), no external `.S` or GAS-based files
- **Bootloaders:** GRUB v1 (`img` build) and GRUB v2 (`iso` build)
- **Emulators:** BOCHS (recommended with GUI debugger), QEMU (partially reliable)
- **Target hardware:** Real Intel i486+ CPUs (system is tested on physical machines)

### Design Principles

- Core components are written in **C** and **inline assembly**, where hardware-level control (like privilege transitions, task state manipulation, or descriptor configuration) is required.
- The system architecture emphasizes **tasks as hardware-managed objects**, with extensive use of **call gates** for controlled transitions across rings.
- Code is highly **non-portable** and intentionally focused on leveraging underused features of x86 CPUs, such as hardware task switching and segment-based protection.

### Build System

The build system supports two main configurations:

| Target | Output | Boot Method |
|--------|--------|-------------|
| `img`  | `.img` floppy image | GRUB v1 |
| `iso`  | `.iso` CD image     | GRUB v2 |

To switch between targets, modify the `BOOT_WAY` variable in `defines.mk`.

Use the following make targets:

```sh
make clean_all_objs   # recommended before each full build
make img              # build floppy-based GRUB1 image
make iso              # build CD-based GRUB2 ISO
make bochs_iso_grub2  # run ISO in Bochs
make bochs            # alias for bochs_iso_grub2
```

### Emulator Notes

- **BOCHS** is recommended for debugging due to its excellent GUI debugger. Older BOCHS versions may be more compatible with Ubuntu 20.04.
- **QEMU** may exhibit inconsistent behavior — some features fail in emulation but succeed on real hardware. Development prioritizes **actual hardware** over emulator compatibility.

---

### Summary

R4R is a hardware-centric system. While most operating systems abstract away x86-specific features in the name of portability, this project attempts the opposite: to embrace x86 as deeply as possible, using GCC and modern GNU tools while maintaining a low-level, bare-metal mindset.