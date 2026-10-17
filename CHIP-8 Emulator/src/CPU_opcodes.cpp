#include <functional>
#include <string>
#include <map>

#include "cpu.hpp"

/*
	OPCODE FUCTION DECLARATIONS
	The reason is normal funciton declaration would mix the opcodes around, now they are in hex order
*/

void opcode_0nnn_JUMP(CPU_State& state, const uint16_t opcode);
void opcode_00E0_CLS(CPU_State& state, const uint16_t opcode);
void opcode_00EE_RET(CPU_State& state, const uint16_t opcode);
void opcode_1nnn_CLS(CPU_State& state, const uint16_t opcode);
void opcode_2nnn_CALL(CPU_State& state, const uint16_t opcode);
void opcode_3xkk_SKIP(CPU_State& state, const uint16_t opcode);
void opcode_4xkk_SKIP(CPU_State& state, const uint16_t opcode);
void opcode_5xy0_SKIP(CPU_State& state, const uint16_t opcode);
void opcode_6xkk_SET(CPU_State& state, const uint16_t opcode);
void opcode_7xkk_ADD(CPU_State& state, const uint16_t opcode);
void opcode_8xy0_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy1_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy2_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy3_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy4_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy5_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy6_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xy7_SET(CPU_State& state, const uint16_t opcode);
void opcode_8xyE_SET(CPU_State& state, const uint16_t opcode);
void opcode_9xy0_SKIP(CPU_State& state, const uint16_t opcode);
void opcode_Annn_JUMP(CPU_State& state, const uint16_t opcode);
void opcode_Bnnn_JUMP(CPU_State& state, const uint16_t opcode);
void opcode_Cxkk_SET(CPU_State& state, const uint16_t opcode);
void opcode_Dxyn_DISPLAY(CPU_State& state, const uint16_t opcode);
void opcode_Ex9E_KEY(CPU_State& state, const uint16_t opcode);
void opcode_ExA1_KEY(CPU_State& state, const uint16_t opcode);
void opcode_Fx07_DELAY(CPU_State& state, const uint16_t opcode);
void opcode_Fx0A_KEY(CPU_State& state, const uint16_t opcode);
void opcode_Fx15_DELAY(CPU_State& state, const uint16_t opcode);
void opcode_Fx18_SOUND(CPU_State& state, const uint16_t opcode);
void opcode_Fx1E_ADD(CPU_State& state, const uint16_t opcode);
void opcode_Fx29_SET(CPU_State& state, const uint16_t opcode);
void opcode_Fx33_STORE(CPU_State& state, const uint16_t opcode);
void opcode_Fx55_STORE(CPU_State& state, const uint16_t opcode);
void opcode_Fx65_READ(CPU_State& state, const uint16_t opcode);

void unknownOpcode(CPU_State& state, const uint16_t opcode);

/*
	DISPATCHER FUNCTION FOR SUB TABLES
*/

void dispatcher_0(CPU_State & state, const uint16_t opcode);
void dispatcher_8(CPU_State& state, const uint16_t opcode);
void dispatcher_E(CPU_State& state, const uint16_t opcode);
void dispatcher_F(CPU_State& state, const uint16_t opcode);

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

std::function<void(CPU_State&, const uint16_t)> table_8[16] = {
	opcode_8xy0_SET, 
	opcode_8xy1_SET, 
	opcode_8xy2_SET, 
	opcode_8xy3_SET, 
	opcode_8xy4_SET,
	opcode_8xy5_SET, 
	opcode_8xy6_SET, 
	opcode_8xy7_SET, 
	unknownOpcode,
	unknownOpcode,
	unknownOpcode,
	unknownOpcode,
	unknownOpcode,
	unknownOpcode,
	opcode_8xyE_SET, 
	unknownOpcode
};

std::map<uint8_t, std::function<void(CPU_State&, const uint16_t)>> table_F = {
	{0x07, opcode_Fx07_DELAY},
	{0x0A, opcode_Fx0A_KEY},
	{0x15, opcode_Fx15_DELAY},
	{0x18, opcode_Fx18_SOUND},
	{0x1E, opcode_Fx1E_ADD},
	{0x29, opcode_Fx29_SET},
	{0x33, opcode_Fx33_STORE},
	{0x55, opcode_Fx55_STORE},
	{0x65, opcode_Fx65_READ}
};

/*
	HELPER FUNCTIONS
*/

void unknownOpcode(CPU_State& state, const uint16_t opcode)
{
	throw "Unknown opcode " + std::to_string(opcode);
}

/*
	DISPATCHER DEFINITIONS
*/

void dispatcher_0(CPU_State& state, const uint16_t opcode)
{
	uint8_t lastTwoNibbles = opcode & 0x00FF;

	if (lastTwoNibbles == 0xE0)
	{
		opcode_00E0_CLS(state, opcode);
	}
	else if (lastTwoNibbles == 0xEE)
	{
		opcode_00EE_RET(state, opcode);
	}
	else
	{
		opcode_0nnn_JUMP(state, opcode);
	}
}

void dispatcher_8(CPU_State& state, const uint16_t opcode)
{
	uint8_t lastNibble = opcode & 0x000F;

	if (lastNibble > 0xE)
	{
		throw "Unknown opcode " + std::to_string(opcode);
	}

	std::function<void(CPU_State&, const uint16_t)> opcodeFunction = table_8[lastNibble];
	opcodeFunction(state, opcode);
}

void dispatcher_E(CPU_State& state, const uint16_t opcode)
{
	uint8_t lastTwoNibbles = opcode & 0x00FF;

	if (lastTwoNibbles == 0x9E)
	{
		opcode_Ex9E_KEY(state, opcode);
	}
	else if (lastTwoNibbles == 0xA1)
	{
		opcode_ExA1_KEY(state, opcode);
	}
	else
	{
		throw "Unknown opcode " + std::to_string(opcode);
	}
}

void dispatcher_F(CPU_State& state, const uint16_t opcode)
{
	uint8_t lastTwoNibbles = opcode & 0x00FF;

	auto it = table_F.find(lastTwoNibbles);
	if (it != table_F.end())
	{
		it->second(state, opcode);
	}
	else
	{
		unknownOpcode(state, opcode);
	}
}


/*
	OPTCODE DEFINITIONS
*/

void opcode_0nnn_JUMP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_00E0_CLS(CPU_State& state, const uint16_t opcode)
{

}

void opcode_00EE_RET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_1nnn_CLS(CPU_State& state, const uint16_t opcode)
{

}

void opcode_2nnn_CALL(CPU_State& state, const uint16_t opcode)
{

}

void opcode_3xkk_SKIP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_4xkk_SKIP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_5xy0_SKIP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_6xkk_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_7xkk_ADD(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy0_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy1_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy2_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy3_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy4_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy5_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy6_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xy7_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_8xyE_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_9xy0_SKIP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Annn_JUMP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Bnnn_JUMP(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Cxkk_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Dxyn_DISPLAY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Ex9E_KEY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_ExA1_KEY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx07_DELAY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx0A_KEY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx15_DELAY(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx18_SOUND(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx1E_ADD(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx29_SET(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx33_STORE(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx55_STORE(CPU_State& state, const uint16_t opcode)
{

}

void opcode_Fx65_READ(CPU_State& state, const uint16_t opcode)
{

}


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
