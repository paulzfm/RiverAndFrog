# River and Frog

This is a simple river and frog arcade game for course _Parallel Programming_.

### Installing

This project is created with `Qt 5`. You may first install Qt library and then use `qmake` (or `Qt Creator` for convenient) to build and run this project.

We also use `pthread` in the project so that you may build this project ONLY on unix-like systems.

### Usage

Please press `UP` and `DOWN` keys to move the frog from one wood to another.

### Multi-threads

We use the following threads in this project:

1. For each row, we use a thread to compute the locations of the woods and the frog (if he is standing on this row).
2. We use another thread to repaint the screen every 20 ms.
3. We also use a thread as a timer, to update both the seconds elapsed and the game speed.
