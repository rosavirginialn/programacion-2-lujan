#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

// Estructuras
struct HistorialMedico {
    int id;
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
    int siguienteIdHistorial;
};

// Llamado a validaciones
bool validarNombre(const char* nombre);
bool validarApellido(const char* apellido);
bool validarFecha(const char* fecha);
bool validarHora(const char* hora);
int compararFechas(const char* fecha1, const char* fecha2);
bool validarCedula(const char* cedula);
bool verificarCedula(Hospital* hospital, const char* cedula);
bool verificarCedulaDoctor(Hospital* hospital, const char* cedula);
bool validarSexo(char sexo);
bool validarTipoSangre(const char* tipoSangre);
bool validarTelefono(const char* telefono);
bool validarEmail(const char* email);


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

void redimensionarArrayPacientes(Hospital* hospital) {
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
    if (!verificarCedula(hospital, cedula)) {
        cout << "La cedula ingresada ya se encuentra en el sistema o es invalida. Por favor utiliza caracteres numericos" << endl;
        return nullptr; // Cedula ya existe
    }
    if (!validarNombre(nombre)) {
    cout << "Nombre inválido. No debe estar vacio ni contener numeros." << endl;
    return nullptr;
    }
    if (!validarApellido(apellido)) {
        cout << "Apellido invalido. No debe estar vacio ni contener numeros." << endl;
        return nullptr;
    }
    if (edad < 0) {
        cout << "Edad invalida. Debe ser a partir de 0." << endl;
        return nullptr;
    }
    if (!validarSexo(sexo)) {
        cout << "Sexo invalido. Usa 'M' para masculino o 'F' para femenino." << endl;
        return nullptr;
    }
    if (!validarTipoSangre(tipoSangre)) {
        cout << "Tipo de sangre invalido. Debe ser uno de los siguientes: A+, A-, B+, B-, AB+, AB-, O+, O-." << endl;
        return nullptr;
    }
    if (!validarTelefono(telefono)) {
        cout << "Telefono invalido. Usa solo numeros entre 7 y 15 dígitos." << endl;
        return nullptr;
    }
    if (!validarEmail(email)) {
        cout << "Email invalido. Verifica se encuentre el '@' y los caracteres validos." << endl;
        return nullptr;
    }

    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        redimensionarArrayPacientes(hospital);
    }
    
    Paciente* nuevoPaciente = &hospital->pacientes[hospital->cantidadPacientes];
    nuevoPaciente->id = hospital->siguienteIdPaciente++;
    // Copiar los datos proporcionados al nuevo paciente
    strncpy(nuevoPaciente->nombre, nombre, sizeof(nuevoPaciente->nombre));
    strncpy(nuevoPaciente->apellido, apellido, sizeof(nuevoPaciente->apellido));
    strncpy(nuevoPaciente->cedula, cedula, sizeof(nuevoPaciente->cedula));
    nuevoPaciente->cedula[sizeof(nuevoPaciente->cedula) - 1] = '\0';    
    cout << "Guardando cédula: [" << cedula << "]" << endl;
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
    cout << "Buscando cédula: [" << cedula << "]" << endl;
    // Buscar el paciente en el arreglo de pacientes
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        cout << "Comparando con: [" << hospital->pacientes[i].cedula << "]" << endl;
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

bool cancelarCita(Hospital* hospital, int idCita);

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
    int idCita = paciente.citasAgendadas[i];
    cancelarCita(hospital, idCita); }

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
            strcpy(nombreCompleto, paciente.nombre);
            strcat(nombreCompleto, " ");
            strcat(nombreCompleto, paciente.apellido);

        cout << "║ " << setw(3) << left << paciente.id << " ║ " 
        // setw ajusta el ancho del campo, left alinea a la izquierda, right a la derecha
             << setw(21) << left << nombreCompleto << " ║ "
             << setw(13) << left << paciente.cedula << " ║ "
             << setw(4) << right << paciente.edad << " ║ "
             << setw(8) << right << paciente.cantidadConsultas << " ║\n";
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

void redimensionarArrayDoctores(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadDoctores * 2;
    Doctor* nuevos = new Doctor[nuevaCapacidad];
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        nuevos[i] = hospital->doctores[i];
    }
    delete[] hospital->doctores;
    hospital->doctores = nuevos;
    hospital->capacidadDoctores = nuevaCapacidad;
}

// Validaciones de doctor
bool validarAniosExperiencia(int anios) {
    return anios >= 0;
}
bool validarCostoConsulta(float costo) {
    return costo >= 0;
}

