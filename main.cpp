#include <SFML/Graphic.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

char memory[4096];
char V[16]; //data registers
short I; //address register
short stack[24];
int timer;
int soundTimer;
char keyboard[16];

void opcode();
void draw();
void input();

int main() {
    
    while(true) {
        opcode();
        draw();
        input();
    }
    return 0;
}

void draw() {

}

void input() {

}
void opcode() {

}

