#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "cpu.hpp"
#include "MMU.hpp"

/*
	Get the next instruction based on the current position of the program counter
	Increments program counter to the next instruction
*/
opcode CPU::getNextInstruction()
{
	if (_programCounter >= 4096) // Max memorry
	{
		throw "programCounter outside memorry range (0-4095): " + std::to_string(_programCounter);
	}

	uint16_t opcodeUpper = _mmuRefrence->readMemorryAddress(_programCounter);
	uint16_t opcodeLower = _mmuRefrence->readMemorryAddress(_programCounter + 1);
	uint16_t fullOpcode = (opcodeUpper << 8) | opcodeLower;

	_programCounter += 2;
	return static_cast<opcode>(fullOpcode);
}

/*
	Runs a single CPU Cycle
*/
void CPU::runClockCycle()
{
	opcode currentOptCode = getNextInstruction();
	executeOptCode(currentOptCode);
}

/*
	Reads an instruction and executes it. 
	Opcodes obtained at: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
*/
void CPU::executeOptCode(const opcode opcode)
{
	srand(time(0));

	uint16_t opcodeID = opcode.nibles.opcodeID;
	uint16_t sectionOne = opcode.nibles.sectionOne;
	uint16_t sectionTwo = opcode.nibles.sectionTwo;
	uint16_t sectionThree = opcode.nibles.sectionThree;

	uint16_t registerNumber = opcode.cmp.registerNumber;
	uint16_t compareNumber = opcode.cmp.number;

	uint16_t jumpAddress = opcode.jump.jump;

	switch (opcodeID)
	{
		case 0:
			// 00E0 - CLS
			if (sectionTwo == 0 && sectionThree == 0)
			{
				_displayRefrence->clearScreen();
			}
			// 00EE - RET
			else if (sectionTwo == 0)
			{
				_programCounter = _stack.top();
				_stack.pop();
				_stackPointer--;
			}
			// 0nnn - SYS addr
			else 
			{
				_programCounter = jumpAddress;
			}
			break;

		// 1nnn - JP addr
		case 1:
			_programCounter = jumpAddress;
			break;

		// 2nnn - CALL addr
		case 2:
			_stackPointer++;
			_stack.push(_programCounter);
			_programCounter = jumpAddress;
			break;

		// 3xkk - SE Vx, byte
		case 3:
			uint16_t registerValue = _registers[registerNumber];
			if (registerValue == compareNumber)
			{
				_programCounter += 2;
			}
			break;

		// 4xkk - SNE Vx, byte
		case 4:
			uint16_t registerValue = _registers[registerNumber];
			if (registerValue != compareNumber)
			{
				_programCounter += 2;
			}
			break;

		// 5xy0 - SE Vx, Vy
		case 5:
			uint16_t registerValue1 = _registers[sectionOne];
			uint16_t registerValue2 = _registers[sectionTwo];
			if (registerValue1 == registerValue2)
			{
				_programCounter += 2;
			}
			break;

		// 6xkk - LD Vx, byte
		case 6:
			_registers[registerNumber] = compareNumber;
			break;

		// 7xkk - ADD Vx, byte
		case 7:
			_registers[registerNumber] += compareNumber;
			break;

		case 8:
			// 8xy0 - LD Vx, Vy
			if (sectionThree == 0)
			{
				_registers[sectionOne] = _registers[sectionTwo];
			}
			// 8xy1 - OR Vx, Vy
			else if (sectionThree == 1)
			{
				_registers[sectionOne] |= _registers[sectionTwo];
			}
			// 8xy2 - AND Vx, Vy
			else if (sectionThree == 2)
			{
				_registers[sectionOne] &= _registers[sectionTwo];
			}
			//8xy3 - XOR Vx, Vy
			else if (sectionThree == 3)
			{
				_registers[sectionOne] ^= _registers[sectionTwo];
			}
			// 8xy4 - ADD Vx, Vy
			else if (sectionThree == 4)
			{
				uint16_t sum = _registers[sectionOne] + _registers[sectionOne];
				_registers[VF] = (sum > 255) ? 1 : 0;
				_registers[sectionOne] = sum;
			}
			// 8xy5 - SUB Vx, Vy
			else if (sectionThree == 5)
			{
				int subtract = _registers[sectionOne] - _registers[sectionOne];
				_registers[VF] = (_registers[sectionOne] > _registers[sectionOne]) ? 1 : 0;
				_registers[sectionOne] = subtract;
			}
			// 8xy6 - SHR Vx {, Vy}
			else if (sectionThree == 6)
			{
				if (_registers[sectionOne] % 0x1)
				{
					_registers[VF] = 1;
				}
				else
				{
					_registers[VF] = 0;
				}

				_registers[sectionOne] /= 2;
			}
			// 8xy7 - SUBN Vx, Vy
			else if (sectionThree == 7)
			{
				int subtract = _registers[sectionOne] - _registers[sectionOne];
				_registers[VF] = (_registers[sectionOne] < _registers[sectionOne]) ? 1 : 0;
				_registers[sectionOne] = subtract;
			}
			// 8xyE - SHL Vx {, Vy}
			else if (sectionThree == 0xE)
			{
				if ((_registers[sectionOne] >> 8) % 0x1)
				{
					_registers[VF] = 1;
				}
				else
				{
					_registers[VF] = 0;
				}

				_registers[sectionOne] *= 2;
			}
			break;

		// 9xy0 - SNE Vx, Vy
		case 9:
			uint16_t registerValue1 = _registers[sectionOne];
			uint16_t registerValue2 = _registers[sectionTwo];
			if (registerValue1 != registerValue2)
			{
				_programCounter += 2;
			}
			break;

		// Annn - LD I, addr
		case 0xA:
			_addressRegisterI = jumpAddress;
			break;

		// Bnnn - JP V0, addr
		case 0xB:
			_programCounter = jumpAddress + _registers[V0];
			break;

		// Cxkk - RND Vx, byte
		case 0xC:
			int randomNumber = rand() % 256;
			_registers[sectionOne] = randomNumber & compareNumber;
			break;

		// Dxyn - DRW Vx, Vy, nibble
		case 0xD:
			bool collision = _displayRefrence->drawSprite(sectionOne, sectionTwo, _addressRegisterI, sectionThree);
			_registers[VF] = collision;
			break;
	}
}