// Crear Doctor
Doctor* crearDoctor(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta) {
    if (!verificarCedulaDoctor(hospital, cedula)) {
        cout << "La cedula ingresada ya se encuentra en el sistema o es invalida. Por favor utiliza caracteres numericos" << endl;
        return nullptr; // Cedula ya existe
    }
    if (strlen(nombre) == 0 || strlen(apellido) == 0 || strlen(cedula) == 0 || strlen(especialidad) == 0 || aniosExperiencia < 0 || costoConsulta < 0) {
        cout << "Datos invalidos para crear el doctor." << endl;
        return nullptr;
    }
    if (!validarNombre(nombre)) {
        cout << "Nombre invalido. No debe estar vacio ni contener numeros." << endl;
        return nullptr;
    }
    if (!validarApellido(apellido)) {
        cout << "Apellido invalido. No debe estar vacio ni contener numeros." << endl;
        return nullptr;
    }
    if (!validarAniosExperiencia(aniosExperiencia)) {
        cout << "Años de experiencia invalidos. Deben ser mayor a 0." << endl;
        return nullptr;
    }
    if (!validarCostoConsulta(costoConsulta)) {
        cout << "Costo de consulta invalido. Debe ser mayor a 0." << endl;
        return nullptr;
    }

    if (hospital->cantidadDoctores >= hospital->capacidadDoctores) {
        redimensionarArrayDoctores(hospital);
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
            strcpy(nombreCompleto, doctor.nombre);
            strcat(nombreCompleto, " ");
            strcat(nombreCompleto, doctor.apellido);

        cout << "║ " << setw(3) << left << doctor.id << " ║ " 
        // setw ajusta el ancho del campo, left alinea a la izquierda, right a la derecha
             << setw(21) << left << nombreCompleto << " ║ "
             << setw(13) << left << doctor.cedula << " ║ "
             << setw(17) << right << doctor.especialidad << " ║ \n";
    }
    cout << "╚═════╩═════════════════════╩══════════════╩═════════════════╝ \n";
}

bool cancelarCita(Hospital* hospital, int idCita);

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

// Redimensionar citas
   void redimensionarArrayCitas(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadCitas * 2;
    // Crear nuevo arreglo de citas con mayor capacidad
    Cita* nuevasCitas = new Cita[nuevaCapacidad];
    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        nuevasCitas[i] = hospital->citas[i];
    }
    delete[] hospital->citas;
    hospital->citas = nuevasCitas;
    hospital->capacidadCitas = nuevaCapacidad;
}

bool validarFecha(const char* fecha);
bool validarHora(const char* hora);

//Agendar Cita
Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo) {
    Paciente* paciente = buscarPacientePorCedula(hospital, hospital->pacientes[idPaciente].cedula);
    if (paciente == nullptr) {
        cout << "El paciente no se encuentra en sistema." << endl;
        return nullptr;
    }
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (doctor == nullptr) {
        cout << "El doctor no se encuentra en sistema." << endl;
        return nullptr;
    }
    if(!validarFecha(fecha)) {
        cout << "La fecha ingresada no cumple con el formato exigido. Por favor utilice el formato YYYY-MM-DD." << endl;
        return nullptr;
    }
    if(!validarHora(hora)) {
        cout << "La hora ingresada no cumple con el formato exigido. Por favor utilice el formato HH:MM." << endl;
        return nullptr;
    }
    for(int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& citaPlanificada = hospital->citas[i];
        if (citaPlanificada.idDoctor == idDoctor && strcmp(citaPlanificada.fecha, fecha) == 0 && strcmp(citaPlanificada.hora, hora) == 0) {
            cout << "El doctor ya tiene una cita pautada para esa fecha y hora." << endl;
            return nullptr;
        }
    }
    if (hospital->cantidadCitas >= hospital->capacidadCitas) {
        redimensionarArrayCitas(hospital);
    }
    // Crear cita nueva
    Cita& nuevaCita = hospital->citas[hospital->cantidadCitas];
    nuevaCita.id = hospital->cantidadCitas + 1;
    nuevaCita.idPaciente = idPaciente;
    nuevaCita.idDoctor = idDoctor;
    strncpy(nuevaCita.fecha, fecha, sizeof(nuevaCita.fecha));
    strncpy(nuevaCita.hora, hora, sizeof(nuevaCita.hora));
    strncpy(nuevaCita.motivo, motivo, sizeof(nuevaCita.motivo));
    strncpy(nuevaCita.estado, "Agendada", sizeof(nuevaCita.estado));
    hospital->cantidadCitas++;
    cout << "Su cita fue agendada." << endl;
    nuevaCita.atendida = false;

    paciente->citasAgendadas[paciente->cantidadCitas] = nuevaCita.id;
    doctor->citasAgendadas[doctor->cantidadCitas] = nuevaCita.id;

    return &nuevaCita;
}

// Cancelar Cita
bool cancelarCita(Hospital* hospital, int idCita) {
Cita* cita = nullptr;
for (int i = 0; i < hospital->cantidadCitas; ++i)
    {
        if (hospital->citas[i].id == idCita) {
            cita = &hospital->citas[i];
            break;
        }
    }
    if (cita == nullptr) {
        cout << "La cita no se encuentra en sistema." << endl;
        return false;
    }
    strncpy(cita->estado, "Cancelada", sizeof(cita->estado));

    Paciente* paciente = &hospital->pacientes[cita->idPaciente];
    for(int i = 0; i < paciente->cantidadCitas; ++i) {
        if (paciente->citasAgendadas[i] == cita->id) {
            // Remover cita del paciente
            for (int j = i; j < paciente->cantidadCitas - 1; ++j) {
                paciente->citasAgendadas[j] = paciente->citasAgendadas[j + 1];
            }
            paciente->cantidadCitas--;
            break;
        }
    }
    Doctor* doctor = buscarDoctorPorId(hospital, cita->idDoctor);
    for(int i = 0; i < doctor->cantidadCitas; ++i) {
        if (doctor->citasAgendadas[i] == cita->id) {
            // Remover cita del doctor
            for (int j = i; j < doctor->cantidadCitas - 1; ++j) {
                doctor->citasAgendadas[j] = doctor->citasAgendadas[j + 1];
            }
            doctor->cantidadCitas--;
            break;
        }
    }

    cout << "La cita fue cancelada." << endl;
    return true;
}

