#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define MAX_OPEN_NODES 1000

typedef struct {
  int x, y;
} Point;

typedef struct {
  Point pos;
  int gCost;
  int hCost;
  int fCost;
  Point parent;
} Node;

int isValid(Point point, int map[MAP_WIDTH][MAP_HEIGHT]) {
  return point.x >= 0 && point.x < MAP_WIDTH && point.y >= 0 && point.y < MAP_HEIGHT && map[point.x][point.y] == 0;
}

int isEqual(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}

int calculateH(Point a, Point b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

Node *getLowestFCostNode(Node *nodes[], int count) {
  Node *lowest = nodes[0];
  for (int i = 1; i < count; i++) {
    if (nodes[i]->fCost < lowest->fCost) {
      lowest = nodes[i];
    }
  }
  return lowest;
}

void removeNode(Node *nodes[], int *count, Node *node) {
  int index = -1;
  for (int i = 0; i < *count; i++) {
    if (nodes[i] == node) {
      index = i;
      break;
    }
  }
  if (index != -1) {
    for (int i = index; i < *count - 1; i++) {
      nodes[i] = nodes[i + 1];
    }
    (*count)--;
  }
}

int isInList(Node *nodes[], int count, Point pos) {
  for (int i = 0; i < count; i++) {
    if (isEqual(nodes[i]->pos, pos)) {
      return 1;
    }
  }
  return 0;
}

void printPath(Node *nodes[], int count, Point start, Point end) {
  for (int i = 0; i < count; i++) {
    if (isEqual(nodes[i]->pos, end)) {
      Node *current = nodes[i];
      while (!isEqual(current->pos, start)) {
        printf("(%d, %d) <- ", current->pos.x, current->pos.y);
        for (int j = 0; j < count; j++) {
          if (isEqual(nodes[j]->pos, current->parent)) {
            current = nodes[j];
            break;
          }
        }
      }
      printf("(%d, %d)\n", start.x, start.y);
      return;
    }
  }
}

void aStar(int map[MAP_WIDTH][MAP_HEIGHT], Point start, Point end) {
  Node *openNodes[MAX_OPEN_NODES];
  int openCount = 0;
  Node *closedNodes[MAX_OPEN_NODES];
  int closedCount = 0;

  Node *startNode = (Node *)malloc(sizeof(Node));
  startNode->pos = start;
  startNode->gCost = 0;
  startNode->hCost = calculateH(start, end);
  startNode->fCost = startNode->gCost + startNode->hCost;
  startNode->parent = start;

  openNodes[openCount++] = startNode;

  Point directions[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  while (openCount > 0) {
    Node *currentNode = getLowestFCostNode(openNodes, openCount);
    if (isEqual(currentNode->pos, end)) {
      printPath(closedNodes, closedCount, start, end);
      return;
    }

    removeNode(openNodes, &openCount, currentNode);
    closedNodes[closedCount++] = currentNode;

    for (int i = 0; i < 4; i++) {
      Point neighborPos = {currentNode->pos.x + directions[i].x, currentNode->pos.y + directions[i].y};
      if (!isValid(neighborPos, map) || isInList(closedNodes, closedCount, neighborPos)) {
        continue;
      }

      int newGCost = currentNode->gCost + 1;
      int newHCost = calculateH(neighborPos, end);
      int newFCost = newGCost + newHCost;

      if (!isInList(openNodes, openCount, neighborPos)) {
        Node *neighborNode = (Node *)malloc(sizeof(Node));
        neighborNode->pos = neighborPos;
        neighborNode->gCost = newGCost;
        neighborNode->hCost = newHCost;
        neighborNode->fCost = newFCost;
        neighborNode->parent = currentNode->pos;
        openNodes[openCount++] = neighborNode;
      } else {
        for (int j = 0; j < openCount; j++) {
          if (isEqual(openNodes[j]->pos, neighborPos) && newGCost < openNodes[j]->gCost) {
            openNodes[j]->gCost = newGCost;
            openNodes[j]->fCost = newFCost;
            openNodes[j]->parent = currentNode->pos;
          }
        }
      }
    }
  }
  printf("No path found\n");
}

int main() {
  int map[MAP_WIDTH][MAP_HEIGHT] = {
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
  };

  Point start = {0, 0};
  Point end = {9, 9};

  aStar(map, start, end);

  return 0;
}