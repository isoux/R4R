# 🧩 R4R – Version 0.00  
### The First Complete 4-Ring Boot and Transition

---

## ✨ Introduction

When you write something completely from scratch — from the first line of boot code to the first character printed on the screen — you’re not just programming.  
You are **learning to speak directly to the machine**.  

There are moments of frustration, long nights, and doubts that make you question everything —  
but the moment when hardware finally obeys your logic, when a simple “Hello” appears because you made it possible,  
that is the kind of reward that reminds you why we love this craft.

R4R was born from that feeling — from the curiosity to understand the processor not as an abstraction,  
but as a living mechanism that reveals its power only when you approach it with patience and respect.  

I wanted to see what Intel’s **four protection rings** truly meant — not in documentation,  
not in diagrams, but in *real execution*.  
Could they work together in a living system, all four rings — 0, 1, 2, and 3 —  
each with its own task, context, and stack, interacting through **call gates** and **hardware task switches**?

The answer is **yes** — and version 0.00 of R4R is that answer made real.  
Small, simple, imperfect — but **fully functional on real hardware**,  
marking the first build where all four rings cooperate as Intel once intended.

Version **0.00** represents the first fully functional proof-of-concept demonstrating the initialization and cooperation of **all four Intel x86 privilege rings** (Ring 0–3).  

This milestone shows that every ring can be independently initialized, given its own task(s), and then communicate through **hardware-managed transitions** using **call gates** and **task gates**.  

Version **0.00** of R4R marks more than just the first running build —  
it represents a personal milestone in understanding what it truly means  
to communicate with the machine at its lowest, most honest level.

---

## 🌍 Why Version 0.00 Was Born Here

This release was developed and tested primarily on a **real Intel 486 system**,  
not on a virtualized abstraction.  
While emulators like Bochs correctly honor the hardware model,  
QEMU (due to a long-standing bug) fails to reproduce task-switching through LDT/TSS —  
a critical mechanism for this concept.  

That limitation became a *motivation*, not an obstacle.  
By returning to **real hardware**, R4R version 0.00 reconnected with the original intent  
of x86’s privilege model: separation of trust enforced not by software checks,  
but by the processor’s own microcode.  

Publishing this version here, from that machine, is symbolic —  
the moment where curiosity met proof.  
The success on real hardware proves that this approach is not theoretical —  
it runs exactly as designed.

---

## 🧠 Concept Summary

R4R divides what would normally be a monolithic kernel into **four tiny mili-kernels**, each living in its own privilege level:

| Ring | mKernel |
|------|---------|
| **0** | Core |
| **1** | Devs |
| **2** | Libs |
| **3** | Users |

Each ring runs its own code and stack, defined through **individual TSS segments**, with legitimate transitions controlled entirely by hardware mechanisms.

This design fulfills Intel’s original idea of **hierarchical privilege isolation** — something most modern OSs simplify down to just Ring 0 and 3.

Each ring inherits the same paging context (flat mapping),  
but operates on isolated stack and descriptor space.

---

## ⚙️ Initialization Flow

The initialization follows this precise chain of hardware and software events:

1. **Boot**  
   The system starts through GRUB and enters protected mode.  
   Core GDT, IDT, paging, and the first TSS (for Ring 0) are established.

2. **Ring 0 (Core)**  
   Initializes memory, loads all ring-specific descriptors, and installs call gates and main task.  
   Then it triggers the first switch to **Ring 1 (Devs)** via IRET Frame.

3. **Ring 1 (Devs)**  
   Sets up its own LDT and TSS(s) and call gates.  
   Responsible for device-layer and hardware interaction.  
   Prints initialization messages and returns control to CORE via `core_resume()`,  
   which then switches to **Ring 2 (Libs)** via another IRET Frame.

4. **Ring 2 (Libs)**  
   Sets up its own LDT and TSS(s) and call gates.  
   Represents shared kernel libraries and services.  
   Demonstrates call gate transitions and TSS-based task switching.  
   Once ready, returns control to CORE, which continues to **Ring 3 (Users)**.

5. **Ring 3 (Users)**  
   Sets up its own LDT and TSS.  
   Displays its own welcome message and logo.  
   Demonstrates user-space access through call gates (for example, the `syscall_printr` system call).  
   Returns to `main()` at **Ring 0**.

6. The final function at `main()` from **Ring 0 (Core)** jumps to `main_task()` at **Ring 3 (Users)**  
   and remains there in a loop — a fitting conclusion for the beginning.

> 🧩 *All text output from less-privileged rings is performed through system calls using call gate descriptors — without passing any arguments on the stack, but instead through available registers.*  
> This serves as the fundamental proof that **call gates** can operate correctly across all less-privileged rings toward Ring 0.  
> In later versions, we will also demonstrate **calls originating from Ring 3 to Ring 2 or 1**, as well as **from Ring 2 to Ring 1**,  
> showcasing how flexible and efficient this mechanism can be when used as Intel originally intended.