// Atender cita
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos) {
    Cita* cita = nullptr;
    // Buscar la cita por ID
    for (int i = 0; i < hospital->cantidadCitas; ++i)
    {
        if (hospital->citas[i].id == idCita) {
            cita = &hospital->citas[i];
            break;
        }
    }
    if (cita == nullptr) {
        cout << "La cita no se encuentra en sistema." << endl;
        return false;
    }
    if (strcmp(cita->estado, "Agendada") != 0) {
        cout << "La cita no se encuentra en estado agendada." << endl;
        return false;
    }
    strncpy(cita->estado, "Atendida", sizeof(cita->estado));
    cita->atendida = true;

    HistorialMedico nuevaConsulta;
    nuevaConsulta.id = hospital->siguienteIdHistorial++;
    strncpy(nuevaConsulta.fecha, cita->fecha, sizeof(nuevaConsulta.fecha));
    strncpy(nuevaConsulta.hora, cita->hora, sizeof(nuevaConsulta.hora));
    strncpy(nuevaConsulta.diagnostico, diagnostico, sizeof(nuevaConsulta.diagnostico));
    strncpy(nuevaConsulta.tratamiento, tratamiento, sizeof(nuevaConsulta.tratamiento));
    strncpy(nuevaConsulta.medicamentos, medicamentos, sizeof(nuevaConsulta.medicamentos));
    nuevaConsulta.idDoctor = cita->idDoctor;
    Doctor* doctor = buscarDoctorPorId(hospital, cita->idDoctor);
    nuevaConsulta.costo = doctor->costoConsulta;

    Paciente* paciente = &hospital->pacientes[cita->idPaciente];
    agregarConsultaAlHistorial(paciente, nuevaConsulta);

    cout << "La cita fue atendida." << endl;
    return true;
}

// Obtener citas de paciente
Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad) {
    if(idPaciente < 0 || idPaciente >= hospital->cantidadPacientes) {
        *cantidad = 0;
        return nullptr;
    }
    Paciente* paciente = &hospital->pacientes[idPaciente];
    // Retornar las citas del paciente
    Cita** citas = new Cita*[paciente->cantidadCitas];
    int citasEncontradas = 0;
    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& cita = hospital->citas[i];
        if (cita.idPaciente == idPaciente) {
            citas[citasEncontradas] = &cita;
            citasEncontradas++;
        }
    }
    *cantidad = citasEncontradas;
    return citas;
}

// Obtener citas de doctor
Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad){
   if(idDoctor < 0 || idDoctor >= hospital->cantidadDoctores) {
        *cantidad = 0;
        return nullptr;
    }
    Doctor* doctor = &hospital->doctores[idDoctor];
    // Retornar las citas del paciente
    Cita** citas = new Cita*[doctor->cantidadCitas];
    int citasEncontradas = 0;
    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& cita = hospital->citas[i];
        if (cita.idDoctor == idDoctor) {
            citas[citasEncontradas] = &cita;
            citasEncontradas++;
        }
    }
    *cantidad = citasEncontradas;
    return citas;
}

// Obtener citas por fecha
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad){
    Cita** citas = new Cita*[hospital->cantidadCitas];
    int citasEncontradas = 0;
    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& cita = hospital->citas[i];
        if (strcmp(cita.fecha, fecha) == 0) {
            citas[citasEncontradas] = &cita;
            citasEncontradas++;
        }
    }
    *cantidad = citasEncontradas;
    return citas;
}

// Listar citas pendientes
void listarCitasPendientes(Hospital* hospital) {
    cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                               CITAS PENDIENTES                            ║\n";
    cout << "╠═════╦══════════════════╦══════════════════╦════════════╦════════╦═════════╣\n";
    cout << "║ ID  ║ PACIENTE         ║ DOCTOR           ║ FECHA      ║ HORA   ║ ESTADO  ║\n";
    cout << "╠═════╬══════════════════╬══════════════════╬════════════╬════════╬═════════╣\n";

    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& cita = hospital->citas[i];
        if (strcmp(cita.estado, "Agendada") == 0) {
            Paciente* paciente = &hospital->pacientes[cita.idPaciente];
            Doctor* doctor = buscarDoctorPorId(hospital, cita.idDoctor);

            char nombrePaciente[100];
            strcpy(nombrePaciente, paciente->nombre);
            strcat(nombrePaciente, " ");
            strcat(nombrePaciente, paciente->apellido);
            char nombreDoctor[100];
            strcpy(nombreDoctor, doctor->nombre);
            strcat(nombreDoctor, " ");
            strcat(nombreDoctor, doctor->apellido);

            cout << "║ " << setw(3) << left << cita.id << " ║ "
                 << setw(18) << left << nombrePaciente << " ║ "
                 << setw(18) << left << nombreDoctor << " ║ "
                 << setw(10) << left << cita.fecha << " ║ "
                 << setw(6) << left << cita.hora << " ║ "
                 << setw(8) << left << cita.estado << " ║\n";
        }
    }
    cout << "╚═════╩══════════════════╩══════════════════╩════════════╩════════╩═════════╝\n";
}

