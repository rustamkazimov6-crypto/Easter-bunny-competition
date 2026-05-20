#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define FILENAME    "data.txt"
#define TEMPFILE    "temp.txt"
#define MAX_LINE    400
#define MAX_BUNNIES 300

typedef struct {
    char name[51];
    char poem[256];
    int  eggs;
} Bunny;

static void sigusr1_handler(int sig, siginfo_t *si, void *ctx) {
    (void)sig;
    (void)ctx;
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "Child %d arrived to water\n", si->si_pid);
    write(STDOUT_FILENO, buf, len);
}

static void setup_sighandler(void) {
    struct sigaction sa = {0};
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void add_bunny() {
    FILE *f = fopen(FILENAME, "a");
    if (!f) { perror("cant open file"); return; }
    char name[51], poem[256];
    printf("Enter bunny name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Enter bunny poem: ");
    fgets(poem, sizeof(poem), stdin);
    poem[strcspn(poem, "\n")] = 0;
    fprintf(f, "%s|%s|0\n", name, poem);
    fclose(f);
    printf("Bunny added!\n");
}

void list_bunnies() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) { perror("cant open file"); return; }
    char line[MAX_LINE];
    printf("\nRegistered bunnies:\n");
    while (fgets(line, sizeof(line), f)) {
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "\n");
        if (name && poem && eggs) {
            printf("Name: %s\nPoem: %s\nEggs: %s\n--\n", name, poem, eggs);
        }
    }
    fclose(f);
}

void change_poem() {
    FILE *f = fopen(FILENAME, "r");
    FILE *tmp = fopen(TEMPFILE, "w");
    if (!f || !tmp) { perror("cant open files"); return; }

    char target[51], newpoem[256], line[MAX_LINE];
    printf("Enter bunny name: ");
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0;
    printf("Enter new poem: ");
    fgets(newpoem, sizeof(newpoem), stdin);
    newpoem[strcspn(newpoem, "\n")] = 0;

    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *n = strtok(line, "|");
        char *p = strtok(NULL, "|");
        char *e = strtok(NULL, "\n");
        if (!n || !p || !e) continue;
        if (strcmp(n, target) == 0) {
            fprintf(tmp, "%s|%s|%s\n", n, newpoem, e);
            found = 1;
        } else {
            fprintf(tmp, "%s|%s|%s\n", n, p, e);
        }
    }
    fclose(f);
    fclose(tmp);
    remove(FILENAME);
    rename(TEMPFILE, FILENAME);
    printf(found ? "Poem updated.\n" : "Bunny not found.\n");
}

void delete_bunny() {
    FILE *f = fopen(FILENAME, "r");
    FILE *tmp = fopen(TEMPFILE, "w");
    if (!f || !tmp) { perror("cant open files"); return; }

    char target[51], line[MAX_LINE];
    printf("Enter bunny name to delete: ");
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0;

    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *n = strtok(line, "|");
        char *p = strtok(NULL, "|");
        char *e = strtok(NULL, "\n");
        if (!n || !p || !e) continue;
        if (strcmp(n, target) != 0) {
            fprintf(tmp, "%s|%s|%s\n", n, p, e);
        } else {
            found = 1;
        }
    }
    fclose(f);
    fclose(tmp);
    remove(FILENAME);
    rename(TEMPFILE, FILENAME);
    printf(found ? "Bunny deleted.\n" : "Bunny not found.\n");
}

void add_eggs() {
    FILE *f = fopen(FILENAME, "r");
    FILE *tmp = fopen(TEMPFILE, "w");
    if (!f || !tmp) { perror("cant open files"); return; }

    char target[51], line[MAX_LINE];
    int extra;
    printf("Enter bunny name: ");
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = 0;
    printf("How many eggs to add? ");
    if (scanf("%d", &extra) != 1) extra = 0;
    getchar();

    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *n = strtok(line, "|");
        char *p = strtok(NULL, "|");
        char *e = strtok(NULL, "\n");
        if (!n || !p || !e) continue;
        int eggs = atoi(e);
        if (strcmp(n, target) == 0) {
            eggs += extra;
            found = 1;
        }
        fprintf(tmp, "%s|%s|%d\n", n, p, eggs);
    }
    fclose(f);
    fclose(tmp);
    remove(FILENAME);
    rename(TEMPFILE, FILENAME);
    printf(found ? "Eggs added.\n" : "Bunny not found.\n");
}

