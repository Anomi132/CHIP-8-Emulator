#include <functional>
#include <string>
#include <tuple>
#include <map>

#include "CPU.hpp"
#include "display.hpp"
#include "Emulator.hpp"

/*
	OPCODE FUCTION DECLARATIONS
	The reason is normal funciton declaration would mix the opcodes around, now they are in hex order
*/

void opcode_0nnn_JUMP(cpuState& state, const uint16_t opcode);
void opcode_00E0_CLS(cpuState& state, const uint16_t opcode);
void opcode_00EE_RET(cpuState& state, const uint16_t opcode);
void opcode_1nnn_JUMP(cpuState& state, const uint16_t opcode);
void opcode_2nnn_CALL(cpuState& state, const uint16_t opcode);
void opcode_3xkk_SKIP(cpuState& state, const uint16_t opcode);
void opcode_4xkk_SKIP(cpuState& state, const uint16_t opcode);
void opcode_5xy0_SKIP(cpuState& state, const uint16_t opcode);
void opcode_6xkk_SET(cpuState& state, const uint16_t opcode);
void opcode_7xkk_ADD(cpuState& state, const uint16_t opcode);
void opcode_8xy0_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy1_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy2_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy3_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy4_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy5_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy6_SET(cpuState& state, const uint16_t opcode);
void opcode_8xy7_SET(cpuState& state, const uint16_t opcode);
void opcode_8xyE_SET(cpuState& state, const uint16_t opcode);
void opcode_9xy0_SKIP(cpuState& state, const uint16_t opcode);
void opcode_Annn_JUMP(cpuState& state, const uint16_t opcode);
void opcode_Bnnn_JUMP(cpuState& state, const uint16_t opcode);
void opcode_Cxkk_SET(cpuState& state, const uint16_t opcode);
void opcode_Dxyn_DISPLAY(cpuState& state, const uint16_t opcode);
void opcode_Ex9E_KEY(cpuState& state, const uint16_t opcode);
void opcode_ExA1_KEY(cpuState& state, const uint16_t opcode);
void opcode_Fx07_DELAY(cpuState& state, const uint16_t opcode);
void opcode_Fx0A_KEY(cpuState& state, const uint16_t opcode);
void opcode_Fx15_DELAY(cpuState& state, const uint16_t opcode);
void opcode_Fx18_SOUND(cpuState& state, const uint16_t opcode);
void opcode_Fx1E_ADD(cpuState& state, const uint16_t opcode);
void opcode_Fx29_SET(cpuState& state, const uint16_t opcode);
void opcode_Fx33_STORE(cpuState& state, const uint16_t opcode);
void opcode_Fx55_STORE(cpuState& state, const uint16_t opcode);
void opcode_Fx65_READ(cpuState& state, const uint16_t opcode);

void unknownOpcode(cpuState& state, const uint16_t opcode);

/*
	DISPATCHER FUNCTION FOR SUB TABLES
*/

void dispatcher_0(cpuState & state, const uint16_t opcode);
void dispatcher_8(cpuState& state, const uint16_t opcode);
void dispatcher_E(cpuState& state, const uint16_t opcode);
void dispatcher_F(cpuState& state, const uint16_t opcode);

/*
	FUNCTION JUMP TABLES
*/

