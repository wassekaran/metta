#include "DefaultConsole.h"
#include "InterruptServiceRoutine.h"
#include "idt.h"
#include "Globals.h"
#include "common.h"

extern "C"
{
	void isrHandler(Registers regs);
	void irqHandler(Registers regs);
}

// Handles a software interrupt/CPU exception.
// This is architecture specific!
// This gets called from our asm interrupt handler stub.
// TODO: implement handling from usermode.
void isrHandler(Registers regs)
{
// 	kconsole.set_color(GREEN);
// 	kconsole.print("Received interrupt: %d\n", regs.int_no);

	InterruptServiceRoutine *isr = interruptsTable.getIsr(regs.int_no);
	if (isr)
	{
		isr->run(&regs);
	}
}

// IRQ8 and above should be acknowledged to the slave controller, too.
#define SLAVE_IRQ 40

// Handles a hardware interrupt request.
// This is architecture specific!
// This gets called from our asm hardware interrupt handler stub.
void irqHandler(Registers regs)
{
	InterruptServiceRoutine *isr = interruptsTable.getIsr(regs.int_no);
	if (isr)
	{
		isr->run(&regs);
	}

	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no >= SLAVE_IRQ)
	{
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);
}
