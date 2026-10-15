#include <functional>

#include "cpu.hpp"

/*
	OPCODE FUCTION DECLARATIONS
	The reason is normal funciton declaration would mix the opcodes around, now they are in hex order
*/

void opcode_0nnn_JUMP(CPU_State& state, const uint16_t op);
void opcode_00E0_CLS(CPU_State& state, const uint16_t op);
void opcode_00EE_RET(CPU_State& state, const uint16_t op);
void opcode_1nnn_CLS(CPU_State& state, const uint16_t op);
void opcode_2nnn_CALL(CPU_State& state, const uint16_t op);
void opcode_3xkk_SKIP(CPU_State& state, const uint16_t op);
void opcode_4xkk_SKIP(CPU_State& state, const uint16_t op);
void opcode_5xy0_SKIP(CPU_State& state, const uint16_t op);
void opcode_6xkk_SET(CPU_State& state, const uint16_t op);
void opcode_7xkk_ADD(CPU_State& state, const uint16_t op);
void opcode_8xy0_SET(CPU_State& state, const uint16_t op);
void opcode_8xy1_SET(CPU_State& state, const uint16_t op);
void opcode_8xy2_SET(CPU_State& state, const uint16_t op);
void opcode_8xy3_SET(CPU_State& state, const uint16_t op);
void opcode_8xy4_SET(CPU_State& state, const uint16_t op);
void opcode_8xy5_SET(CPU_State& state, const uint16_t op);
void opcode_8xy6_SET(CPU_State& state, const uint16_t op);
void opcode_8xy7_SET(CPU_State& state, const uint16_t op);
void opcode_8xyE_SET(CPU_State& state, const uint16_t op);
void opcode_9xy0_SKIP(CPU_State& state, const uint16_t op);
void opcode_Annn_JUMP(CPU_State& state, const uint16_t op);
void opcode_Bnnn_JUMP(CPU_State& state, const uint16_t op);
void opcode_Cxkk_SET(CPU_State& state, const uint16_t op);
void opcode_Dxyn_DISPLAY(CPU_State& state, const uint16_t op);
void opcode_Ex9E_KEY(CPU_State& state, const uint16_t op);
void opcode_ExA1_KEY(CPU_State& state, const uint16_t op);
void opcode_Fx07_DELAY(CPU_State& state, const uint16_t op);
void opcode_Fx0A_KEY(CPU_State& state, const uint16_t op);
void opcode_Fx15_DELAY(CPU_State& state, const uint16_t op);
void opcode_Fx18_SOUND(CPU_State& state, const uint16_t op);
void opcode_Fx1E_ADD(CPU_State& state, const uint16_t op);
void opcode_Fx29_SET(CPU_State& state, const uint16_t op);
void opcode_Fx33_STORE(CPU_State& state, const uint16_t op);
void opcode_Fx55_STORE(CPU_State& state, const uint16_t op);
void opcode_Fx65_READ(CPU_State& state, const uint16_t op);

/*
	DISPATCHER FUNCTION FOR SUB TABLES
*/

void dispatcher_0(CPU_State & state, const uint16_t op);
void dispatcher_8(CPU_State& state, const uint16_t op);
void dispatcher_E(CPU_State& state, const uint16_t op);
void dispatcher_F(CPU_State& state, const uint16_t op);

/*
	FUNCTION JUMP TABLES
*/

std::function<void(CPU_State&, const uint16_t)> primaryTable[16] = {
   dispatcher_0,
   opcode_1nnn_CLS,
   opcode_2nnn_CALL,
   opcode_3xkk_SKIP,
   opcode_4xkk_SKIP,
   opcode_5xy0_SKIP,
   opcode_6xkk_SET,
   opcode_7xkk_ADD,
   dispatcher_8,
   opcode_9xy0_SKIP,
   opcode_Annn_JUMP,
   opcode_Bnnn_JUMP,
   opcode_Cxkk_SET,
   opcode_Dxyn_DISPLAY,
   dispatcher_E,
   dispatcher_F
};

