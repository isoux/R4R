# 🧰 Development and Toolchain Overview

The R4R operating system is developed from scratch using the **GCC compiler** with **inline assembly**, targeting **32-bit i386/i486 processors**.  
**Inline assembly** is used because R4R is strictly hardware-oriented and complex due to the multi-ring architecture, so it is essential that the assembly code reside within C files — sharing their scope and context without unnecessary linking.  
This approach improves visibility, readability, and direct interaction with system-level constructs.  
The core intent is to maintain full control over hardware behavior, particularly across all four x86 protection rings.  

> 🧩 **Why I chose C for R4R:**  
> After exploring alternatives like Zig and Rust, I concluded that C remains the most practical, predictable, and hardware-transparent language for low-level system development on 32-bit Intel machines.  
> My reasoning is explained in detail in this forum post:  
> 👉 [“A Warm Acknowledgment and an Objective Conclusion from an Enthusiast” — FreeBSD Forum](https://forums.freebsd.org/threads/zig-programming-language.88728/page-6#post-710025)

---

### Toolchain

- **Compiler:** GCC (std=c23) with support for Intel `__asm__` syntax  
- **Assembler:** Inline assembly, no external `.S` or GAS-based files  
- **Bootloaders:** GRUB v1 (`img` build) and GRUB v2 (`iso` build)  
- **Emulators:** BOCHS (recommended with GUI debugger), QEMU (partially reliable)  
- **Target hardware:** Real Intel i486+ CPUs (system tested on physical machines)  

---

### Design Principles

- Core components are written in **C** and **inline assembly**, where hardware-level control (like privilege transitions, task state manipulation, or descriptor configuration) is required.  
- The system architecture emphasizes **tasks as hardware-managed objects**, with extensive use of **call gates** for controlled transitions across rings.  
- Code is highly **non-portable** and intentionally focused on leveraging underused features of x86 CPUs, such as hardware task switching and segment-based protection.  

---

### Build System

The build system currently supports **floppy image (GRUB v1)** configuration.

Use the following make targets:

```sh
make clean   # recommended before each full build
make         # build folder build/ with objects and floppy-based build/sys_out/grub1.img
make bochs   # run floppy-based grub1.img in Bochs

```
### Emulator Notes

- **BOCHS** is recommended for debugging due to its excellent GUI debugger. Older BOCHS versions may be more compatible with Ubuntu 20.04. I am using Ubuntu 24.04 and Bochs x86 Emulator 2.8.
- **QEMU** may exhibit inconsistent behavior — some features fail in emulation but succeed on real hardware. Development prioritizes **actual hardware** over emulator compatibility.

---

### Summary

R4R is a hardware-centric system. While most operating systems abstract away x86-specific features in the name of portability, this project attempts the opposite: to embrace x86 as deeply as possible, using GCC and modern GNU tools while maintaining a low-level, bare-metal mindset.