#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int contador = 0;

void timer_handler(int sig) {
    if (sig == SIGALRM) {
        contador++;

        time_t ahora = time(NULL);
        char *timestamp = ctime(&ahora);

        printf("🕐 [%s] Timer disparado! Segundo #%d\n", timestamp, contador);

        signal(SIGALRM, timer_handler);
    }
}

int main() {
    printf("🎯 TEST DEL TIMER - DCControl\n");
    printf("============================\n\n");
    
    printf("�� Configurando timer...\n");
    
    // Configurar el manejador de señales
    signal(SIGALRM, timer_handler);
    printf("   ✅ Manejador de señales configurado\n");

    struct itimerval timer;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec =0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    printf("   ⏱️  Timer configurado para disparar cada 1 segundo\n");
    printf("   �� Primera señal en 1 segundo\n");

    if (setitimer(ITIMER_REAL, &timer, NULL) == 0) {
        printf("   ✅ Timer configurado correctamente\n");
    } else {
        printf("   ❌ Error al configurar el timer\n");
        perror("   setitimer");
        return 1;

    }

    printf("\n📊 INFORMACIÓN DEL TIMER:\n");
    printf("   🎯 Timer se dispara cada: 1 segundo\n");
    printf("   🔄 El programa continuará hasta que presiones Ctrl+C\n");
    printf("   ⏰ Timer funcionando en segundo plano...\n\n");
    
    printf("⏳ Esperando que el timer se dispare...\n");
    printf("   Presiona Ctrl+C para terminar\n\n");

    while (1) {
        // Simular que el programa está procesando comandos
        sleep(5);  // Dormir 5 segundos para no saturar la salida

        printf("�� Programa principal: procesando comandos...\n");
        printf("   (Timer sigue funcionando en segundo plano)\n\n");
    }

    return 0;
}