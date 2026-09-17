#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <valgrind/callgrind.h>

#define NUM_SENSORS 1000
#define READINGS_PER_SENSOR 100

int read_sensor_value(){
    return rand() % 1024;
}

std::vector<int>* generate_sensor_data(){
    // Reserva dinámica de memoria para el vector en el heap
    std::vector<int>* readings = new std::vector<int>();

    for (size_t i = 0; i < READINGS_PER_SENSOR; ++i){
        readings->push_back(read_sensor_value());
    }

    return readings;
}

double calculate_average(std::vector<int>* data){
    double sum = 0;

    // Corrección de condición de parada (< en vez de <=) y tipos sin signo
    for (size_t i = 0; i < data->size(); ++i){
        sum += (*data)[i];
    }

    return sum / data->size();
}

int main()
{
    srand(time(0));

    std::vector<double> averages;

    for (size_t i = 0; i < NUM_SENSORS; ++i) {
        // Iniciar instrumentacion de Callgrind (Region de Interes)
        CALLGRIND_START_INSTRUMENTATION;

        std::vector<int>* sensor_data = generate_sensor_data();

        double avg = calculate_average(sensor_data);

        averages.push_back(avg);

        // Detener instrumentacion antes del retardo
        CALLGRIND_STOP_INSTRUMENTATION;

        // =========================================================================
        // CORRECCION DE FUGA DE MEMORIA (Sección 7.1.4 de la Guía):
        // Se invoca 'delete' para ejecutar el destructor de std::vector<int>
        // (liberando su búfer dinámico interno de 512 bytes) y retornar los 
        // 24 bytes del objeto al heap del sistema operativo.
        // =========================================================================
        delete sensor_data;

        usleep(1000);   // aproximadamente 1 ms
    }

    std::cout << "Processed data from "
              << NUM_SENSORS
              << " sensors.\n";

    return 0;
}
