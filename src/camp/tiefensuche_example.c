#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define WIDTH 5
#define HEIGHT 5

typedef struct {
  int x, y;
  bool is_walkable;
  bool visited;
  int id;
} Tile;

Tile grid[HEIGHT][WIDTH];

// Berechnet die Manhattan-Distanz zwischen zwei Tiles
int heuristic(Tile* a, Tile* b) {
  return abs(a->x - b->x) + abs(a->y - b->y);
}

// Tiefensuche mit heuristischer Sortierung und Backtracking
bool dfs(Tile* current, Tile* goal, int* path, int* path_index) {
  // Prüft, ob das aktuelle Tile das Ziel ist
  if (current->x == goal->x && current->y == goal->y) {
    path[(*path_index)++] = current->id; // Füge das Ziel zum Pfad hinzu
    return true; // Ziel erreicht
  }

  current->visited = true; // Markiere das aktuelle Tile als besucht
  path[(*path_index)++] = current->id; // Füge das aktuelle Tile zum Pfad hinzu

  // Bewegungsrichtungen: oben, unten, links, rechts
  int directions[4][2] = {
    { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }
  };

  Tile* neighbors[4]; // Array für die Nachbarn des aktuellen Tiles
  int neighbor_count = 0; // Zähler für die Anzahl der Nachbarn

  // Finde alle gültigen Nachbarn
  for (int i = 0; i < 4; i++) {
    int new_x = current->x + directions[i][0];
    int new_y = current->y + directions[i][1];

    // Überprüft, ob die neuen Koordinaten innerhalb des Spielfeldes liegen
    if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT) {
      Tile* neighbor = &grid[new_y][new_x];
      // Überprüft, ob der Nachbar begehbar und unbesucht ist
      if (neighbor->is_walkable && !neighbor->visited) {
        neighbors[neighbor_count++] = neighbor; // Füge den Nachbarn zum Array hinzu
      }
    }
  }

  // Sortiere Nachbarn nach heuristischer Distanz zum Ziel
  for (int i = 0; i < neighbor_count - 1; i++) {
    for (int j = i + 1; j < neighbor_count; j++) {
      if (heuristic(neighbors[i], goal) > heuristic(neighbors[j], goal)) {
        Tile* temp = neighbors[i];
        neighbors[i] = neighbors[j];
        neighbors[j] = temp;
      }
    }
  }

  // Besuche Nachbarn in der sortierten Reihenfolge
  for (int i = 0; i < neighbor_count; i++) {
    if (dfs(neighbors[i], goal, path, path_index)) {
      return true; // Weg gefunden
    }
  }

  path[--(*path_index)] = -1; // Backtracking: Entferne das aktuelle Tile vom Pfad
  current->visited = false; // Markiere das aktuelle Tile als unbesucht
  return false; // Kein Weg gefunden
}

// Findet einen Pfad vom Starttile zum Zieltile
int* find_path(Tile* start, Tile* goal) {
  int* path = (int*)malloc(WIDTH * HEIGHT * sizeof(int)); // Allokiere Speicher für den Pfad
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    path[i] = -1; // Initialisiere Pfad mit -1
  }
  int path_index = 0; // Initialisiere den Pfadindex

  // Starte die Tiefensuche
  if (!dfs(start, goal, path, &path_index)) {
    free(path); // Wenn kein Weg gefunden wurde, gib den Speicher frei
    return NULL; // Rückgabe NULL, wenn kein Weg gefunden wurde
  }
  return path; // Rückgabe des gefundenen Pfades
}