std::function<void(CPU_State&, const uint16_t)> table_0[3] = {
   opcode_0nnn_JUMP,
   opcode_00E0_CLS,
   opcode_00EE_RET
};

std::function<void(CPU_State&, const uint16_t)> table_8[9] = {
	opcode_8xy0_SET,
	opcode_8xy1_SET,
	opcode_8xy2_SET,
	opcode_8xy3_SET,
	opcode_8xy4_SET,
	opcode_8xy5_SET,
	opcode_8xy6_SET,
	opcode_8xy7_SET,
	opcode_8xyE_SET
};

std::function<void(CPU_State&, const uint16_t)> table_E[2] = {
	opcode_Ex9E_KEY,
	opcode_ExA1_KEY
};

std::function<void(CPU_State&, const uint16_t)> table_F[9] = {
	opcode_Fx07_DELAY,
	opcode_Fx0A_KEY,
	opcode_Fx15_DELAY,
	opcode_Fx18_SOUND,
	opcode_Fx1E_ADD,
	opcode_Fx29_SET,
	opcode_Fx33_STORE,
	opcode_Fx55_STORE,
	opcode_Fx65_READ
};

/*
	FUNCTION DEFINITIONS
*/

/*
	Calls the coresponding opcode function
	Opcodes obtained at: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
*/
void CPU::executeOptCode(const uint16_t opcode)
{
	/*srand(time(0));

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
	{
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
	}

	// 1nnn - JP addr
	case 1:
	{
		_programCounter = jumpAddress;
		break;
	}

	// 2nnn - CALL addr
	case 2:
	{
		_stackPointer++;
		_stack.push(_programCounter);
		_programCounter = jumpAddress;
		break;
	}

	// 3xkk - SE Vx, byte
	case 3:
	{
		uint16_t registerValue = _registers[registerNumber];
		if (registerValue == compareNumber)
		{
			_programCounter += 2;
		}
		break;
	}

	// 4xkk - SNE Vx, byte
	case 4:
	{
		uint16_t registerValue = _registers[registerNumber];
		if (registerValue != compareNumber)
		{
			_programCounter += 2;
		}
		break;
	}

	// 5xy0 - SE Vx, Vy
	case 5:
	{
		uint16_t registerValue1 = _registers[sectionOne];
		uint16_t registerValue2 = _registers[sectionTwo];
		if (registerValue1 == registerValue2)
		{
			_programCounter += 2;
		}
		break;
	}

	// 6xkk - LD Vx, byte
	case 6:
	{
		_registers[registerNumber] = compareNumber;
		break;
	}

	// 7xkk - ADD Vx, byte
	case 7:
	{
		_registers[registerNumber] += compareNumber;
		break;
	}

	case 8:
	{
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
	}

	// 9xy0 - SNE Vx, Vy
	case 9:
	{
		uint16_t registerValue1 = _registers[sectionOne];
		uint16_t registerValue2 = _registers[sectionTwo];
		if (registerValue1 != registerValue2)
		{
			_programCounter += 2;
		}
		break;
	}

	// Annn - LD I, addr
	case 0xA:
	{
		_addressRegisterI = jumpAddress;
		break;
	}

	// Bnnn - JP V0, addr
	case 0xB:
	{
		_programCounter = jumpAddress + _registers[V0];
		break;
	}

	// Cxkk - RND Vx, byte
	case 0xC:
	{
		int randomNumber = rand() % 256;
		_registers[sectionOne] = randomNumber & compareNumber;
		break;
	}

	// Dxyn - DRW Vx, Vy, nibble
	case 0xD:
	{
		bool collision = _displayRefrence->drawSprite(sectionOne, sectionTwo, _addressRegisterI, sectionThree);
		_registers[VF] = collision;
		break;
	}
	}*/
}
