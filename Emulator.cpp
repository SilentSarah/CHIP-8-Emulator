#include <Emulator.hpp>

const uint16_t start_address = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

Emulator::Emulator() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
    this->program_counter = start_address;
    this->randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    for (uint32_t i = 0; i < FONTSET_SIZE; i++) {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }
}

void    Emulator::LoadROM(char const *filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        size_t size = file.tellg();

        char *buf = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(buf, size);
        file.close();

        for (long i = 0; i < size; i++) {
            this->memory[start_address + i] = buf[i];
        }

        delete[] buf;
    }
}

void Emulator::cls(void) {
    memset(video_data, 0, sizeof(video_data));
}

void Emulator::ret(void) {
    --stack_pointer;
    program_counter = stack[stack_pointer];
}

void Emulator::jp(void) {
    uint16_t address = opcode & 0x0FFFu;

    program_counter = address;
}

void Emulator::call(void) {
    uint16_t address = opcode & 0x0FFFu;

    stack[stack_pointer] = program_counter;
    ++stack_pointer;
    program_counter = address;
}

void Emulator::se(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] == kk) {
        program_counter += 2;
    }
}

void Emulator::seNOT(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] != kk) {
        program_counter += 2;
    }
}

void Emulator::seXY(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] == registers[Vy]) {
        program_counter += 2;
    }
}

void Emulator::ld(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    registers[Vx] = kk;
}

void    Emulator::add(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00F0u;

    registers[Vx] += kk;
}

void Emulator::ldSetRegister(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];
}

void Emulator::and(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = (registers[Vx] & registers[Vy]);
}

void Emulator::xor(void) {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = (registers[Vx] ^ registers[Vy]);
}