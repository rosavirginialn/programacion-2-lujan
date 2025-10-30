# Sistema de Gestión Hospitalaria

## Descripcion del proyecto

### Objetivo General
Este proyecto académico tiene como objetivo desarrollar un sistema completo de gestión hospitalaria utilizando C++. El sistema permite administrar pacientes, doctores, citas médicas e historiales, haciendo uso de punteros, estructuras y memoria dinámica. 

### Funciones Principales
- Registro, búsqueda y actualizacion de datos hospitalarios.
- Organizacion y atencion de citas médicas.
- Gestión de historiales médicos por paciente.
- Validaciones de los distintos formatos.
- Interfaz de usuario con menús.

## Activar codificacion UTF-8
  ```bash
   chcp 65001
   ```

## Instrucciones de compilacion
   ```bash
   g++ -std=c++11 hospital.cpp -o hospital
   ```

## Instrucciones de ejecucion
  ```bash
   ./hospital
   ```

## Estructura de datos

### Structs
- struct Hospital
Todos los arreglos dinámicos del sistema (pacientes, doctores, citas, historiales). 
También incluye contadores únicos.
- struct Paciente
Almacena datos personales del paciente, su historial médico y las citas agendadas. 
- struct Doctor
Almacena información profesional del doctor, sus pacientes asignados y citas agendadas.
- struct Cita
Representa una cita médica entre un paciente y un doctor, estas incluyen fecha, hora, estado y motivo.
- struct HistorialMedico
Registra una consulta médica atendida, incluyendo diagnóstico, tratamiento, medicamentos, fecha, hora, doctor y costo.

### Relaciones entre estructuras
- El hospital es el centro de todos los datos y gestiona las relaciones entre estructuras.
- Cada cita se encuentra vinculada con un paciente y un doctor.
- Cada historial medico pertenece a un paciente y referencia un doctor.

## Funciones principales

### Listado de funciones por modulo con descripcion
- Módulo de Pacientes
crearPaciente: Registra un nuevo paciente.
buscarPacientePorCedula: Busca por cedula.
buscarPacientesPorNombre: Busca por el nombre ingresado.
actualizarDatosPaciente: Modifica datos existentes.
eliminarPaciente: Elimina paciente y sus citas.
listarPacientes: Muestra todos los pacientes.

- Módulo de Doctores
crearDoctor: Registra un nuevo doctor.
buscarDoctorPorId: Busca por ID.
buscarDoctoresPorEspecialidad: Filtra por especialidad.
asignarPacienteADoctor: Asigna paciente a su doctor.
listarPacientesDeDoctor: Muestra pacientes asignados.
eliminarDoctor: Elimina doctor y sus citas.
listarDoctores: Muestra todos los doctores.

- Módulo de Citas
agendarCita: Crea una nueva cita.
cancelarCita: Cambia estado a cancelada.
atenderCita: Registra consulta medica.
obtenerCitasDePaciente: Lista citas por paciente.
obtenerCitasDeDoctor: Lista citas por doctor.
obtenerCitasPorFecha: Filtra por fecha.
listarCitasPendientes: Muestra citas agendadas.

- Módulo de Utilidades y Helpers
inicializarHospital: Marca el inicio a la gestion hospitalaria.
destruirHospitol: Libera todo lo almacenado.
redimensionarArrays: Duplica capacidad de arrays dinamicos.

## Decisiones de diseno
- Uso de arrays dinámicos
- Manejo y consideracion de memoria
- Estrategia de redimensionamiento

## Tecnologías Utilizadas
- Lenguaje: C++ 
- Compilador: GCC 15.2.0 (MSYS2)
- Bibliotecas estándar: iostream, cstring, ctime, iomanip

## Control de Versiones
1. Inicializar repositorio:
git init
2. Añadir archivos:
git add
3. Commit de cambios:
git commit -m  "mensaje contextual"
4. Subir a repositorio remoto:
git push -u origin main