void competition() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) { perror("cant open file"); return; }

    Bunny bunnies[MAX_BUNNIES];
    int count = 0;
    char line[MAX_LINE];

    while (count < MAX_BUNNIES && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs_str = strtok(NULL, "|");
        if (name && poem && eggs_str) {
            strncpy(bunnies[count].name, name, sizeof(bunnies[count].name) - 1);
            bunnies[count].name[sizeof(bunnies[count].name) - 1] = '\0';
            strncpy(bunnies[count].poem, poem, sizeof(bunnies[count].poem) - 1);
            bunnies[count].poem[sizeof(bunnies[count].poem) - 1] = '\0';
            bunnies[count].eggs = atoi(eggs_str);
            count++;
        }
    }
    fclose(f);

    if (count == 0) {
        printf("No bunnies registered.\n");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        close(pipefd[0]);
        srand(time(NULL) ^ getpid());

        for (int i = 0; i < count; i++) {
            kill(getppid(), SIGUSR1);
            printf("Bunny %s recites: \"%s\"\n", bunnies[i].name, bunnies[i].poem);
            int got = (rand() % 20) + 1;
            printf("Bunny %s received %d eggs\n", bunnies[i].name, got);
            bunnies[i].eggs = got;
            sleep(1);
        }

        if (write(pipefd[1], bunnies, sizeof(Bunny) * count) != sizeof(Bunny) * count) {
            perror("write");
        }

        close(pipefd[1]);
        _exit(EXIT_SUCCESS);
    } else {
        setup_sighandler();

        close(pipefd[1]);

        ssize_t total_bytes = sizeof(Bunny) * count;
        ssize_t bytes_read = 0;
        char *buffer = (char *)bunnies;

        while (bytes_read < total_bytes) {
            ssize_t r = read(pipefd[0], buffer + bytes_read, total_bytes - bytes_read);
            if (r < 0) {
                if (errno == EINTR) continue;
                perror("read");
                break;
            } else if (r == 0) {
                break;
            }
            bytes_read += r;
        }

        close(pipefd[0]);
        waitpid(pid, NULL, 0);

        f = fopen(FILENAME, "w");
        if (!f) {
            perror("cant open file");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(f, "%s|%s|%d\n", bunnies[i].name, bunnies[i].poem, bunnies[i].eggs);
        }
        fclose(f);

        int best = -1, best_idx = -1;
        for (int i = 0; i < count; i++) {
            if (bunnies[i].eggs > best) {
                best = bunnies[i].eggs;
                best_idx = i;
            }
        }

        printf("\n=== Easter Bunny King ===\n");
        if (best_idx >= 0) {
            printf("%s with %d eggs!\n", bunnies[best_idx].name, best);
        } else {
            printf("No winner could be determined.\n");
        }
    }
}

int main() {
    int choice;
    do {
        printf("\nBunny Menu:\n");
        printf("1. Add bunny\n");
        printf("2. Change poem\n");
        printf("3. Delete bunny\n");
        printf("4. Show bunnies\n");
        printf("5. Add eggs manually\n");
        printf("6. Start competition\n");
        printf("0. Exit\n");
        printf("Your choice: ");
        if (scanf("%d", &choice) != 1) choice = -1;
        getchar();
        switch (choice) {
            case 1: add_bunny(); break;
            case 2: change_poem(); break;
            case 3: delete_bunny(); break;
            case 4: list_bunnies(); break;
            case 5: add_eggs(); break;
            case 6: competition(); break;
            case 0: printf("Bye!\n"); break;
            default: printf("Wrong option.\n");
        }
    } while (choice != 0);
    return 0;
}
