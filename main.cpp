#include <iostream>
#include <fstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class chip8 {
    private:
        unsigned char memory[4096];
        unsigned int pc;
        unsigned char V[16]; // data registers
        unsigned short I; //address register
        unsigned short stack[24];
        unsigned short sp;
        unsigned int timer;
        unsigned int soundTimer;
        unsigned char keyboard[16];
        unsigned char gfx[2048];
        unsigned char fontSet[80] = {
          0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
          0x20, 0x60, 0x20, 0x20, 0x70, // 1
          0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
          0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
          0x90, 0x90, 0xF0, 0x10, 0x10, // 4
          0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
          0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
          0xF0, 0x10, 0x20, 0x40, 0x40, // 7
          0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
          0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
          0xF0, 0x90, 0xF0, 0x90, 0x90, // A
          0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
          0xF0, 0x80, 0x80, 0x80, 0xF0, // C
          0xE0, 0x90, 0x90, 0x90, 0xE0, // D
          0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
          0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

    public:
        chip8() {
            pc = 0x200;
            I = 0;
            sp = 0;
            for (int i = 0; i < sizeof fontSet / sizeof fontSet[0]; i++) {
                memory[i] = fontSet[i];
            }
        }
        void loop(sf::Window& window) {
            while(window.isOpen()) {
                input(window);
                opcode();
            }
        }

        void loadFile(char* dir) {
            std::ifstream rom(dir, std::ios::binary);
            for (int i = 0; i < 3584; i++) {
                if (rom) rom >> memory[i + 512];
                printf("%X", memory[512 + i]);
            }
            rom.close(); 
        }

        void clearScreen() {

        }

        void draw(char x, char y, char height) {

        }

        void input(sf::Window& window) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }
        }
        void opcode() {
            unsigned short opcode = memory[pc] << 8 | memory[pc+1];
            unsigned int x = (opcode & 0x0F00) >> 4;
            unsigned int y = (opcode & 0x00F0) >> 8;
            unsigned int NNN = opcode & 0x0FFF;
            unsigned int NN = opcode & 0x00FF;

            switch (opcode & 0xF000) {
                case 0x0000:
                    switch(opcode & 0x00FF) {
                        case 0x00E0:
                            clearScreen();   
                        break;
                        case 0x00EE:
                            pc= stack[--sp];
                        break;
                        default:
                            printf("Unknown opcode, %X at program counter number %X \n", opcode, pc);
                    } 
                break;
                case 0x1000:
                    pc = opcode & 0x0FFF;
                break;
                case 0x2000:
                    stack[sp] = pc;
                    ++sp;
                    pc = opcode & 0x0FFF;
                break;
                case 0x3000:
                    if (V[x] == NN) pc += 4;
                    else pc += 2;
                break;
                case 0x4000:
                    if (V[x] != NN) pc += 4;
                    else pc += 2;
                break;
                case 0x5000:
                    if (V[x] == V[y]) pc += 4; 
                    else pc += 2;
                break;
                case 0x6000:
                    V[x] = NN;
                    pc += 2;
                break;

                case 0x7000:
                    V[x] += NN;
                    pc += 2;
                break;

                case 0x8000: // bitwise ops
                    switch(opcode & 0x000F) {
                        case 0x0000:
                            V[x] = V[y];
                            pc += 2;
                        break;

                        case 0x0001:
                            V[x] = V[x] | V[y];
                            pc += 2;
                        break;

                        case 0x0002:
                            V[x] = V[x] & V[y];
                            pc += 2;
                        break;
                        
                        case 0x0003:
                            V[x] = V[x] ^ V[y];
                            pc += 2;
                        break;   
                        
                        case 0x0004:
                            if (V[y] > (0xFF - V[x])) V[0xF] = 1;
                            else V[0xF] = 0;
                            V[x] += V[y];
                            pc += 2;
                        break;
                        
                        case 0x0005:
                            if (V[y] > V[x]) V[0xF] = 0;
                            else V[0xF] = 1;
                            V[x] -= V[y];
                            pc += 2;
                        break;    

                        case 0x0006:
                            V[0xF] = V[x] & 1;
                            V[x] = V[x] >> 1;
                            pc += 2;
                        break;
                        case 0x0007:
                            if (V[x] > V[y]) V[0xF] = 0;
                            else V[0xF] = 1;
                            V[y] -= V[x];
                            pc += 2;
                        break; 
                        
                        case 0x000E:
                            V[0xF] = V[x] & 0xF000;
                            V[x] = V[x] << 1;
                        break;
                        default:
                            printf("Unknown opcode, %X at program counter number %X \n", opcode, pc);
                    }
                break;

                case 0x9000:
                    if (V[x] != V[y]) pc += 4; 
                    else pc += 2;
                break; 

                case 0xA000:
                    I = NNN;
                    pc += 2;
                break;

                case 0xB000:
                    pc = V[0] + NNN; 
                break;

                case 0xC000:
                    V[x] = (rand() % 256) & NN;
                    pc += 2;
                break;

                case 0xD000:
                    draw(V[x], V[y], opcode & 0x000F);
                    pc += 2;
                break;
                
                case 0xE000:
                    switch(opcode & 0x00FF) {
                        case 0x009E:
                            if (keyboard[V[x]] == 1) pc += 4;
                            else pc += 2;
                        break;
                        case 0x00A1:
                            if (keyboard[V[x]] == 0) pc += 4;
                            else pc += 2;

                        break;

                        default:
                            printf("Unknown opcode, %X at program counter number %X \n", opcode, pc);
                    }
                break;

                case 0xF000:
                    switch(opcode & 0x00FF) {
                        case 0x0007:
                            V[x] = timer;
                            pc += 2;
                        break;

                        case 0x000A:
                           //wait for key 
                        break;

                        case 0x0015:
                            timer = V[x];
                            pc += 2; 
                        break;

                        case 0x0018:
                            soundTimer = V[x];
                            pc += 2;
                        break;

                        case 0x001E:
                            I += V[x];
                            pc += 2;
                        break;

                        case 0x0029:
                            //complete later
                            pc += 2;
                        break;


                    }
                break;
                default:
                    printf("Unknown opcode, %X at program counter number %X \n", opcode, pc);

            }
        }
};

int main(int argc, char** argv) {
    chip8 Chip8;
    if (argc > 1) Chip8.loadFile(argv[1]);
    else Chip8.loadFile("./games/TETRIS"); 

    sf::Window window(sf::VideoMode(800, 600), "Chip-8 Window");
    Chip8.loop(window);
    return 0;
}



