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

## 🛠 Tech Stack

**Languages:** Java, Python, C, C#, SQL  
**Frontend:** HTML, CSS, JavaScript  
**Tools:** Git, GitLab CI/CD, MySQL, XAMPP

---

## 📌 Projects

| Project | Description | Tech |
|---|---|---|
| [Transport Empire](https://github.com/rustamkazimov6-crypto/transport-empire) | Turn-based transport strategy game with A* pathfinding and economy engine | Java, JavaFX |
| [Maze Highscore Game](https://github.com/rustamkazimov6-crypto/maze-highscore-game) | Procedurally generated maze game with SQL leaderboard | Java, Swing, SQL |
| [TCP Guessing Game](https://github.com/rustamkazimov6-crypto/tcp-guessing-game) | Client-server guessing game using binary search over TCP sockets | Python |
| [Network File Transfer](https://github.com/rustamkazimov6-crypto/netcopy-file-transfer) | 3-component file transfer system with MD5 checksum verification | Python |
| [Rubik Sliding Board](https://github.com/rustamkazimov6-crypto/rubik-board-game) | N x N sliding puzzle with circular row/column shifts | Java, Swing |
| [Easter Bunny Competition](https://github.com/rustamkazimov6-crypto/easter-bunny-competition) | Unix process competition simulator using fork, pipes, and signals | C |

---

## About Me

- Studying CS at ELTE Budapest since 2023
- Candidate Master of Sport in Water Polo (KMS) - Azerbaijan national level
- Languages: Russian (C2), Azerbaijani (C2), English (B2), Turkish (B1)
- Reach me: rustamkazimov6@gmail.com

---

*Always open to internship opportunities and interesting projects.*
