# Hi, I'm Rustam

Computer Science student at **Eötvös Loránd University (ELTE)**, Budapest - 6th semester.

I enjoy building things from sc# Easter Bunny Competition

A C program that manages a bunny competition using Unix processes, pipes, and signals.

## Features

- Add, delete, and list bunnies with names and poems
- Run a competition where each bunny recites its poem and receives random eggs
- Competition runs in a **child process** (fork) and sends results back to the parent via a **pipe**
- Parent listens for **SIGUSR1 signals** from the child as each bunny arrives
- Winner (most eggs) is announced at the end
- All data persisted to `data.txt` between runs

## Tech Stack

C, Unix processes (fork/wait), pipes, signals (SIGUSR1, sigaction), file I/O

## How to Run

```
gcc -o bunny main.c
./bunny
```

## Author

Rustam Kazimov - [github.com/rustamkazimov6-crypto](https://github.com/rustamkazimov6-crypto)
ratch: games, tools, and anything that involves solving a real problem with code.

---