// Verificar disponibilidad
bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora){
    for(int i = 0; i < hospital->cantidadCitas; ++i) {
        Cita& citaPlanificada = hospital->citas[i];
        if (citaPlanificada.idDoctor == idDoctor && strcmp(citaPlanificada.fecha, fecha) == 0 && strcmp(citaPlanificada.hora, hora) == 0) {
            return false;
        }
    }
    return true; // Disponible
}


// Modulo de utilidades y helpers

// Inicializar hospital
Hospital* inicializarHospital(const char* nombre, int capacidadInicial) {
   Hospital* hospital = new Hospital;

   strncpy(hospital->nombre, nombre, sizeof(hospital->nombre));

    // Inicializar campos de texto como vacíos
    hospital->direccion[0] = '\0';
    hospital->telefono[0] = '\0';

    // Inicializar pacientes
    hospital->cantidadPacientes = 0;
    hospital->capacidadPacientes = capacidadInicial;
    hospital->pacientes = new Paciente[capacidadInicial];

    // Inicializar doctores
    hospital->cantidadDoctores = 0;
    hospital->capacidadDoctores = capacidadInicial;
    hospital->doctores = new Doctor[capacidadInicial];

    // Inicializar citas
    hospital->cantidadCitas = 0;
    hospital->capacidadCitas = capacidadInicial;
    hospital->citas = new Cita[capacidadInicial];

    // Inicializar IDs
    hospital->siguienteIdPaciente = 1;
    hospital->siguienteIdDoctor = 1;
    hospital->siguienteIdCita = 1;
    hospital->siguienteIdConsulta = 1;
    hospital->siguienteIdHistorial = 1;

    return hospital;
}

// Destruir hospital
void destruirHospital(Hospital* hospital) {
    if(hospital == nullptr) return;

    for(int i = 0; i < hospital->cantidadPacientes; ++i) {
        delete[] hospital->pacientes[i].historial;
        delete[] hospital->pacientes[i].citasAgendadas;
    }
    delete[] hospital->pacientes;
    for(int i = 0; i < hospital->cantidadDoctores; ++i) {
        delete[] hospital->doctores[i].pacientesAsignados;
        delete[] hospital->doctores[i].citasAgendadas;
    }
    delete[] hospital->doctores;
    delete[] hospital->citas;
    delete hospital;
}


// Validaciones

// Validar nombre
bool validarNombre(const char* nombre) {
    if (nombre == nullptr || nombre[0] == '\0') return false; // Vacio
    for (int i = 0; nombre[i] != '\0'; ++i) {
        if (isdigit(nombre[i])) return false; // No debe contener numeros
    }
    return true;
}

// Validar apellido
bool validarApellido(const char* apellido) {
    if (apellido == nullptr || apellido[0] == '\0') return false; // Vacio
    for (int i = 0; apellido[i] != '\0'; ++i) {
        if (isdigit(apellido[i])) return false; // No debe contener numeros
    }
    return true;
}

// Validar fecha
bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false; // Formato incorrecto
    if(!isdigit(fecha[0]) || !isdigit(fecha[1]) || !isdigit(fecha[2]) || !isdigit(fecha[3]) || fecha[4]  != '-' || 
        !isdigit(fecha[5]) || !isdigit(fecha[6]) || fecha[7] != '-' ||
        !isdigit(fecha[8]) || !isdigit(fecha[9])) {
        return false; // Formato incorrecto
    }
    int anio = atoi(fecha);
    int mes = atoi(fecha + 5);
    int dia = atoi(fecha + 8);

    if (mes < 1 || mes > 12) return false;
    int diasPorMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (mes == 2) {
        // Verificar año bisiesto
        if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
            diasPorMes[1] = 29;
        }
    }
        if (dia < 1 || dia > diasPorMes[mes - 1]) return false;
    return true;
}

// Validar hora
bool validarHora(const char* hora) {
    if (strlen(hora) != 5) return false; // Formato incorrecto
    if(!isdigit(hora[0]) || !isdigit(hora[1]) || hora[2] != ':' || !isdigit(hora[3]) || !isdigit(hora[4])) {
        return false; // Formato incorrecto
    }
    // Extraer horas y minutos
    int horas = atoi(hora);
    int minutos = atoi(hora + 3);

    if (horas < 0 || horas > 23) return false;
    if (minutos < 0 || minutos > 59) return false;
    return true;
}

// Comparar fechas
int compararFechas(const char* fecha1, const char* fecha2) {
    int anio1 = atoi(fecha1);
    int mes1 = atoi(fecha1 + 5);
    int dia1 = atoi(fecha1 + 8);
    int anio2 = atoi(fecha2);
    int mes2 = atoi(fecha2 + 5);
    int dia2 = atoi(fecha2 + 8);

    if (anio1 < anio2) return -1;
    if (anio1 > anio2) return 1;
    if (mes1 < mes2) return -1;
    if (mes1 > mes2) return 1;
    if (dia1 < dia2) return -1;
    if (dia1 > dia2) return 1;
    return 0; // Fechas iguales
}

