#include <iostream>
#include <cstdlib>
#include <ctime>

#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

// Colores ANSI
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

// Prototipos de funciones
void generateMaze(char **maze, int width, int height);
void printMaze(char **maze, int width, int height);
bool solveMaze(char **maze, int width, int height, int x, int y);
void carveMaze(char **maze, int width, int height, int x, int y);
void printMazeWithColors(char **maze, int width, int height);

int main(int argc, char *argv[]) {
    srand(time(NULL)); // Inicializa la semilla para la generación aleatoria

    // Verifica los argumentos de línea de comandos para el tamaño del laberinto
    int width = 10, height = 10;
    if (argc == 3) {
        width = std::atoi(argv[1]);
        height = std::atoi(argv[2]);
    }

    // Reserva de memoria para el laberinto
    char **maze = new char*[height];
    for (int i = 0; i < height; i++) {
        maze[i] = new char[width];
    }

    // Generar laberinto
    generateMaze(maze, width, height);

    // Imprimir laberinto
    printMazeWithColors(maze, width, height);

    // Resolver laberinto
    if (solveMaze(maze, width, height, 0, 0)) {
        std::cout << "Maze Solved:" << std::endl;
        printMazeWithColors(maze, width, height);
    } else {
        std::cout << "No solution found." << std::endl;
    }

    // Liberar memoria
    for (int i = 0; i < height; i++) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}

void generateMaze(char **maze, int width, int height) {
    // Inicializa el laberinto con paredes
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze[i][j] = WALL;
        }
    }

    // Comenzar desde la esquina superior izquierda
    maze[0][0] = START;
    maze[1][0] = PATH;
    maze[1][1] = PATH;
    carveMaze(maze, width, height, 1, 1);
    maze[height - 1][width - 1] = END;
}

void carveMaze(char **maze, int width, int height, int x, int y) {
    // Direcciones en las que podemos movernos (Derecha, Izquierda, Abajo, Arriba)
    int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    // Mezclar direcciones para mayor aleatoriedad
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp[2] = { directions[i][0], directions[i][1] };
        directions[i][0] = directions[r][0];
        directions[i][1] = directions[r][1];
        directions[r][0] = temp[0];
        directions[r][1] = temp[1];
    }

    // Intentar moverse en cada dirección
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0] * 2;
        int ny = y + directions[i][1] * 2;

        if (nx >= 0 && nx < width && ny >= 0 && ny < height && maze[ny][nx] == WALL) {
            maze[ny - directions[i][1]][nx - directions[i][0]] = PATH;
            maze[ny][nx] = PATH;
            carveMaze(maze, width, height, nx, ny);
        }
    }
}

void printMaze(char **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << maze[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

void printMazeWithColors(char **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == WALL) {
                std::cout << BLUE << maze[i][j] << RESET;
            } else if (maze[i][j] == START) {
                std::cout << GREEN << maze[i][j] << RESET;
            } else if (maze[i][j] == END) {
                std::cout << RED << maze[i][j] << RESET;
            } else if (maze[i][j] == '.') {
                std::cout << CYAN << maze[i][j] << RESET;
            } else {
                std::cout << maze[i][j];
            }
        }
        std::cout << std::endl;
    }
}

bool solveMaze(char **maze, int width, int height, int x, int y) {
    // Caso base: hemos llegado a la salida
    if (x == width - 1 && y == height - 1) {
        maze[y][x] = '.';
        return true;
    }

    // Verificar si la posición es válida
    if (x >= 0 && x < width && y >= 0 && y < height && (maze[y][x] == PATH || maze[y][x] == START)) {
        // Marcar la ruta actual
        char temp = maze[y][x];
        maze[y][x] = '.';

        // Moverse en las 4 direcciones
        if (solveMaze(maze, width, height, x + 1, y) || // Derecha
            solveMaze(maze, width, height, x - 1, y) || // Izquierda
            solveMaze(maze, width, height, x, y + 1) || // Abajo
            solveMaze(maze, width, height, x, y - 1)) { // Arriba
            return true;
        }

        // Desmarcar la ruta actual (backtrack)
        maze[y][x] = temp;
    }

    return false;
}
