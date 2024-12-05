#include "gps.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// GPS 전역 변수
GPS_t GPS;
static uint8_t rx_data;
static char rx_buffer[GPS_BUFFER_SIZE];
static int rx_index = 0;

// UART 핸들러 포인터
static UART_HandleTypeDef *gps_huart;

// 초기화 함수
void GPS_Init(UART_HandleTypeDef *huart) {
    gps_huart = huart;
    HAL_UART_Receive_IT(gps_huart, &rx_data, 1); // UART 인터럽트 시작
}

// UART 인터럽트 콜백
void GPS_UART_Callback(void) {
    if (rx_data != '\n' && rx_index < GPS_BUFFER_SIZE - 1) {
        rx_buffer[rx_index++] = rx_data; // 데이터 저장
    } else {
        rx_buffer[rx_index] = '\0'; // 문자열 종료
        if (GPS_ValidateChecksum(rx_buffer)) { // 체크섬 유효성 검사
            GPS_ParseNMEA(rx_buffer);         // NMEA 문장 파싱
        }
        rx_index = 0; // 버퍼 초기화
    }
    HAL_UART_Receive_IT(gps_huart, &rx_data, 1); // 다음 데이터 수신 준비
}

// NMEA 체크섬 유효성 검사
int GPS_ValidateChecksum(char *nmea) {
    char *checksum_start = strchr(nmea, '*');
    if (!checksum_start || (checksum_start - nmea) > GPS_BUFFER_SIZE - 3) {
        return 0; // '*'가 없거나 위치가 잘못됨
    }

    uint8_t calculated_checksum = 0;
    for (char *ptr = nmea + 1; ptr < checksum_start; ptr++) {
        calculated_checksum ^= *ptr;
    }

    uint8_t received_checksum = (uint8_t)strtol(checksum_start + 1, NULL, 16);
    return calculated_checksum == received_checksum;
}

// NMEA 문장 파싱
void GPS_ParseNMEA(char *nmea) {
    if (strncmp(nmea, "$GPGGA", 6) == 0) { // GGA 문장 처리
        float nmea_latitude, nmea_longitude;
        char ns, ew;
        int fix_status, satellites;
        float altitude;

        if (sscanf(nmea, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%*f,%f", 
                   &GPS.utc_time, &nmea_latitude, &ns, &nmea_longitude, &ew, 
                   &fix_status, &satellites, &altitude) >= 1) {
            GPS.latitude = GPS_ConvertToDecimal(nmea_latitude, ns);
            GPS.longitude = GPS_ConvertToDecimal(nmea_longitude, ew);
            GPS.fix_status = fix_status;
            GPS.satellites = satellites;
            GPS.altitude = altitude;
        }
    }
}

// NMEA 좌표를 십진수 형식으로 변환
float GPS_ConvertToDecimal(float nmea_coord, char direction) {
    int degrees = (int)(nmea_coord / 100);
    float minutes = nmea_coord - degrees * 100;
    float decimal = degrees + (minutes / 60);

    if (direction == 'S' || direction == 'W') {
        decimal = -decimal; // 남위(S)와 서경(W)는 음수로 변환
    }
    return decimal;
}