// Validar cédula
bool validarCedula(const char* cedula) {
        if(cedula == nullptr || cedula[0] == '\0') return false; // Vacia
        if (strlen(cedula) < 7 || strlen(cedula) > 10) return false; // Longitud inválida
        for (int i = 0; cedula[i] != '\0'; ++i) {
            if (!isdigit(cedula[i])) return false; // Caracter no numérico
        }
        return true;
}

// Verificar cedula
bool verificarCedula(Hospital* hospital, const char* cedula) {
   if (!validarCedula(cedula)) {
        cout << "La cédula ingresada no es valida." << endl;
        return false; // Cedula invalida
    }
    if (comparaCedula(hospital, cedula)) {
        cout << "La cédula ingresada ya se encuentra en el sistema." << endl;
        return false; // Cedula ya existe
    }
    return true;
}

// Verificar cedula doctor
bool verificarCedulaDoctor(Hospital* hospital, const char* cedula) {
   if (!validarCedula(cedula)) {
        cout << "La cédula ingresada no es valida." << endl;
        return false; // Cedula invalida
    }
    if (comparaCedulaDoctor(hospital, cedula)) {
        cout << "La cédula ingresada ya se encuentra en el sistema." << endl;
        return false; // Cedula ya existe
    }
    return true;
}

// Validar sexo
bool validarSexo(char sexo) {
    return (sexo == 'M' || sexo == 'F');
}

