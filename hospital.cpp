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

bool comparaCedula(Hospital* hospital, const char* cedula) {
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

// Crear Paciente
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

// Buscar Paciente por Cedula
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    // Buscar el paciente en el arreglo de pacientes
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        // strcmp devuelve 0 si las cadenas son iguales esto quiere decir que si son iguales entra al if
        if (strcmp(hospital->pacientes[i].cedula, cedula) == 0) {
            // Paciente encontrado
            return &hospital->pacientes[i];
        }
    }
    return nullptr; // Paciente no encontrado
}

// Buscar Paciente por ID
Paciente* buscarPacientePorId(Hospital* hospital, int id) {
    // Buscar el paciente en el arreglo de pacientes
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        if (hospital->pacientes[i].id == id) {
            // Paciente encontrado
            return &hospital->pacientes[i];
        }
    }
    return nullptr; // Paciente no encontrado
}

// Buscar Paciente por Nombre
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad) {
    *cantidad = 0;
    Paciente** resultados = new Paciente*[hospital->cantidadPacientes];
    
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        // Convertir ambos nombres a minúsculas para comparación case-insensitive
        char nombrePaciente[50];
        char nombreBusqueda[50];
        strncpy(nombrePaciente, hospital->pacientes[i].nombre, sizeof(nombrePaciente));
        // sizeof asegura que no se copie más allá del tamaño del array
        strncpy(nombreBusqueda, nombre, sizeof(nombreBusqueda));
        
        for (int j = 0; nombrePaciente[j]; j++) {
            nombrePaciente[j] = tolower(nombrePaciente[j]);
        }
        for (int j = 0; nombreBusqueda[j]; j++) {
            nombreBusqueda[j] = tolower(nombreBusqueda[j]);
        }
        // Buscar coincidencia parcial
        if (strstr(nombrePaciente, nombreBusqueda) != nullptr) {
            resultados[*cantidad] = &hospital->pacientes[i];
            (*cantidad)++;
        }
    }
    // Redimensionar el array de resultados al tamaño exacto
    Paciente** finales = new Paciente*[*cantidad];
    for (int i = 0; i < *cantidad; ++i) {
        finales[i] = resultados[i];
    }
    delete[] resultados;
    
    return finales;
}

// Actualizar Datos del Paciente
bool actualizarDatosPaciente(Hospital* hospital, int id){
    Paciente* paciente = buscarPacientePorId(hospital, id);
    if (paciente == nullptr) {
        cout << "El paciente no fue encontrado en sistema" << endl;
        return false;
    }
    cout << "A continuacion le sera posible modificar los datos del paciente. " << endl;
    
    char modificacion[100];

    cout << "Ingrese el nombre nuevo, el registrado actualmente es: " << paciente->nombre << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->nombre, modificacion, sizeof(paciente->nombre));
    }
    cout << "Ingrese el apellido nuevo, el registrado actualmente es: " << paciente->apellido << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->apellido, modificacion, sizeof(paciente->apellido));
    }
    cout << "Ingrese la edad nueva, la registrada actualmente es: " << paciente->edad << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
        // atoi convierte una cadena a entero
    int nuevaEdad = atoi(modificacion);
    if (nuevaEdad > 0) {
        paciente->edad = nuevaEdad;
        } else {
        cout << "Edad inválida. No se actualizó." << endl;
        }
    }
    cout << "Ingrese el sexo nuevo [F / M], el registrado actualmente es: " << paciente->sexo << ", (ENTER para no modificar): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    if (modificacion[0] == 'M' || modificacion[0] == 'F') {
        paciente->sexo = modificacion[0];
        } else {
        cout << "Sexo inválido. No se actualizó." << endl;
        }
    }
    cout << "Ingrese el tipo de sangre nuevo, el registrado actualmente es: " << paciente->tipoSangre << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->tipoSangre, modificacion, sizeof(paciente->tipoSangre));
    }
    cout << "Ingrese el telefono nuevo, el registrado actualmente es: " << paciente->telefono << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->telefono, modificacion, sizeof(paciente->telefono));
    }
    cout << "Ingrese la direccion nueva, la registrada actualmente es: " << paciente->direccion << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->direccion, modificacion, sizeof(paciente->direccion));
    }
    cout << "Ingrese el email nuevo, el registrado actualmente es: " << paciente->email << ", (en caso de no querer actualizar presione ENTER): ";
    cin.getline(modificacion, sizeof(modificacion));
    if (strlen(modificacion) > 0) {
    strncpy(paciente->email, modificacion, sizeof(paciente->email));
    }

    cout << "Los datos fueron modificados." << endl;
    return true;
}

