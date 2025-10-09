# 🧭 Architecture & Design Philosophy

## Basic Idea and Assumptions

### Tasks as Hardware Objects and the Power of Call Gates

While studying Linux version 0.00, where the core mechanisms for task switching are shown, I noticed something powerful: a task is not just a software structure, but a true **hardware object** on Intel x86 processors. 

This object, when maintained throughout the life of a process and manipulated through hardware-managed mechanisms, offers a huge opportunity:  
> The more you rely on these hardware-level objects, the easier it becomes to build a rich operating system using the processor’s native features.

I speculate that such a design could significantly reduce software-layer dependencies, shrink code size, and lower software complexity.  
Would it consume more memory? Possibly — but that tradeoff is worth examining.

### Why Avoiding Ring Complexity Became the Norm

As Linus Torvalds explained in his early work, the complexity of using all 4 protection rings made most systems avoid them altogether. Instead, UNIX-based systems stuck with ring 0 (kernel) and ring 3 (user space), embracing simplicity and portability across architectures.

But what if we embraced that complexity?

### R4R's Unique Approach: 4 Milli-Kernels Across 4 Rings

To manage that complexity, R4R divides its monolithic kernel into **four separate parts**, each living in its own protection ring.  
These parts act like **mili-kernels (mKernels)** — small yet capable units, each with its own scheduler, signal handler, and interrupt controller.

| Ring | mKernel | Role  |
|------|---------|-------|
|  0   | Core    | System-level operations, init, and dispatch |
|  1   | Devs    | Drivers, hardware access, IRQ dispatch      |
|  2   | Libs    | Shared libraries and services               |
|  3   | Users   | User-mode tasks and interfaces              |

This modular structure is inspired by both elegance and practicality. Each mKernel can be independently loaded, executed, and tested.  
Together, they form a full kernel capable of showcasing hardware privilege separation in action.

### Call Gates: Fast, Versatile, and Underused

Intel’s own documentation has long emphasized:

> *"Call gates are more versatile and faster than trap gates."*

Despite this, they are underused in modern OS designs. R4R makes a point to use them **extensively** — not just as IPC mechanisms, but as fundamental control flow tools between privilege levels.

---

## Proof of Concept

What you see in this system is a working proof-of-concept kernel, made of four smaller segments loaded separately in memory.  
It is designed to run on **real Intel hardware or emulators**, starting from the i486 architecture.

In future articles and tools, we will dive into:

- How these mKernels interact
- How scheduling is done per ring
- How memory and GDT/TSS are configured per privilege level
- How call gates create clean and secure transitions

Stay tuned as this documentation expands.