std::function<void(cpuState&, const uint16_t)> primaryTable[16] = {
   dispatcher_0,
   opcode_1nnn_JUMP,
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

std::function<void(cpuState&, const uint16_t)> table_8[16] = {
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

std::map<uint8_t, std::function<void(cpuState&, const uint16_t)>> table_F = {
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

void unknownOpcode(cpuState& state, const uint16_t opcode)
{
	throw std::runtime_error("Unknown opcode " + std::to_string(opcode));
}

uint16_t extractJumpLocation(uint16_t opcode)
{
	return opcode & 0x0FFF;
}

std::pair<uint8_t, uint8_t> extractRegisterAndValue(uint16_t opcode)
{
	uint8_t registerValue = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;

	return { registerValue, value };
}

std::pair<uint8_t, uint8_t> extractTwoRegisters(uint16_t opcode)
{
	uint8_t registerA = (opcode & 0x0F00) >> 8;
	uint8_t registerB = (opcode & 0x00F0) >> 4;

	return { registerA, registerB };
}

/*
	DISPATCHER DEFINITIONS
*/

void dispatcher_0(cpuState& state, const uint16_t opcode)
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

void dispatcher_8(cpuState& state, const uint16_t opcode)
{
	uint8_t lastNibble = opcode & 0x000F;

	if (lastNibble > 0xE)
	{
		throw std::runtime_error("Unknown opcode " + std::to_string(opcode));
	}

	std::function<void(cpuState&, const uint16_t)> opcodeFunction = table_8[lastNibble];
	opcodeFunction(state, opcode);
}

void dispatcher_E(cpuState& state, const uint16_t opcode)
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
		throw std::runtime_error("Unknown opcode " + std::to_string(opcode));
	}
}

void dispatcher_F(cpuState& state, const uint16_t opcode)
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

void opcode_0nnn_JUMP(cpuState& state, const uint16_t opcode)
{
	uint16_t jumpLocation = extractJumpLocation(opcode);
	state.programCounter = jumpLocation;
}

void opcode_00E0_CLS(cpuState& state, const uint16_t opcode)
{
	state.displayRefrence->clearScreen();
}

void opcode_00EE_RET(cpuState& state, const uint16_t opcode)
{
	if (state.stackPointer == 0)
	{
		throw std::runtime_error("No subroutine to return to at program counter location: " + std::to_string(state.programCounter));
	}

	state.programCounter = state._stack.top();
	state._stack.pop();

	state.stackPointer -= 1;
}

void opcode_1nnn_JUMP(cpuState& state, const uint16_t opcode)
{
	uint16_t jumpLocation = extractJumpLocation(opcode);
	state.programCounter = jumpLocation;
}

void opcode_2nnn_CALL(cpuState& state, const uint16_t opcode)
{
	uint16_t subroutineLocation = extractJumpLocation(opcode);

	state.stackPointer += 1;
	state._stack.push(state.programCounter);

	state.programCounter = subroutineLocation;
}

void opcode_3xkk_SKIP(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister, compareValue;
	std::tie(cpuRegister, compareValue) = extractRegisterAndValue(opcode);

	uint8_t registerValue = state.registers[cpuRegister];
	if (registerValue == compareValue)
	{
		state.programCounter += 2;
	}
}

void opcode_4xkk_SKIP(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister, compareValue;
	std::tie(cpuRegister, compareValue) = extractRegisterAndValue(opcode);

	uint8_t registerValue = state.registers[cpuRegister];
	if (registerValue != compareValue)
	{
		state.programCounter += 2;
	}
}

void opcode_5xy0_SKIP(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint8_t registerAValue = state.registers[registerA];
	uint8_t registerBValue = state.registers[registerB];
	if (registerAValue == registerBValue)
	{
		state.programCounter += 2;
	}
}

void opcode_6xkk_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister, setValue;
	std::tie(cpuRegister, setValue) = extractRegisterAndValue(opcode);

	state.registers[cpuRegister] = setValue;
}

void opcode_7xkk_ADD(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister, addValue;
	std::tie(cpuRegister, addValue) = extractRegisterAndValue(opcode);

	state.registers[cpuRegister] += addValue;
}

void opcode_8xy0_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	state.registers[registerA] = state.registers[registerB];
}

void opcode_8xy1_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	state.registers[registerA] |= state.registers[registerB];
	state.registers[VF] = 0;
}

void opcode_8xy2_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	state.registers[registerA] &= state.registers[registerB];
	state.registers[VF] = 0;
}

void opcode_8xy3_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	state.registers[registerA] ^= state.registers[registerB];
	state.registers[VF] = 0;
}

void opcode_8xy4_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint16_t registerSum = state.registers[registerA] + state.registers[registerB];
	uint8_t carry = registerSum > 255 ? 1 : 0;

	state.registers[registerA] = registerSum & 0xFF; // Take only overflow bits
	state.registers[VF] = carry;
}

void opcode_8xy5_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint16_t registerAValue =  state.registers[registerA];
	uint16_t registerBValue = state.registers[registerB];

	uint8_t carry = registerAValue >= registerBValue ? 1 : 0;

	state.registers[registerA] -= registerBValue; 
	state.registers[VF] = carry;
}

void opcode_8xy6_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint8_t targetValue = state.registers[registerA]; 
	uint8_t flag = targetValue & 0x01;

	state.registers[registerA] = (targetValue >> 1) & 0xFF;
	state.registers[VF] = flag;
}

void opcode_8xy7_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint16_t registerAValue = state.registers[registerA];
	uint16_t registerBValue = state.registers[registerB];

	uint8_t carry = registerBValue >= registerAValue ? 1 : 0;

	state.registers[registerA] = registerBValue - registerAValue;
	state.registers[VF] = carry;
}

void opcode_8xyE_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint8_t targetValue = state.registers[registerA]; 

	uint8_t mostBitIs1 = (targetValue >> 7) & 0x1;

	state.registers[registerA] = (targetValue << 1) & 0xFF;
	state.registers[VF] = mostBitIs1;
}

void opcode_9xy0_SKIP(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);

	uint8_t registerAValue = state.registers[registerA];
	uint8_t registerBValue = state.registers[registerB];
	if (registerAValue != registerBValue)
	{
		state.programCounter += 2;
	}
}

void opcode_Annn_JUMP(cpuState& state, const uint16_t opcode)
{
	uint16_t jump = extractJumpLocation(opcode);

	state.addressRegisterI = jump;
}

void opcode_Bnnn_JUMP(cpuState& state, const uint16_t opcode)
{
	uint16_t jump = extractJumpLocation(opcode);

	state.programCounter = jump + state.registers[V0];
}

