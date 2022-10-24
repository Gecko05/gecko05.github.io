#include <iostream>
#include <stdint.h>
#include <string.h>

#define RAM_LENGTH 4096
uint16_t RAM[RAM_LENGTH];

typedef uint16_t blue_register;
blue_register A;         // Accumulator - For arithmetics, calculations
blue_register DIL;       // Data-In - To receive input data
blue_register DOL;       // Data-Out - To output data
blue_register DSL;       // Device Selector - To select IO device
blue_register IR;        // Instruction Register - Stores the current instruction and it's operands
blue_register MAR;       // Memory Address Register - Tells us which RAM address to read
blue_register MBR;       // Memory Buffer Register - Actual RAM contents as pointed by MAR
blue_register PC = 0x00; // Program Counter - To perform jumps and handle instruction numbers
blue_register SR;        // Console Switch Register - To input data manually
blue_register Z;         // Z register - Auxiliary register for calculations

typedef enum {
	EXECUTE,
	FETCH,
} State;

State STATE = FETCH;

// The first nibble of the most significant byte holds the
// instruction. The rest is just operands.
// F0 03 means NOP with a 3 as an operand
// 17 00 03 in Octal

// Sample program
uint16_t program0[6] = {
	0xF000,
	0xF003,
	0xA004,
	0xF005,
	0xF010,
	0xA000
};

uint8_t get_instruction()
{
	return ((IR & 0xF000) >> 12);
}

void do_NOP(uint8_t tick)
{
	if (tick == 8)
		MAR = PC;
}

void do_JMP(uint8_t tick)
{
	if (tick == 6) {
		PC = 0;
	} else if (tick == 7) {
		PC = (IR & 0x0FFF);
	} else if (tick == 8) {
		MAR = PC;
	}
}

void process_tick(uint8_t tick)
{
	switch (tick) {
	case 1:
		break;
	case 2:
		if (STATE == FETCH)
			PC += 1;
		break;
	case 3:
		if (STATE == FETCH)
			MBR = 0x00;
		break;
	case 4:
		if (STATE == FETCH) {
			IR = 0x00;
			MBR = RAM[MAR]; // The Clear in step 4 also means updating MBR in the next step
		}
		break;
	case 5:
		if (STATE == FETCH)
			IR = MBR;
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	default:
		break;
	}
	uint8_t INS = get_instruction();
	if (INS == 15) {
		do_NOP(tick);
	}
	else if (INS == 10) {
		do_JMP(tick);
	}
}

uint8_t clock_pulse = 1;

void emulateCycle()
{
	while (clock_pulse < 9)
	{
		process_tick(clock_pulse);
		clock_pulse++;
	}
	clock_pulse = 1;
}

void dumpRegisters()
{
	printf("PC: %04x A: %04x IR: %04x Z: %04x MAR: %04x MBR: %04x DSL: %02x DIL: %02x DOL: %02x\n", PC, A, IR, Z, MAR, MBR, (DSL & 0x00FF), (DIL & 0x00FF), (DOL & 0x00FF));
}

void runProgram(const uint16_t* program)
{
	std::cout << "Copying program to the RAM\n";
	memset(RAM, 0x00, RAM_LENGTH * sizeof(uint16_t));
	memmove(RAM, program, (RAM_LENGTH * sizeof(uint16_t)));
	for (;;) {
		emulateCycle();
		dumpRegisters();
	}
}

int main(int argc, char* argv[])
{
    runProgram(program0);
    return 0;
}
