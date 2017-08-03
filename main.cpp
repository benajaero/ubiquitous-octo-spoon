#include <iostream>
#include <SFML/Graphic.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

char memory[4096];
int pc;
char V[16]; //data registers
short I; //address register
short stack[24];
int timer;
int soundTimer;
char keyboard[16];
char fontSet[50]; //I don't know how big this is yet.
void opcode();
void draw();
void input();
void clearScreen();

int main() {
    pc = 0x200; 
    while(true) {
        opcode();
        draw();
        input();
        pc += 2;
    }
    return 0;
}

void clearScreen() {

}

void draw() {

}

void input() {

}
void opcode() {
    unsigned short opcode = memory[pc] << 8 | memory[pc+1];
    switch (opcode & 0xF000) {
        int x = opcode & 0x0F00;
        int y = opcode & 0x00F0;
        int NNN = opcode & 0x0FFF;
        case 0x0000:
            switch(opcode & 0x00FF) {
                case 0x00E0:
                    clearScreen();   
                break;
                case 0x00EE:
                    pc= stack[--sp];
                break;
                default:
                    printf("Unknown opcode, %d at program counter number %d", opcode, pc)
            } 
        break;
        case 0x1000:
            pc = opcode & 0x0FFF;
        break;
        case 0x2000:
            stack[sp];
            ++sp;
            pc = opcode & 0x0FFF;
        break;
        case 0x3000:
            if V[x] == NNN pc +=2;
        break;
        case 0x4000:
           
        break;
        case 0x5000:
        
        break;

        case 0x6000:

        break;

        case 0x7000:

        break;

        case 0x8000:

        break;

        case 0x9000:

        break; 
        default:
                    printf("Unknown opcode, %d at program counter number %d", opcode, pc)

    }
}

