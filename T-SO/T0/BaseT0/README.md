# DCControl - Control de Misiones Espaciales

## 📋 Descripción del Proyecto

DCControl es una shell que simula el control de misiones en una estación espacial. Permite gestionar procesos externos en paralelo, administrar su ciclo de vida y enviar señales para controlarlos. El proyecto está desarrollado en C puro utilizando syscalls del sistema operativo.

## 🎯 Objetivos

- Crear procesos externos en paralelo
- Administrar el ciclo de vida de los procesos (ejecutar, monitorear, terminar)
- Enviar señales (SIGTERM, SIGKILL, SIGINT)
- Evitar procesos zombie u huérfanos
- Mostrar estadísticas de los procesos

## 🚀 Estado Actual del Código

### ✅ Implementado
- **Estructura básica del proyecto** con Makefile funcional
- **Sistema de input** que lee comandos del usuario
- **Comando `launch`** - Crea nuevos procesos usando fork/exec
- **Comando `status`** - Muestra información de todos los procesos activos
- **Manejo básico de procesos** con límite máximo de 10 simultáneos
- **Estructura de datos** para almacenar información de procesos
- **Comando `exit`** - Sale de DCControl

### 🔧 Pendiente por Implementar
- Comando `abort` - Terminar procesos después de un tiempo
- Comando `shutdown` - Cerrar DCControl de forma segura
- Comando `emergency` - Terminación inmediata de todos los procesos
- Manejo de señales para detectar cuando terminan los procesos
- Validación del parámetro `time_max` del ejecutable

## 🛠️ Cómo Compilar y Ejecutar

### 1. Compilar el proyecto
```bash
make
```

### 2. Ejecutar DCControl
```bash
./DCControl <tiempo_maximo>
```

**Ejemplo:**
```bash
./DCControl 60
```

## 📚 Comandos Disponibles

### `launch <ejecutable> [argumentos]`
Lanza un nuevo proceso en segundo plano.

**Ejemplos:**
```bash
launch sleep 5
launch echo "hola mundo"
launch ls -la
launch ping -c 3 google.com
```

### `status`
Muestra una tabla con información de todos los procesos:
- **PID**: Identificador del proceso
- **Nombre**: Nombre del ejecutable
- **Tiempo**: Tiempo transcurrido desde su lanzamiento
- **Estado**: Estado actual del proceso
- **Exit Code**: Código de salida del proceso
- **Signal**: Señal recibida por el proceso

### `exit`
Sale de DCControl.

## 🧪 Tests para Probar la Funcionalidad

### Test 1: Proceso Simple
```bash
launch sleep 3
status
# Esperar 4 segundos
status
```
**Resultado esperado:** El proceso debe aparecer como RUNNING y luego como FINISHED.

### Test 2: Múltiples Procesos
```bash
launch sleep 5
launch sleep 3
launch sleep 1
status
```
**Resultado esperado:** Deben aparecer 3 procesos simultáneos.

### Test 3: Proceso que Termina Rápido
```bash
launch echo "hola"
status
```
**Resultado esperado:** El proceso debe terminar inmediatamente.

### Test 4: Listar Archivos
```bash
launch ls
status
```
**Resultado esperado:** Debe mostrar el resultado de `ls` y el proceso aparecer como terminado.

### Test 5: Límite de Procesos
```bash
# Lanzar 11 procesos para probar el límite
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10
launch sleep 10  # Este debe dar error
```
**Resultado esperado:** El último proceso debe mostrar "ERROR: Máximo de procesos alcanzado".

### Test 6: Error en Comando
```bash
launch
launch comando_inexistente
```
**Resultado esperado:** Debe mostrar mensajes de error apropiados.

## 🏗️ Estructura del Código

### Archivos Principales

#### `src/DCControl/main.c`
- **Función `main()`**: Loop principal que lee comandos del usuario
- **Función `launch_process()`**: Crea nuevos procesos usando fork/exec
- **Función `show_status()`**: Muestra información de todos los procesos
- **Variable global `processes[]`**: Array que almacena información de todos los procesos
- **Variable global `process_count`**: Contador de procesos activos

#### `src/DCControl/main.h`
- **Struct `ProcessInfo`**: Define la estructura de datos para cada proceso
- **Constantes**: `MAX_PROCESSES = 10`
- **Declaraciones de funciones**

#### `src/input_manager/manager.c`
- **Función `read_user_input()`**: Lee y parsea comandos del usuario
- **Función `set_buffer()`**: Configura el buffer de entrada
- **Función `free_user_input()`**: Libera memoria del input

### Flujo de Ejecución

1. **Inicialización**: Se configura el buffer y se inicia el loop principal
2. **Lectura de comando**: Se lee la entrada del usuario y se divide en tokens
3. **Procesamiento**: Se identifica el comando y se ejecuta la función correspondiente
4. **Gestión de procesos**: 
   - `launch`: Fork + exec para crear nuevo proceso
   - `status`: Itera sobre el array de procesos y muestra información


