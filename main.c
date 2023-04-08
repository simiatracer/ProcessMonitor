#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void monitor_processes() {
    DIR* proc = opendir("/proc"); //DIR è una struttura definita in dirent.h fatta apposta per facilitare il browsing delle cartelle
    struct dirent* entry;
    
    while ((entry = readdir(proc)) != NULL) { // readdir lo possiamo paragonare a readline (giusto per fare un analogia sul funzionamento)
        int pid = atoi(entry->d_name); // atoi pulisce la stringa del pid del processo
        if (pid == 0) { // salto la certella 0 dato che ha info che non mi interessano
            continue;
        }

        // Apro il file di stato con le info del processo
        char status_file[256];
        sprintf(status_file, "/proc/%d/status", pid); // creo una stringa con il nome del file da aprire
        FILE* fp = fopen(status_file, "r");
        if (fp == NULL) {
            continue;
        }

        // estraggo tutte le info dal file
        char line[256];
        char* name = NULL;
        char* state = NULL;
        char* cpu_time = NULL;
        char* memory = NULL;
        while (fgets(line, 256, fp) != NULL) {
            if (strncmp(line, "Name:", 5) == 0) {
                name = strdup(line + 6);
                name[strlen(name) - 1] = '\0';  // rimuovo fine riga
            } else if (strncmp(line, "State:", 6) == 0) {
                state = strdup(line + 7);
                state[strlen(state) - 1] = '\0';  // rimuovo fine riga
            } else if (strncmp(line, "Cpu(s):", 7) == 0) {
                cpu_time = strdup(line + 8);
                cpu_time[strlen(cpu_time) - 1] = '\0';  // rimuovo fine riga
            } else if (strncmp(line, "VmRSS:", 6) == 0) {
                memory = strdup(line + 7);
                memory[strlen(memory) - 1] = '\0';  //rimuovo fine riga
            }
        }
        fclose(fp);

        // stampo a video le info ricavate
        if (name != NULL) {
            printf("PID: %d, Name: %s, State: %s, CPU time: %s, Memory: %s\n", pid, name, state, cpu_time, memory);
            free(name);
            free(state);
            free(cpu_time);
            free(memory);
        }
    }
    closedir(proc);
}

int main() {
    monitor_processes();

    return 0;
}
