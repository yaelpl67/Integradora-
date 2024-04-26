/*
 * File:   esa16.c
 * Author: YAEL
 *
 * Created on 18 de abril de 2024, 03:42 PM
 */


#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000 // Frecuencia del oscilador externo (8MHz)

#define SENSOR_1_PIN RB0 // Pin del sensor 1 conectado al puerto B0
#define SENSOR_2_PIN RB1 // Pin del sensor 2 conectado al puerto B1

void I2C_Slave_Init(uint8_t address) {
    SSPSTAT = 0x80; // Habilitar la recepción de datos en modo 7 bits
    SSPADD = address << 1; // Configurar dirección del esclavo
    SSPCON = 0x36; // Habilitar modo esclavo I2C con dirección 7 bits
    SSPCON2 = 0x01; // Habilitar detección de stop
    TRISC3 = 1; // SDA como entrada
    TRISC4 = 1; // SCL como entrada
}

void I2C_Slave_Write(uint8_t data) {
    SSPBUF = data;
    while(!SSPIF); // Esperar a que se complete la transmisión
    SSPIF = 0; // Limpiar bandera de interrupción
}

void main() {
    uint16_t dato_sensor1, dato_sensor2;
    
    TRISB = 0xFF; // Puerto B como entrada
    PORTB = 0x00; // Inicializar puerto B en bajo
    
    SSPCON = 0x28; // Modo esclavo I2C con dirección 7 bits
    SSPSTAT = 0x80; // Habilitar la recepción de datos en modo 7 bits
    SSPADD = 0x08; // Dirección arbitraria para el esclavo
    
    while(1) {
        // Leer datos de los sensores YL-69
        dato_sensor1 = SENSOR_1_PIN;
        dato_sensor2 = SENSOR_2_PIN;
        
        // Enviar datos al maestro ESP32 a través de I2C
        I2C_Slave_Write((dato_sensor1 >> 8) & 0xFF); // Envía el byte más significativo primero
        I2C_Slave_Write(dato_sensor1 & 0xFF); // Envía el byte menos significativo
        I2C_Slave_Write((dato_sensor2 >> 8) & 0xFF);
        I2C_Slave_Write(dato_sensor2 & 0xFF);
        
        __delay_ms(1000); // Esperar un segundo antes de la próxima lectura
    }
}