// Eliminar Paciente
bool eliminarPaciente(Hospital* hospital, int id) {
    int posicion = -1;
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        if (hospital->pacientes[i].id == id) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) {
        cout << "El paciente no se encontro en el sistema" << endl;
        return false;
    }
    Paciente& paciente = hospital->pacientes[posicion];

    delete[] paciente.historial;
    for (int i = 0; i < paciente.cantidadCitas; ++i) {
    cancelarCita(hospital, paciente.citasAgendadas[i]->idCita);
    }

    delete[] paciente.citasAgendadas;
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        Doctor& doctor = hospital->doctores[i];
        for (int j = 0; j < doctor.cantidadPacientes; ++j) {
            if (doctor.pacientesAsignados[j] == id) {
                // Remover paciente del doctor
                for (int k = j; k < doctor.cantidadPacientes - 1; ++k) {
                    doctor.pacientesAsignados[k] = doctor.pacientesAsignados[k + 1];
                }
                doctor.cantidadPacientes--;
                break;
            }
        }
    }
    for (int i = posicion; i < hospital->cantidadPacientes - 1; ++i) {
        hospital->pacientes[i] = hospital->pacientes[i + 1];
    }
    hospital->cantidadPacientes--;
    cout << "El paciente fue eliminado del sistema." << endl;
    return true;
}


// Listar Todos los Pacientes
void listarPacientes(Hospital* hospital) {
    cout << "╔════════════════════════════════════════════════════════════╗ \n";
    cout << "║                    LISTA DE PACIENTES                      ║ \n";
    cout << "╠═════╦═════════════════════╦══════════════╦══════╦══════════╣ \n";
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ CÉDULA       ║ EDAD ║ CONSULTAS║ \n";
    cout << "╠═════╬═════════════════════╬══════════════╬══════╬══════════╣ \n";

    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        Paciente& paciente = hospital->pacientes[i];
            char nombreCompleto[100];
            strcpy(nombreCompleto, paciente->nombre);
            strcat(nombreCompleto, " ");
            strcat(nombreCompleto, paciente->apellido);
        string nombreCompleto = string(paciente->nombre) + " " + paciente->apellido;

        cout << "║ " << setw(3) << left << paciente.id << " ║ " 
        // setw ajusta el ancho del campo, left alinea a la izquierda, right a la derecha
             << setw(21) << left << nombreCompleto << " ║ "
             << setw(13) << left << paciente->cedula << " ║ "
             << setw(4) << right << paciente->edad << " ║ "
             << setw(8) << right << paciente->cantidadConsultas << " ║\n";
    }
    cout << "╚═════╩═════════════════════╩══════════════╩══════╩══════════╝ \n";
}


// Funciones de historial médico (arrays dinámicos)


// Agregar consulta al historial
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta){
    if (paciente->cantidadConsultas >= paciente->capacidadHistorial) {
        int nuevaCapacidad = paciente->capacidadHistorial * 2;
        HistorialMedico* nuevoHistorial = new HistorialMedico[nuevaCapacidad];

        for (int i = 0; i < paciente->cantidadConsultas; ++i) {
            nuevoHistorial[i] = paciente->historial[i];
        }
        delete[] paciente->historial;
        paciente->historial = nuevoHistorial;
        paciente->capacidadHistorial = nuevaCapacidad;
    }
    paciente->historial[paciente->cantidadConsultas] = consulta;
    paciente->cantidadConsultas++;
}

