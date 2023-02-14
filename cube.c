#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

float cubeWidth = 10;
int width = 160, height = 40;
float zBuffer[160 * 40];
char buffer[160 * 40];
int backgroundASCIICode = ' ';
int distanceFromCam = 50;
float K1 = 40;

float incrementSpeed = 0.5;
float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateAxesX(int i, int j, int k) {  //calculo da rotação no eixo x
    return j * sin(A) * sin(B) * cos(C) -
     k * cos(A) * sin(B) * cos(C) + 
     j * cos(A) * sin(C) + k * sin(A) * sin(C) + 
     i * cos(B) * cos(C);
}

float calculateAxesY(int i, int j, int k) { //calculo da rotação no eixo y
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
     j * sin(A) * sin(B) * sin(C) + 
     k * cos(A) * sin(B) * sin(C) - 
     i * cos(B) * sin(C);
}

float calculateAxesZ(int i, int j, int k) { //calculo da rotação no eixo z
    return k * cos(A) * cos(B) -
    j * sin(A) * cos(B) +
    i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateAxesX(cubeX, cubeY, cubeZ);
    y = calculateAxesY(cubeX, cubeY, cubeZ);
    z = calculateAxesZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1/z;
    xp = (int)(width/2 + K1 * ooz * x * 2); //multiplicado por 2 para aumentar o tamanho do cubo
    yp = (int)(height / 2 + K1 * ooz * y);
    idx = xp + yp * width;
    if(idx >= 0 && idx < width * height) 
    {
        if(ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}


int main() {
    printf("\x1b[2J]"); //esse printf serve para limpar a tela
    while(1) {
        memset(buffer, backgroundASCIICode, width * height); //memset serve para preencher um bloco de memoria com um valor especifico
        memset(zBuffer, 0, width * height * sizeof(float)); //primeiro parametro: começo do endereço de memoria a ser preenchido
                                                                    //segundo parametro: valor a ser preenchido
                                                                    //terceiro parametro: numero de bytes a ser peenchido a partir do primeiro parametro
        for(float cubeX = - cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '@');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '-');
            }   
        }
        printf("\x1b[H"); //esse printf serve para posicionar o cursor no inicio da tela        
        for(int i = 0; i < width * height; i++)
        {
            putchar(i % width ? buffer[i] : 5); //putchar serve para imprimir um caracter na tela
        }
        A += 0.005;
        B += 0.005;
        usleep(1500); //usleep serve para dar um delay na execução do programa por um tempo especifico (em microsegundos
    }                                              
    return 0;
}