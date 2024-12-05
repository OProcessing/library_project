#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN 30
#define MAX_BUFFER_SIZE 256
#define UART_BUFFER_SIZE 512

char uartBuffer[UART_BUFFER_SIZE] = 
    "$GPGGA,102723.328,3749.4919,N,12700.0078,E,1,03,7.9,28.0,M,19.6,M,0.0,0000*40\n"
    "$GPGSV,3,1,10,03,86,244,00,19,51,218,38,16,51,057,00,07,40,048,00*77\n"
    "$GPGSV,3,2,10,13,34,279,00,23,33,236,00,15,29,076,40,25,25,143,38*71\n"
    "$GPRMC,102723.328,A,3749.4919,N,12700.0078,E,0.0,0.0,050623,,,A*68\n"
    "$GPGSV,3,3,10,21,18,051,,27,12,315,*77\n"
    "$GPGSA,A,3,07,08,09,10,,,,,,,,,2.0,1.8,1.0*30\n";

int uartBufferIndex = 0; // UART 버퍼에서 읽어온 데이터의 위치

// UART에서 데이터를 읽는 함수 (시뮬레이션)
int uartRead(char *data, int maxLength) {
    int i = 0;
    while (i < maxLength - 1 && uartBuffer[uartBufferIndex] != '\0') {
        data[i] = uartBuffer[uartBufferIndex++];
        if (data[i] == '\n') break; // 한 줄의 데이터를 완전히 읽었을 때
        i++;
    }
    data[i] = '\0'; // 문자열 끝에 NULL 추가
    return i; // 읽어온 데이터의 길이 반환
}

// GPS 데이터 파싱
void parseGPS(char *inputData, char *output[MAX_TOKEN]) {
    int j = 0;
    output[j++] = inputData;
    for (int i = 0; inputData[i] != '\0'; i++) {
        if (inputData[i] == ',' || inputData[i] == '*') {
            inputData[i] = '\0';
            output[j++] = &inputData[i + 1];
            if (j >= MAX_TOKEN) break;
        }
    }
}

// 공통 메시지 처리
void handleGPSMessage(char *tokens[]) {
    if (strncmp(tokens[0], "$GPGGA", 6) == 0) {
        printf("====== GPGGA Data ======\n");
        printf("UTC Time   : %s\n", tokens[1]);
        printf("Latitude   : %s %s\n", tokens[2], tokens[3]);
        printf("Longitude  : %s %s\n", tokens[4], tokens[5]);
        printf("Fix Quality: %s\n", tokens[6]);
        printf("Satellites: %s\n", tokens[7]);
        printf("Altitude: %s %s\n", tokens[9], tokens[10]);
    } else if (strncmp(tokens[0], "$GPGSV", 6) == 0) {
        printf("=== GPGSV Data ===\n");
        printf("Sentences: %s\n", tokens[1]);
        printf("Sentence Number: %s\n", tokens[2]);
        printf("Satellites: %s\n", tokens[3]);
        for (int i = 4; tokens[i] != NULL && i + 3 < MAX_TOKEN; i += 4) {
            printf("Satellite PRN: %s, Elevation: %s, Azimuth: %s, SNR: %s\n",
                   tokens[i], tokens[i + 1], tokens[i + 2], tokens[i + 3]);
        }
    } else if (strncmp(tokens[0], "$GPRMC", 6) == 0) {
        printf("====== GPRMC Data ======\n");
        printf("UTC Time   : %s\n", tokens[1]);
        printf("Status     : %s\n", tokens[2]);
        printf("Latitude   : %s %s\n", tokens[3], tokens[4]);
        printf("Longitude  : %s %s\n", tokens[5], tokens[6]);
        printf("Speed over Ground: %s knots\n", tokens[7]);
        printf("Course over Ground: %s degrees\n", tokens[8]);
        printf("Date: %s\n", tokens[9]);
    } else if (strncmp(tokens[0], "$GPGSA", 6) == 0) {
        printf("====== GPGSA Data ======\n");
        printf("Mode: %s\n", tokens[1]);
        printf("Fix Type: %s\n", tokens[2]);
        printf("Satellites Used: ");
        for (int i = 3; i <= 14 && tokens[i] != NULL; i++) {
            if (tokens[i][0] != '\0') {
                printf("%s ", tokens[i]);
            }
        }
        printf("\nPDOP: %s, HDOP: %s, VDOP: %s\n", tokens[15], tokens[16], tokens[17]);
    } else {
        printf("Unknown NMEA Message: %s\n", tokens[0]);
    }
}

int main(void) {
    char inputData[MAX_BUFFER_SIZE];
    char *tokens[MAX_TOKEN] = {0};

    while (1) {
        // UART 데이터 읽기
        if (uartRead(inputData, MAX_BUFFER_SIZE) > 0) {
            printf("Received: %s\n", inputData);

            // 데이터 파싱
            parseGPS(inputData, tokens);

            // 메시지 처리
            handleGPSMessage(tokens);
        } else {
            break; // UART 데이터 없음
        }
    }

    return 0;
}