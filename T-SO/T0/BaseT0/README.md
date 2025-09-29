# DCControl - Shell para Gestión de Procesos

## ¿Qué es?

DCControl es una shell en C que permite lanzar y gestionar procesos externos. Simula el control de misiones en una estación espacial donde necesitas manejar múltiples procesos en paralelo.

## ¿Qué implementé?

Implementé todos los comandos que pedía el enunciado:

- `launch` - Lanza procesos externos
- `status` - Muestra info de todos los procesos  
- `abort <tiempo>` - Termina procesos después de X segundos
- `shutdown` - Cierra DCControl de forma segura
- `emergency` - Termina todo inmediatamente

También implementé:
- Manejo de señales (SIGTERM, SIGKILL, SIGINT)
- Timer automático para timeouts
- Límite de 10 procesos simultáneos
- Gestión de memoria sin leaks
- Parámetro time_max para límite de tiempo por proceso

## Cómo usar

### Compilar
```bash
make
```

### Ejecutar
```bash
./DCControl <tiempo_maximo>
```

Ejemplo:
```bash
./DCControl 60
```

## Comandos

### `launch <ejecutable> [args]`
Lanza un proceso. Máximo 10 al mismo tiempo.

```bash
launch sleep 5
launch echo "hola"
launch ls -la
```

### `status`
Muestra tabla con todos los procesos:
- PID, Nombre, Tiempo, Exit Code, Signal

### `abort <tiempo>`
Termina procesos después de X segundos.

```bash
abort 5  # termina en 5 segundos
```

### `shutdown`
Cierra DCControl:
1. Envía SIGINT a procesos activos
2. Espera 10 segundos  
3. Envía SIGKILL a los que siguen vivos
4. Muestra estadísticas

### `emergency`
Termina todo inmediatamente con SIGKILL.

## Tests

### Básico
```bash
./DCControl 10
launch echo "hola"
status
shutdown
```

### Abort
```bash
./DCControl 10
launch sleep 60
abort 3
# esperar 3 segundos
```

### Emergency
```bash
./DCControl 10
launch sleep 60
emergency
```

### Timeout automático
```bash
./DCControl 3
launch sleep 10
# esperar 3 segundos (SIGTERM)
# esperar 5 segundos más (SIGKILL)
```

## Detalles técnicos

- Array dinámico para procesos (sin límite total, solo 10 activos)
- Sin memory leaks (verificado con AddressSanitizer)
- Manejo de señales: SIGCHLD, SIGALRM, SIGTERM, SIGKILL, SIGINT
- Timer automático cada segundo
- Estados: running, terminated, timeout_warning, force_terminated
- Historial completo de todos los procesos lanzados

## Lo que cumple

✅ Todos los comandos del enunciado  
✅ Gestión de señales  
✅ Sin procesos zombie  
✅ Estadísticas finales  
✅ Límite de 10 procesos simultáneos  
✅ Timer automático  
✅ Sin memory leaks  
✅ Ejecución en paralelo  
✅ Parámetro time_max  
✅ Manejo de errores