// Obtener historial completo
HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad) {
    // Retornar el historial completo y la cantidad de consultas
    *cantidad = paciente->cantidadConsultas;
    return paciente->historial;
}

// Mostrar historial médico
void mostrarHistorialMedico(Paciente* paciente) {
    cout << "╔══════════════════════════════════════════════════════════════════╗ \n";
    cout << "║                      CONSULTAS DEL PACIENTE                      ║ \n";
    cout << "╠═════════╦══════════╦═══════════════════╦══════════════╦══════════╣ \n";
    cout << "║  FECHA  ║   HORA   ║    DIAGNOSTICO    ║    DOCTOR    ║   COSTO  ║ \n";
    cout << "╠═════════╬══════════╬═══════════════════╬══════════════╬══════════╣ \n";

    for (int i = 0; i < paciente->cantidadConsultas; ++i) {
        HistorialMedico& consulta = paciente->historial[i];

        cout << "║ " << setw(9) << left << consulta.fecha << " ║ "
             << setw(8) << left << consulta.hora << " ║ "
             << setw(19) << left << consulta.diagnostico << " ║ "
             << setw(10) << right << consulta.idDoctor << " ║ "
             << setw(8) << right << fixed << setprecision(2) << consulta.costo << " ║\n";
    }
    cout << "╚═════════╩══════════╩═══════════════════╩══════════════╩══════════╝ \n";
}

// Obtener ultima consulta
HistorialMedico* obtenerUltimaConsulta(Paciente* paciente){
    if (paciente->cantidadConsultas == 0) {
        return nullptr; // No hay consultas
    }
    return &paciente->historial[paciente->cantidadConsultas - 1];
}


// Modulo de Gestion de Doctores

bool comparaCedulaDoctor(Hospital* hospital, const char* cedula) {
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
// strcmp devuelve 0 si las cadenas son iguales
        if (strcmp(hospital->doctores[i].cedula, cedula) == 0) {
            return true; // Cedula ya existe
        }
    }
    return false; // Cedula no existe
}

// Crear Doctor
Doctor* crearDoctor(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta) {
    if (comparaCedulaDoctor(hospital, cedula)) {
        cout << "La cedula ingresada ya se encuentra en el sistema." << endl;
        return nullptr; // Cedula ya existe
    }
    
    if (strlen(nombre) == 0 || strlen(apellido) == 0 || strlen(cedula) == 0 || strlen(especialidad) == 0 || aniosExperiencia < 0 || costoConsulta < 0) {
        cout << "Datos invalidos para crear el doctor." << endl;
        return nullptr;
    }

    if (hospital->cantidadDoctores >= hospital->capacidadDoctores) {
        redimensionarDoctores(hospital);
    }
    
    Doctor* nuevoDoctor = &hospital->doctores[hospital->cantidadDoctores];
    nuevoDoctor->id = hospital->siguienteIdDoctor++;
    // Copiar los datos proporcionados al nuevo doctor
    strncpy(nuevoDoctor->nombre, nombre, sizeof(nuevoDoctor->nombre));
    strncpy(nuevoDoctor->apellido, apellido, sizeof(nuevoDoctor->apellido));
    strncpy(nuevoDoctor->cedula, cedula, sizeof(nuevoDoctor->cedula));
    strncpy(nuevoDoctor->especialidad, especialidad, sizeof(nuevoDoctor->especialidad));
    nuevoDoctor->aniosExperiencia = aniosExperiencia;
    nuevoDoctor->costoConsulta = costoConsulta;
    
    // Inicializar otros campos
    nuevoDoctor->capacidadPacientes = 5;
    nuevoDoctor->cantidadPacientes = 0;
    nuevoDoctor->pacientesAsignados = new int[5];

    nuevoDoctor->capacidadCitas = 5;
    nuevoDoctor->cantidadCitas = 0;
    nuevoDoctor->citasAgendadas = new int[5];
    
    nuevoDoctor->disponible = true;
    strcpy(nuevoDoctor->horarioAtencion, "");
    strcpy(nuevoDoctor->telefono, "");
    strcpy(nuevoDoctor->email, "");
    
    hospital->cantidadDoctores++;
    return nuevoDoctor;
}

