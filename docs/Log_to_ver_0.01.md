# 🧩 R4R – Log of Changes Toward Version 0.01  
### Initial fixes of non-functional code

After the system boots, when GRUB loads the required modules, the **init module** is placed at the **2 MB address**.  
Execution jumps there, where all core initialization steps are performed — this is where the `main()` function resides.  
From there, initialization continues through **IRET frames** into all four rings, setting up their respective environments.

What was non-functional in the previous iteration but correct in **version 0.00** has now been **refined and adjusted for version 0.01**.  
Previously, the **first 32 exception handlers** (CPU exceptions) were part of the **init domain**, which gets deleted after launching the main user task in **Ring 3**.  
As a result, those handlers were lost once the `init` module was unloaded.

To fix this, all **exception handlers** and their corresponding **IDT entries** were moved into the **Core mili-kernel**, so they remain persistent and under **Ring 0** control.  
This ensures that all critical faults and traps are now handled correctly within the Core domain — the legitimate owner of system exceptions.

🔧 **Related commits**

- [Refined and re-set IDT table in core domain](https://github.com/isoux/R4R/commit/c82bd773846f8f7f80ccace168920f82bd9ab574)  
- [Move exceptions from init to core](https://github.com/isoux/R4R/commit/6ccb19e45cf7fd951d2be101dea93c4282b35533)  
- [Moved exceptions from init to core (final adjustment)](https://github.com/isoux/R4R/commit/1a74fcaf695dbe9bf25089eb746ea7bf40853542)


✅ **Summary:**  
Exception handling is now fully managed by the **Core mili-kernel**, ensuring stable and permanent interrupt behavior after the `init` module is unloaded.  
The initialization process remains clean, hardware-consistent, and logically aligned with the 4-ring architecture.

---

# Interrupt Handling Across Rings

In this setup, we handle hardware interrupts from Ring 3 tasks by routing them to Ring 1 handlers, ensuring controlled I/O access.

 1. IDT Descriptor
For the keyboard interrupt (`0x21`), we set an IDT entry with a **32-bit interrupt gate**.  
The gate’s **DPL** is set to **1**, allowing Ring 3 to invoke it but ensuring the handler executes in **Ring 1**.

 2. TSS Setup
The **TSS** for the current task specifies a valid **Ring 1 stack (SS1:ESP1)**.  
When the interrupt occurs, the CPU automatically switches from the Ring 3 stack to the Ring 1 stack.

 3. IOPL and EFLAGS
The Ring 3 task’s **EFLAGS** have `IF=1` (interrupts enabled) and `IOPL=1`.  
With `IOPL < 3`, Ring 3 cannot perform `IN/OUT` instructions — ensuring all I/O must be executed at a higher privilege level.

 4. I/O Control
The **Ring 1 handler** can safely perform I/O operations since it executes at a higher privilege level than the Ring 3 task that triggered the interrupt.  
This guarantees that tasks in Ring 3 **cannot directly access hardware**.

 5. Segmentation and Gates
**Call gates** and segmentation ensure controlled transitions between privilege rings.  
Ring 3 can invoke the Ring 1 interrupt handler but **cannot escalate privileges** or perform unauthorized I/O.

---

 ✅ Summary
This architecture ensures that **Ring 3 tasks cannot directly interact with hardware**, maintaining system stability and security by enforcing privilege levels and controlled access to hardware through trusted Ring 1 interrupt handlers.

---

# Keyboard Interrupt Flow, Devs IRQ Mechanism, and Userspace Event Loop

In this version, we demonstrate a minimal keyboard interrupt flow that shows how the system handles input step-by-step.

When a keyboard interrupt occurs, the `register_irq()` function is called, which switches to a nested task in ring 1. This nested task temporarily disables interrupts and processes the event in a very lightweight manner. The stack used by this nested task is small, and the result of the interrupt (like the ASCII code of the key pressed) is stored on that stack. This keeps stack usage predictable and reduces the risk of overflow.

The `devs_irq_task` event loop then calls `get_keyboard_int()`, which handles the key press and stores the result back on the stack. After this, the system returns to the original task (in ring 3) via an `iret`, where the result is read from the stack.

In the userspace event loop, we continuously check if a new character is available. If there is, we print it immediately using `print_char()`. Right now, the keyboard functionality is minimal, with only basic keys working (no shift yet), but this demonstrates how the system can process input in real-time, without waiting for a full scheduler cycle.

This approach makes it clear how tasks can handle specific events, keeps the stacks isolated, and reduces complexity. Each task does a small job, and the system remains responsive and simple.

_For more detailed explanations, we recommend checking the code in [this commit](https://github.com/isoux/R4R/commit/47c3addedd383e8acc33ac5b51d3d9900a326f6d), which contains extensive comments._

---

# Gradual Introduction of Testing

Initial testing has begun, focusing primarily on compiling files directly on the same machine used for building the kernel. These tests are not about verifying how the R4R system runs, but rather about checking various macros and memory values of variables. This marks the [first step](https://github.com/isoux/R4R/commit/209547f0b297e483ebdec81bfeb111238d52fe53) in ensuring that system definitions are correct and consistent.

---