// Validar tipo de sangre
bool validarTipoSangre(const char* tipoSangre) {
    const char* tiposValidos[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (const char* tipo : tiposValidos) {
        // Comparar con cada tipo válido
        if (strcmp(tipoSangre, tipo) == 0) {
            return true;
        }
    }
    return false;
}

// Validar telefono
bool validarTelefono(const char* telefono) {
    if(telefono == nullptr || telefono[0] == '\0') return false; // Vacio
    int longitud = strlen(telefono);
    if (longitud < 7 || longitud > 15) return false;
    for (int i = 0; i < longitud; ++i) {
        if (!isdigit(telefono[i])) {
            return false; // Caracter invalido
        }
    }
    return true;
}

// Validar email
bool validarEmail(const char* email) {
    if(email == nullptr || email[0] == '\0') return false; // Vacio

    const char* arroba = strchr(email, '@');
    if (arroba == nullptr || arroba == email) return false; // No contiene '@'
    const char* punto = strchr(arroba, '.');
    if (punto == nullptr || punto < arroba) return false; // No contiene '.' después de '@'
    return true;
}


// Funciones de copia

// Copiar string
char* copiarString(const char* origen){
    if (origen == nullptr) return nullptr;
    size_t longitud = strlen(origen);
    // Crear nuevo arreglo y copiar contenido
    char* copia = new char[longitud + 1];
    strcpy(copia, origen);
    return copia;
}

// Deep cppy de paciente
Paciente* copiarPaciente(Paciente* original){
    if(original == nullptr) return nullptr;
    Paciente* copia = new Paciente;

    copia->id = original->id;
    strcpy(copia->nombre, original->nombre);
    strcpy(copia->apellido, original->apellido);
    strcpy(copia->cedula, original->cedula);
    copia->edad = original->edad;
    copia->sexo = original->sexo;
    strcpy(copia->tipoSangre, original->tipoSangre);
    strcpy(copia->telefono, original->telefono);
    strcpy(copia->direccion, original->direccion);
    strcpy(copia->email, original->email);

    copia->cantidadConsultas = original->cantidadConsultas;
    copia->capacidadHistorial = original->capacidadHistorial;

    copia->historial = new HistorialMedico[copia->capacidadHistorial];
    for (int i = 0; i < original->cantidadConsultas; ++i) {
        copia->historial[i] = original->historial[i];
    }

    copia->cantidadCitas = original->cantidadCitas;
    copia->capacidadCitas = original->capacidadCitas;

    copia->citasAgendadas = new int[copia->capacidadCitas];
    for (int i = 0; i < original->cantidadCitas; ++i) {
        copia->citasAgendadas[i] = original->citasAgendadas[i];
    }
    return copia;
}



// Interfaz de usuario

int main() {
    Hospital* hospital = inicializarHospital("Hospital Clinico", 8); 
    int opcion;

    do {
        system("cls");
        cout << "\t\t╔══════════════════════════════════════════════════════╗\n";
        cout << "\t\t║              SISTEMA DE GESTION HOSPITALARIA         ║\n";
        cout << "\t\t╠══════════════════════════════════════════════════════╣\n";
        cout << "\t\t║ 1. Gestion de Pacientes                              ║\n";
        cout << "\t\t║ 2. Gestion de Doctores                               ║\n";
        cout << "\t\t║ 3. Gestion de Citas                                  ║\n";
        cout << "\t\t║ 4. Salir                                             ║\n";
        cout << "\t\t╚══════════════════════════════════════════════════════╝\n";
        cout << "\t\tSeleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                int opModuloPaciente;
                do {
                    system("cls");
                    cout << "\t\t╔══════════════════════════════════════════════════════╗\n";
                    cout << "\t\t║              GESTIÓN DE PACIENTES                    ║\n";
                    cout << "\t\t╠══════════════════════════════════════════════════════╣\n";
                    cout << "\t\t║ 1. Registrar nuevo paciente                          ║\n";
                    cout << "\t\t║ 2. Buscar paciente por cédula                        ║\n";
                    cout << "\t\t║ 3. Buscar paciente por nombre                        ║\n";
                    cout << "\t\t║ 4. Ver historial médico completo                     ║\n";
                    cout << "\t\t║ 5. Actualizar datos del paciente                     ║\n";
                    cout << "\t\t║ 6. Listar todos los pacientes                        ║\n";
                    cout << "\t\t║ 7. Eliminar paciente                                 ║\n";
                    cout << "\t\t║ 0. Volver al menú principal                          ║\n";
                    cout << "\t\t╚══════════════════════════════════════════════════════╝\n";
                    cout << "\t\tSeleccione una opción: ";
                    cin >> opModuloPaciente;
                    cin.ignore();

                    switch(opModuloPaciente) {
                        case 1: {
                            char nombre[50], apellido[50], cedula[20], tipoSangre[5], telefono[15], direccion[100], email[50];
                            int edad;
                            char sexo;

                            cout << "Nombre: "; cin.getline(nombre, 50); 
                            cout << "Apellido: "; cin.getline(apellido, 50);
                            cout << "Cédula: "; cin.getline(cedula, 20);
                            cout << "Edad: "; cin >> edad; cin.ignore();
                            cout << "Sexo (M/F): "; cin >> sexo; cin.ignore();
                            cout << "Tipo de sangre: "; cin.getline(tipoSangre, 5);
                            cout << "Teléfono: "; cin.getline(telefono, 15);
                            cout << "Dirección: "; cin.getline(direccion, 100);
                            cout << "Email: "; cin.getline(email, 50);

                            Paciente* nuevo = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo, tipoSangre, telefono, direccion, email);
                            if (nuevo != nullptr) {
                                cout << "Paciente creado con ID: " << nuevo->id << endl;
                            }
                            system("pause");
                            break;
                        }
                        case 2: {
                            char cedula[20];
                            cin.ignore(1000, '\n'); 
                            cout << "Ingrese la cédula: ";
                            cin.getline(cedula, 20);
                            Paciente* p = buscarPacientePorCedula(hospital, cedula);
                            if (p != nullptr) {
                                cout << "Paciente: " << p->nombre << " " << p->apellido << ", Edad: " << p->edad << endl;
                            } else {
                                cout << "No se encontró el paciente." << endl;
                            }
                            system("pause");
                            break;
                            }
                        case 3: {
                            char nombre[50];
                            cout << "Ingrese el nombre: ";
                            cin.ignore(); cin.getline(nombre, 50);
                            int cantidad = 0;
                            Paciente** resultados = buscarPacientesPorNombre(hospital, nombre, &cantidad);
                            if (cantidad > 0) {
                                for (int i = 0; i < cantidad; ++i) {
                                    cout << "- " << resultados[i]->nombre << " " << resultados[i]->apellido << " (ID: " << resultados[i]->id << ")\n";
                                }
                            } else {
                                cout << "No se encontraron pacientes." << endl;
                            }
                            delete[] resultados;
                            system("pause");
                            break;
                        }
                        case 4: {
                            int id;
                            cout << "Ingrese el ID del paciente: ";
                            cin >> id; cin.ignore();

                            Paciente* paciente = buscarPacientePorId(hospital, id);
                            if (paciente == nullptr) {
                                cout << "Paciente no encontrado." << endl;
                            } else if (paciente->cantidadConsultas == 0) {
                                cout << "Este paciente no tiene historial médico registrado." << endl;
                            } else {
                                cout << "Historial médico de: " << paciente->nombre << " " << paciente->apellido << endl;
                                mostrarHistorialMedico(paciente);
                            }
                            system("pause");
                            break;
                        }
                        case 5: {
                            int id;
                            cout << "Ingrese el ID del paciente: ";
                            cin >> id; cin.ignore();
                            actualizarDatosPaciente(hospital, id);
                            system("pause");
                            break;
                        }
                        case 6:{
                            listarPacientes(hospital);
                            system("pause");
                            break;
                        }
                        case 7: {
                            int id;
                            cout << "Ingrese el ID del paciente a eliminar: ";
                            cin >> id; cin.ignore();
                            eliminarPaciente(hospital, id);
                            system("pause");
                            break;
                        }
                        case 0:
                            cout << "Volviendo al menú principal..." << endl;
                            system("pause");
                            break;
                        default:
                            cout << "Opción inválida. Intente de nuevo." << endl;
                            system("pause");
                            break;
                    }
                } while (opModuloPaciente != 0);
                break;
            }

            case 2: {
                int opModuloDoctor;
                do {
                    system("cls");
                    cout << "\t\t╔══════════════════════════════════════════════════════╗\n";
                    cout << "\t\t║              GESTIÓN DE DOCTORES                     ║\n";
                    cout << "\t\t╠══════════════════════════════════════════════════════╣\n";
                    cout << "\t\t║ 1. Registrar nuevo doctor                            ║\n";
                    cout << "\t\t║ 2. Buscar doctor por ID                              ║\n";
                    cout << "\t\t║ 3. Buscar doctores por especialidad                  ║\n";
                    cout << "\t\t║ 4. Asignar paciente a doctor                         ║\n";
                    cout << "\t\t║ 5. Ver pacientes asignados a doctor                  ║\n";
                    cout << "\t\t║ 6. Listar todos los doctores                         ║\n";
                    cout << "\t\t║ 7. Eliminar doctor                                   ║\n";
                    cout << "\t\t║ 0. Volver al menú principal                          ║\n";
                    cout << "\t\t╚══════════════════════════════════════════════════════╝\n";
                    cout << "\t\tSeleccione una opción: ";
                    cin >> opModuloDoctor;
                    cin.ignore();

                    switch (opModuloDoctor) {
                        case 1: {
                            char nombre[50], apellido[50], cedula[20], tipoSangre[5], telefono[15], direccion[100], email[50];
                            char edadTexto[10], sexoTexto[5];
                            int edad;
                            char sexo;
                            cin.ignore(1000, '\n');
                            cout << "Nombre: "; cin.getline(nombre, 50);
                            if (!validarNombre(nombre)) {
                                cout << "Nombre inválido. No debe estar vacío ni contener números." << endl;
                                break;
                            }
                            cout << "Apellido: "; cin.getline(apellido, 50);
                            if (!validarApellido(apellido)) {
                                cout << "Apellido inválido. No debe estar vacío ni contener números." << endl;
                                break;
                            }
                            cout << "Cédula: "; cin.getline(cedula, 20);
                            if (!verificarCedula(hospital, cedula)) {
                                cout << "La cédula ingresada ya se encuentra en el sistema o es inválida." << endl;
                                break;
                            }
                            edad = atoi(edadTexto);
                            if (edad < 0 || edad > 120) {
                                cout << "Edad fuera de rango." << endl;
                                break;
                            }
                            cout << "Sexo (M/F): "; cin.getline(sexoTexto, 5);
                            sexo = toupper(sexoTexto[0]);
                            if (!validarSexo(sexo)) {
                                cout << "Sexo inválido. Usa 'M' o 'F'." << endl;
                                break;
                            }
                            cout << "Tipo de sangre: "; cin.getline(tipoSangre, 5);
                            if (!validarTipoSangre(tipoSangre)) {
                                cout << "Tipo de sangre inválido." << endl;
                                break;
                            }
                            cout << "Teléfono: "; cin.getline(telefono, 15);
                            if (!validarTelefono(telefono)) {
                                cout << "Teléfono inválido. Usa solo números entre 7 y 15 dígitos." << endl;
                                break;
                            }
                            cout << "Dirección: "; cin.getline(direccion, 100);
                            cout << "Email: "; cin.getline(email, 50);
                            if (!validarEmail(email)) {
                                cout << "Email inválido." << endl;
                                break;
                            }
                            Paciente* nuevo = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo, tipoSangre, telefono, direccion, email);
                            if (nuevo != nullptr) {
                                cout << "Paciente creado con ID: " << nuevo->id << endl;
                            }
                            system("pause");
                            break;
                        }

                        case 2: {
                            int id;
                            cout << "Ingrese el ID del doctor: ";
                            cin >> id; cin.ignore();
                            Doctor* doctor = buscarDoctorPorId(hospital, id);
                            if (doctor != nullptr) {
                                cout << "Doctor encontrado: " << doctor->nombre << " " << doctor->apellido
                                    << ", Especialidad: " << doctor->especialidad << ", Costo: $" << doctor->costoConsulta << endl;
                            } else {
                                cout << "No se encontró el doctor." << endl;
                            }
                            system("pause");
                            break;
                        }

                        case 3: {
                            char especialidad[50];
                            cout << "Ingrese la especialidad: ";
                            cin.getline(especialidad, 50);
                            int cantidad = 0;
                            Doctor** resultados = buscarDoctoresPorEspecialidad(hospital, especialidad, &cantidad);
                            if (resultados != nullptr && cantidad > 0) {
                                for (int i = 0; i < cantidad; ++i) {
                                    cout << "- " << resultados[i]->nombre << " " << resultados[i]->apellido
                                        << " (ID: " << resultados[i]->id << ", Costo: $" << resultados[i]->costoConsulta << ")\n";
                                }
                                delete[] resultados;
                            } else {
                                cout << "No se encontraron doctores con esa especialidad." << endl;
                            }
                            system("pause");
                            break;
                        }

                        case 4: {
                            int idDoctor, idPaciente;
                            cout << "Ingrese el ID del doctor: ";
                            cin >> idDoctor; cin.ignore();
                            cout << "Ingrese el ID del paciente: ";
                            cin >> idPaciente; cin.ignore();

                            Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
                            if (doctor != nullptr) {
                                asignarPacienteADoctor(doctor, idPaciente);
                            } else {
                                cout << "Doctor no encontrado." << endl;
                            }
                            system("pause");
                            break;
                        }

                        case 5: {
                            int id;
                            cout << "Ingrese el ID del doctor: ";
                            cin >> id; cin.ignore();
                            listarPacientesDeDoctor(hospital, id);
                            system("pause");
                            break;
                        }

                        case 6:
                            listarDoctores(hospital);
                            system("pause");
                            break;

                        case 7: {
                            int id;
                            cout << "Ingrese el ID del doctor a eliminar: ";
                            cin >> id; cin.ignore();
                            eliminarDoctor(hospital, id);
                            system("pause");
                            break;
                        }

                        case 0:
                            cout << "Volviendo al menú principal..." << endl;
                            system("pause");
                            break;

                        default:
                            cout << "Opción inválida. Intente de nuevo." << endl;
                            system("pause");
                            break;
                    }
                } while (opModuloDoctor != 0);
                break;
            }