// Buscar Doctor por ID
Doctor* buscarDoctorPorId(Hospital* hospital, int id) {
    // Buscar el doctor en el arreglo de doctores
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        if (hospital->doctores[i].id == id) {
            // Doctor encontrado
            return &hospital->doctores[i];
        }
    }
    return nullptr; // Doctor no encontrado
}

// Buscar doctores por especialidad
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad) {
    *cantidad = 0;
    Doctor** resultados = new Doctor*[hospital->cantidadDoctores];
    
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        // Convertir ambos especialidades a minúsculas para comparación case-insensitive
        char especialidadDoctor[50];
        char especialidadBusqueda[50];
        strncpy(especialidadDoctor, hospital->doctores[i].especialidad, sizeof(especialidadDoctor));
        // sizeof asegura que no se copie más allá del tamaño del array
        strncpy(especialidadBusqueda, especialidad, sizeof(especialidadBusqueda));
        
        for (int j = 0; especialidadDoctor[j]; j++) {
            especialidadDoctor[j] = tolower(especialidadDoctor[j]);
        }
        for (int j = 0; especialidadBusqueda[j]; j++) {
            especialidadBusqueda[j] = tolower(especialidadBusqueda[j]);
        }
        // Buscar coincidencia parcial
        if (strstr(especialidadDoctor, especialidadBusqueda) != nullptr) {
            resultados[*cantidad] = &hospital->doctores[i];
            (*cantidad)++;
        }
    }

    if (*cantidad == 0) {
    delete[] resultados;
    return nullptr;
    }

    // Redimensionar el array de resultados al tamaño exacto
    Doctor** finales = new Doctor*[*cantidad];
    for (int i = 0; i < *cantidad; ++i) {
        finales[i] = resultados[i];
    }
    delete[] resultados;
    
    return finales;
}

// Asignar Paciente a Doctor
    bool asignarPacienteADoctor(Doctor* doctor, int idPaciente) {
    // Verificar si el doctor y el paciente son válidos
    if (doctor == nullptr || idPaciente < 0) return false;
    // Verificar si el paciente ya está asignado
    for (int i = 0; i < doctor->cantidadPacientes; ++i) {
        if (doctor->pacientesAsignados[i] == idPaciente) {
            cout << "El paciente ya esta asignado al doctor." << endl;
            return false; // Paciente ya asignado
        }
    }
    // Redimensionar arreglo si esta lleno
    if (doctor->cantidadPacientes >= doctor->capacidadPacientes) {
        int nuevaCapacidad = doctor->capacidadPacientes * 2;
        // Crear nuevo arreglo con mayor capacidad
        int* nuevosPacientes = new int[nuevaCapacidad];
        // Copiar pacientes existentes al nuevo arreglo
        for (int i = 0; i < doctor->cantidadPacientes; ++i) {
            nuevosPacientes[i] = doctor->pacientesAsignados[i];
        }
        // Liberar memoria del arreglo antiguo y actualizar el puntero
        delete[] doctor->pacientesAsignados;
        doctor->pacientesAsignados = nuevosPacientes;
        doctor->capacidadPacientes = nuevaCapacidad;
    }
    doctor->pacientesAsignados[doctor->cantidadPacientes] = idPaciente;
    doctor->cantidadPacientes++;
    cout << "El paciente fue asignado al doctor." << endl;
    return true;
}

// Remover Paciente de Doctor
bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente){
    if (doctor == nullptr || idPaciente < 0) return false;

    int posicion = -1;
    // Buscar la posición del paciente en el arreglo
    for (int i = 0; i < doctor->cantidadPacientes; ++i) {
        if (doctor->pacientesAsignados[i] == idPaciente) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) {
        cout << "El paciente no se encuentra asignado a este doctor." << endl;
        return false; // Paciente no encontrado
    }
    // Remover paciente del arreglo
    for (int i = posicion; i < doctor->cantidadPacientes - 1; ++i) {
        doctor->pacientesAsignados[i] = doctor->pacientesAsignados[i + 1];
    }
    // Disminuir la cantidad de pacientes
    doctor->cantidadPacientes--;
    cout << "El paciente fue removido del doctor." << endl;
    return true;
}