void opcode_Cxkk_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister, andValue;
	std::tie(cpuRegister, andValue) = extractRegisterAndValue(opcode);

	uint8_t randomNumber = std::rand() % 0xFF;
	state.registers[cpuRegister] = andValue & randomNumber;
}

void opcode_Dxyn_DISPLAY(cpuState& state, const uint16_t opcode)
{
	uint8_t registerA, registerB;
	std::tie(registerA, registerB) = extractTwoRegisters(opcode);
	uint8_t numOfBytes = opcode & 0x000F;

	state.registers[VF] = state.displayRefrence->drawSprite(
		state.registers[registerA],
		state.registers[registerB],
		state.addressRegisterI,
		numOfBytes
	);
}

void opcode_Ex9E_KEY(cpuState& state, const uint16_t opcode)
{
	uint8_t keyRegister = (opcode & 0x0F00) >> 8;
	uint8_t keyIndex = state.registers[keyRegister];

	if (state.emulatorRefrence->getKeyStateById(keyIndex))
	{
		state.programCounter += 2;
	}
}

void opcode_ExA1_KEY(cpuState& state, const uint16_t opcode)
{
	uint8_t keyRegister = (opcode & 0x0F00) >> 8;
	uint8_t keyIndex = state.registers[keyRegister];

	if (!state.emulatorRefrence->getKeyStateById(keyIndex))
	{
		state.programCounter += 2;
	}
}

void opcode_Fx07_DELAY(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;
	state.registers[cpuRegister] = state.delayTimer;
}

void opcode_Fx0A_KEY(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;

	if (state.keyWaitingForRelease == -1)
	{
		int keyPressed = state.emulatorRefrence->getPressedKey();

		if (keyPressed != -1)
		{
			state.keyWaitingForRelease = keyPressed;
		}
		else
		{
			state.programCounter -= 2;
			return;
		}
	}

	if (state.emulatorRefrence->getKeyStateById(state.keyWaitingForRelease))
	{
		state.programCounter -= 2;
		return;
	}

	state.registers[cpuRegister] = state.keyWaitingForRelease;
	state.keyWaitingForRelease = -1;
}

void opcode_Fx15_DELAY(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;
	uint8_t cpuValue = state.registers[cpuRegister];

	state.delayTimer = cpuValue;
}

void opcode_Fx18_SOUND(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;
	uint8_t cpuValue = state.registers[cpuRegister];

	state.soundTimer = cpuValue;
}

void opcode_Fx1E_ADD(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;
	uint8_t cpuValue = state.registers[cpuRegister];

	state.addressRegisterI += cpuValue;
}

void opcode_Fx29_SET(cpuState& state, const uint16_t opcode)
{
	uint8_t charecterRegister = (opcode & 0x0F00) >> 8;
	uint8_t charecterValue = state.registers[charecterRegister] & 0x0F;

	state.addressRegisterI = charecterValue * 5;
}

void opcode_Fx33_STORE(cpuState& state, const uint16_t opcode)
{
	uint8_t cpuRegister = (opcode & 0x0F00) >> 8;
	uint8_t cpuValue = state.registers[cpuRegister];

	uint8_t one = cpuValue % 10;
	cpuValue /= 10;

	uint8_t ten = cpuValue % 10;
	cpuValue /= 10;

	uint8_t hundred = cpuValue % 10;
	cpuValue /= 10;

	uint16_t adress = state.addressRegisterI;

	state.mmuRefrence->writeToMemorryAddress(adress, hundred);
	state.mmuRefrence->writeToMemorryAddress(adress + 1, ten);
	state.mmuRefrence->writeToMemorryAddress(adress + 2, one);
}

void opcode_Fx55_STORE(cpuState& state, const uint16_t opcode)
{
	uint8_t registerLimit = (opcode & 0x0F00) >> 8;

	uint16_t adress = state.addressRegisterI;
	for (int i = 0; i <= registerLimit; i++)
	{
		uint8_t registerValue = state.registers[i];
		state.mmuRefrence->writeToMemorryAddress(adress, registerValue);
		adress += 1;
	}

	state.addressRegisterI = adress;
}

void opcode_Fx65_READ(cpuState& state, const uint16_t opcode)
{
	uint8_t registerLimit = (opcode & 0x0F00) >> 8;

	uint16_t adress = state.addressRegisterI;
	for (int i = 0; i <= registerLimit; i++)
	{
		state.registers[i] = state.mmuRefrence->readMemorryAddress(adress);
		adress += 1;
	}

	state.addressRegisterI = adress;
}


/*
	Calls the coresponding opcode function
	Opcodes obtained at: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
*/
void CPU::executeOptCode(const uint16_t opcode)
{
	uint8_t firstNibble = (opcode & 0xF000) >> 12;

	std::function<void(cpuState&, const uint16_t)> opcodeFunction = primaryTable[firstNibble];
	opcodeFunction(state, opcode);
}