case 3: {
    int opModuloCitas;
    do {
        system("cls");
        cout << "\t\t╔═══════════════════════════════════════════════════════╗\n";
        cout << "\t\t║                   GESTIÓN DE CITAS                    ║\n";
        cout << "\t\t╠═══════════════════════════════════════════════════════╣\n";
        cout << "\t\t║ 1. Agendar nueva cita                                 ║\n";
        cout << "\t\t║ 2. Cancelar cita                                      ║\n";
        cout << "\t\t║ 3. Atender cita                                       ║\n";
        cout << "\t\t║ 4. Ver citas de un paciente                           ║\n";
        cout << "\t\t║ 5. Ver citas de un doctor                             ║\n";
        cout << "\t\t║ 6. Ver citas de una fecha                             ║\n";
        cout << "\t\t║ 7. Ver citas pendientes                               ║\n";
        cout << "\t\t║ 0. Volver al menú principal                           ║\n";
        cout << "\t\t╚══════════════════════════════════════════════════════╝\n";
        cout << "\t\tSeleccione una opción: ";
        cin >> opModuloCitas;
        cin.ignore();

        switch (opModuloCitas) {
            case 1: {
                int idPaciente, idDoctor;
                char fecha[11], hora[6], motivo[100];

                cout << "ID del paciente: "; cin >> idPaciente; cin.ignore();
                cout << "ID del doctor: "; cin >> idDoctor; cin.ignore();
                cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                cout << "Hora (HH:MM): "; cin.getline(hora, 6);
                cout << "Motivo de la cita: "; cin.getline(motivo, 100);

                agendarCita(hospital, idPaciente, idDoctor, fecha, hora, motivo);
                system("pause");
                break;
            }

            case 2: {
                int idCita;
                cout << "Ingrese el ID de la cita a cancelar: ";
                cin >> idCita; cin.ignore();
                cancelarCita(hospital, idCita);
                system("pause");
                break;
            }

            case 3: {
                int idCita;
                char diagnostico[100], tratamiento[100], medicamentos[100];

                cout << "ID de la cita: "; cin >> idCita; cin.ignore();
                cout << "Diagnóstico: "; cin.getline(diagnostico, 100);
                cout << "Tratamiento: "; cin.getline(tratamiento, 100);
                cout << "Medicamentos: "; cin.getline(medicamentos, 100);

                atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos);
                system("pause");
                break;
            }

            case 4: {
                int idPaciente;
                cout << "Ingrese el ID del paciente: ";
                cin >> idPaciente; cin.ignore();
                int cantidad = 0;
                Cita** citas = obtenerCitasDePaciente(hospital, idPaciente, &cantidad);
                if (cantidad == 0) {
                    cout << "No se encontraron citas para este paciente." << endl;
                } else {
                    for (int i = 0; i < cantidad; ++i) {
                        Cita* c = citas[i];
                        cout << "- Cita ID: " << c->id << ", Fecha: " << c->fecha << ", Hora: " << c->hora << ", Estado: " << c->estado << endl;
                    }
                }
                delete[] citas;
                system("pause");
                break;
            }

            case 5: {
                int idDoctor;
                cout << "Ingrese el ID del doctor: ";
                cin >> idDoctor; cin.ignore();
                int cantidad = 0;
                Cita** citas = obtenerCitasDeDoctor(hospital, idDoctor, &cantidad);
                if (cantidad == 0) {
                    cout << "No se encontraron citas para este doctor." << endl;
                } else {
                    for (int i = 0; i < cantidad; ++i) {
                        Cita* c = citas[i];
                        cout << "- Cita ID: " << c->id << ", Fecha: " << c->fecha << ", Hora: " << c->hora << ", Estado: " << c->estado << endl;
                    }
                }
                delete[] citas;
                system("pause");
                break;
            }

            case 6: {
                char fecha[11];
                cout << "Ingrese la fecha (YYYY-MM-DD): ";
                cin.ignore(); cin.getline(fecha, 11);
                int cantidad = 0;
                Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                if (cantidad == 0) {
                    cout << "No hay citas para esa fecha." << endl;
                } else {
                    for (int i = 0; i < cantidad; ++i) {
                        Cita* c = citas[i];
                        cout << "- Cita ID: " << c->id << ", Paciente ID: " << c->idPaciente << ", Doctor ID: " << c->idDoctor << ", Hora: " << c->hora << ", Estado: " << c->estado << endl;
                    }
                }
                delete[] citas;
                system("pause");
                break;
            }

            case 7:
                listarCitasPendientes(hospital);
                system("pause");
                break;

            case 0:
                cout << "Volviendo al menú principal..." << endl;
                system("pause");
                break;

            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                system("pause");
                break;
            }
    } while (opModuloCitas != 0);
    break;
}

            case 4:
                cout << "Saliendo del sistema..." << endl;
                break;

            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                system("pause");
                break;
        }

    } while (opcion != 4);

    return 0;
}