// Listar pacientes asignados a doctor
void listarPacientesDeDoctor(Hospital* hospital, int idDoctor){
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (doctor == nullptr) {
        cout << "El doctor no fue encontrado en el sistema." << endl;
        return;
    }
    cout << "╔════════════════════════════════════════════════════════════╗ \n";
    cout << "║               PACIENTES ASIGNADOS AL DOCTOR                ║ \n";
    cout << "╠═════╦═════════════════════╦══════════════╦══════╦══════════╣ \n";
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ CÉDULA       ║ EDAD ║ CONSULTAS║ \n";
    cout << "╠═════╬═════════════════════╬══════════════╬══════╬══════════╣ \n";

    for (int i = 0; i < doctor->cantidadPacientes; ++i) {
        int idPaciente = doctor->pacientesAsignados[i];
        Paciente* paciente = buscarPacientePorId(hospital, idPaciente);
        if (paciente != nullptr) {
            // Construir nombre completo
            char nombreCompleto[100];
            strcpy(nombreCompleto, paciente->nombre);
            strcat(nombreCompleto, " ");
            strcat(nombreCompleto, paciente->apellido);
            cout << "║ " << setw(3) << left << paciente->id << " ║ "
                 << setw(21) << left << nombreCompleto << " ║ "
                 << setw(13) << left << paciente->cedula << " ║ "
                 << setw(4) << right << paciente->edad << " ║ "
                 << setw(8) << right << paciente->cantidadConsultas << " ║\n";
        }
    }
    cout << "╚═════╩═════════════════════╩══════════════╩══════╩══════════╝ \n";
}

// Listar Doctores
void listarDoctores(Hospital* hospital){
    cout << "╔════════════════════════════════════════════════════════════╗ \n";
    cout << "║                     LISTA DE DOCTORES                      ║ \n";
    cout << "╠═════╦═════════════════════╦══════════════╦═════════════════╣ \n";
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ CÉDULA       ║ ESPECIALIDAD    ║ \n";
    cout << "╠═════╬═════════════════════╬══════════════╬═════════════════╣ \n";

    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        Doctor& doctor = hospital->doctores[i];
            char nombreCompleto[100];
            strcpy(nombreCompleto, doctor->nombre);
            strcat(nombreCompleto, " ");
            strcat(nombreCompleto, doctor->apellido);

        cout << "║ " << setw(3) << left << doctor.id << " ║ " 
        // setw ajusta el ancho del campo, left alinea a la izquierda, right a la derecha
             << setw(21) << left << nombreCompleto << " ║ "
             << setw(13) << left << doctor->cedula << " ║ "
             << setw(17) << right << doctor->especialidad << " ║ \n";
    }
    cout << "╚═════╩═════════════════════╩══════════════╩═════════════════╝ \n";
}

// Eliminar Doctor
bool eliminarDoctor(Hospital* hospital, int id) { 
    int posicion = -1;
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        if (hospital->doctores[i].id == id) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) {
        cout << "El doctor no se encontro en el sistema." << endl;
        return false;
    }
    Doctor& doctor = hospital->doctores[posicion];

    delete[] doctor.historial;
    for (int i = 0; i < doctor.cantidadCitas; ++i) {
        int idCita = doctor.citasAgendadas[i];
    cancelarCita(hospital, idCita);
    }

    delete[] doctor.citasAgendadas;
    delete[] doctor.pacientesAsignados;

    for (int i = posicion; i < hospital->cantidadDoctores - 1; ++i) {
        hospital->doctores[i] = hospital->doctores[i + 1];
    }
    hospital->cantidadDoctores--;
    cout << "El doctor fue eliminado del sistema." << endl;
    return true;
}


// Modulo de Gestion de Citas