The structure ensures that no ring can directly modify or corrupt another —  
each can only communicate via hardware-controlled mechanisms.


This strict separation makes debugging and tracing state changes straightforward —  
each fault belongs unambiguously to its ring.

At this point, all four rings are **independently initialized and fully functional**.  
Transitions between them occur entirely through **hardware-defined mechanisms** — no software emulation or privilege hacks.

---

## 🧩 Paging and Memory Layout

Version 0.00 uses **a static paging model** for clarity and stability.  
The page directory and tables map a **flat 8 MB address space**.

No dynamic allocator or virtual memory expansion exists yet —  
this version focuses on correctness and predictability.  
However, the paging model is written to allow seamless evolution  
into per-ring memory virtualization in later builds.  

The paging layer was deliberately kept simple to let segmentation and descriptor logic shine through first.

---

## 🔁 Task Switching and Call Gates

Task transitions between rings are handled via:
- **TSS-based task switches** (hardware `ljmp` to a TSS selector)
- **Call gates** for direct control flow across privilege boundaries

This design is intentional:  
to demonstrate that Intel’s segmentation and privilege system  
can form a coherent, elegant message-passing structure —  
one enforced by hardware, not software.

Each call gate in R4R can serve multiple system calls,  
depending on register context — reducing overhead  
while highlighting the flexibility Intel engineers built into the model.

---

## 🧩 Technical Highlights

- Custom **GDT/TSS/LDT** design for each ring  
- Full **task-switching** using TSS descriptors  
- **Call gates** and **hardware ring transitions** instead of software traps  
- Separate **ring stacks** for Rings 0–3  
- Core uses **C + inline assembly** with `-std=c23` (GCC 14)  
- Paging enabled (currently static for 8 MB memory window)  
- Bootable via **Multiboot v1**  

---

## 🧮 Hardware-Level Philosophy

R4R is not a software OS in the modern sense —  
it is a *hardware-cooperative system*.  
Instead of simulating privilege levels through software rules,  
it fully relies on:
- The **CPU’s segmentation model**
- The **GDT/TSS stack switching logic**
- The **hardware-enforced call gates**

This allows for a minimalist, deterministic system  
where no privileged operation is emulated.

---

## 💾 Testing and Real Hardware Results

- ✅ **Bochs emulator:** perfect compliance, including TSS and call gates  
- ⚠️ **QEMU:** fails due to known i486 protected-mode bug  
  → `QEMU Bug 2024806 — "Protected mode LJMP via TSS/LDT fails with pc=nil"`  
- 🖥️ **Real hardware (Compaq Contura Aero 4/33C):**  
  Fully operational; ring transitions and paging verified during execution.

Running on real silicon confirmed that the theory holds:  
the original x86 protection model is fully functional when implemented faithfully.

---

## 📜 Tasks and Structure

Each ring has its own TSS-defined task(s):
- `tss_core_main`
- `tss_devs_irq`
- `tss_devs_sched`
- `tss_libs_irq`
- `tss_libs_sched`
- `tss_users_main`

At this stage, all tasks exist and can be invoked through **hardware task switching**,  
but only `tss_users_main` is active.  
Others serve as **placeholders** for the next phase — IRQ handling and cooperative task scheduling.

---

## 🚀 Next Steps (for Version 0.01)

Planned for version **0.01**:
- Introduce a minimal ring-aware scheduler  
- Implement IRQ redirection and interrupt controller per ring  
- Implement IRQ handler for keyboard input  
- Introduce a minimal message-passing model between mKernels  
- Extend Devs and Libs tasks to actively run loops or routines  
- Expand paging into a partial virtual memory system  
- Begin work on a small interactive user-space environment in Ring 3  

---

## 💬 Final Note

Version 0.00 concludes the **foundational stage** of R4R — where every protection ring is live, accessible, and initialized by legitimate hardware transitions.  

It is a proof — that four rings, properly initialized and respected,  
can coexist, communicate, and protect each other *in hardware*.

It is also a statement:  
that the elegance of x86 architecture was never in its complexity,  
but in the discipline it demanded from those who tried to master it.    

> *“From this point, everything above is just refinement —  
the hard part, the true 4-ring foundation, already works.”*

---

## 🖼️ Screenshots

**COMPAQ CONTURA AERO 4/33C (Real Hardware)**  
![Compaq](../assets/COMPAQ.jpg)

**BOCHS Emulator**  
![Bochs](../assets/BOCHS.png)

---

## 🧾 License

**R4R** is distributed under the **MIT License**.  
Use freely for learning, research, or hobby OS development.

---
