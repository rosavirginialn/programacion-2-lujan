#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

// Estructuras
struct HistorialMedico {
    int idConsulta;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
};

struct Paciente {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    
    HistorialMedico* historial;
    int cantidadConsultas;
    int capacidadHistorial;
    
    int* citasAgendadas;
    int cantidadCitas;
    int capacidadCitas;
    
    char alergias[500];
    char observaciones[500];
    
    bool activo;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    
    int* pacientesAsignados;
    int cantidadPacientes;
    int capacidadPacientes;
    
    int* citasAgendadas;
    int cantidadCitas;
    int capacidadCitas;
    
    bool disponible;
};

struct Cita {
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
};

struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    Paciente* pacientes;
    int cantidadPacientes;
    int capacidadPacientes;
    
    Doctor* doctores;
    int cantidadDoctores;
    int capacidadDoctores;
    
    Cita* citas;
    int cantidadCitas;
    int capacidadCitas;
    
    int siguienteIdPaciente;
    int siguienteIdDoctor;
    int siguienteIdCita;
    int siguienteIdConsulta;
};

// Modulo de Gestion de Pacientes
bool comparaCedula(Hospital* hospital, const char* cedula); {
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
// strcmp devuelve 0 si las cadenas son iguales
        if (strcmp(hospital->pacientes[i].cedula, cedula) == 0) {
            return true; // Cedula ya existe
        }
    }
    return false; // Cedula no existe
}

void redimensionarPacientes(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadPacientes * 2;
    // Crear nuevo arreglo de pacientes con mayor capacidad
    Paciente* nuevosPacientes = new Paciente[nuevaCapacidad];
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        nuevosPacientes[i] = hospital->pacientes[i];
    }
    // Liberar memoria del arreglo antiguo y actualizar el puntero
    delete[] hospital->pacientes;
    // Actualizar el puntero hospital con el nuevo arreglo y capacidad
    hospital->pacientes = nuevosPacientes;
    hospital->capacidadPacientes = nuevaCapacidad;
}

Paciente* crearPaciente(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, int edad, char sexo, const char* tipoSangre, const char* telefono, const char* direccion, const char* email) {
    if (comparaCedula(hospital, cedula)) {
        cout << "La cédula ingresada ya se encuentra en el sistema." << endl;
        return nullptr; // Cedula ya existe
    }
    
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        redimensionarPacientes(hospital);
    }
    
    Paciente* nuevoPaciente = &hospital->pacientes[hospital->cantidadPacientes];
    nuevoPaciente->id = hospital->siguienteIdPaciente++;
    // Copiar los datos proporcionados al nuevo paciente
    strncpy(nuevoPaciente->nombre, nombre, sizeof(nuevoPaciente->nombre));
    strncpy(nuevoPaciente->apellido, apellido, sizeof(nuevoPaciente->apellido));
    strncpy(nuevoPaciente->cedula, cedula, sizeof(nuevoPaciente->cedula));
    nuevoPaciente->edad = edad;
    nuevoPaciente->sexo = sexo;
    strncpy(nuevoPaciente->tipoSangre, tipoSangre, sizeof(nuevoPaciente->tipoSangre));
    strncpy(nuevoPaciente->telefono, telefono, sizeof(nuevoPaciente->telefono));
    strncpy(nuevoPaciente->direccion, direccion, sizeof(nuevoPaciente->direccion));
    strncpy(nuevoPaciente->email, email, sizeof(nuevoPaciente->email));
    
    // Inicializar otros campos
    nuevoPaciente->capacidadHistorial = 5;
    nuevoPaciente->cantidadConsultas = 0;
    nuevoPaciente->historial = new HistorialMedico[5];

    nuevoPaciente->capacidadCitas = 5;
    nuevoPaciente->cantidadCitas = 0;
    nuevoPaciente->citasAgendadas = new int[5];
    
    nuevoPaciente->alergias[0] = '\0';
    nuevoPaciente->observaciones[0] = '\0';
    
    nuevoPaciente->activo = true;
    
    hospital->cantidadPacientes++;
    
    return nuevoPaciente;
}