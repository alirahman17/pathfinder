#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cjson/cJSON.h"

// JSON Descriptors for each tile (can allow for different formatting)
#define GROUND -1
#define ELEVATED 3
#define START 8.3f  // Change back to 8.3 for submission (examples used 8.1)
#define TARGET 0.5f

// Struct to define coordinates
typedef struct {
    int x, y;
} Point;

// struct for structuring a queue
typedef struct Node {
    Point point;
    struct Node* parent;  // need parent for printing complete path (vector in c++ wouldve been useful)
} Node;

int directions[4][2] = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    // Directions of available moves (+y, +x, -y, -x)
};

//checks if the board position is within the bounds set by the json
int in_bounds(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// algorithm of choice (breadth first search)
// i chose breadth first search since there was no weight assigned with a move and we can treat it as equal
void bfs(float* map, int width, int height, Point start, Point target) {
    int** visited = malloc(height * sizeof(int*));  // visited list of nodes allocation
    for (int i = 0; i < height; i++) {
        visited[i] = calloc(width, sizeof(int));
    }

    Node** queue = malloc(width * height * sizeof(Node*));  // queue for node selecting to maintain bfs algo

    // Algo Init
    int front = 0, rear = 0;
    Node* startNode = malloc(sizeof(Node));
    startNode->point = start;
    startNode->parent = NULL;
    queue[rear++] = startNode;
    visited[start.y][start.x] = 1; // (need to do y, x because of how we allocated)
    Node* targetNode = NULL;

    while (front < rear) {
        // Need this to maintain while loop to complete search
        Node* current = queue[front++];

        // exit condition if node matches target
        if (current->point.x == target.x && current->point.y == target.y) {
            targetNode = current;
            break;
        }

        for (int i = 0; i < 4; i++) { // Check each direction
            int newx = current->point.x + directions[i][0];
            int newy = current->point.y + directions[i][1];

            // Check if new location is within bounds
            if (!in_bounds(newx, newy, width, height)){
              continue;
            }
            // Check if we've visited this location already
            if (visited[newy][newx]){
              continue;
            }

            float tile = map[newy * width + newx];  //check tile type now that we know it exists
            if (tile == ELEVATED){
              continue;   // Can't traverse elevated terrain so we skip
            }

            // Create next element to add to queue
            Node* neighbor = malloc(sizeof(Node));
            neighbor->point.x = newx;
            neighbor->point.y = newy;
            neighbor->parent = current;
            queue[rear++] = neighbor;
            visited[newy][newx] = 1;  // mark the location we just checked as visited
        }
    }

    if (targetNode) {
        printf("Path found:\n");
        Node* path[width * height];
        int count = 0;

        for (Node* n = targetNode; n != NULL; n = n->parent){
          path[count++] = n;  // rebuild path but in reverse
        }

        for (int i = count - 1; i >= 0; i--) {
            printf("(%d, %d)\n", path[i]->point.x, path[i]->point.y); // print in reverse to get correct order
        }
    } else {
        printf("No path found.\n");
    }

    // Freeing memory to avoid memory leaks/crashes
    for (int i = 0; i < height; i++){
      free(visited[i]);
    }
    free(visited);
    for (int i = 0; i < rear; i++){
      free(queue[i]);
    }
    free(queue);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,"Usage: %s <map.json>\n", argv[0]);
        return -1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr,"Error opening file");
        return -2;
    }

    // Get length of file
    fseek(file, 0, SEEK_END);
    long len = ftell(file);

    // Set file back to begin and read into buffer
    rewind(file);
    char* jsonData = malloc(len + 1);
    fread(jsonData, 1, len, file);
    fclose(file);
    // Null terminator to avoid issues
    jsonData[len] = '\0';

    // Parse json using cJSON (refer to example: https://github.com/DaveGamble/cJSON?tab=readme-ov-file#parsing)
    cJSON* root = cJSON_Parse(jsonData);
    if (!root) {
        fprintf(stderr,"Error parsing JSON\n");
        free(jsonData);
        return -3;
    }

    // Structure
    // layers (not sure what anything aside from world layer is for)
    //    name
    //    tileset
    //    data        **** Important

    // tilesets (not sure what anything past tileset 1 is for)
    //    name
    //    image
    //    imagewidth
    //    imageheight
    //    tilewidth   **** Important
    //    tileheight  **** Important

    // canvas
    //    width       **** Important
    //    height      **** Important
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    cJSON* firstLayer = cJSON_GetArrayItem(layers, 0);
    cJSON* data = NULL;
    cJSON* item = NULL;
    cJSON_ArrayForEach(item, layers) {
        cJSON *name = cJSON_GetObjectItem(item, "name");
        // printf("%s\n", name->valuestring);
        // making an assumption that the name of the layer with the tileset is called world
        if (cJSON_IsString(name) && (!strcmp(name->valuestring,"world"))) {
            data = cJSON_GetObjectItem(item, "data");
        }
    }

    if(data == NULL){
      fprintf(stderr,"Unable to obtain data to path find\n");
      return -4;
    }
    // Width and height of map calculation using canvas / tileset
    cJSON* tilesets = cJSON_GetObjectItem(root, "tilesets");
    cJSON* tileset = cJSON_GetArrayItem(tilesets, 0);
    cJSON* tileset_width = cJSON_GetObjectItem(tileset, "tilewidth");
    cJSON* tileset_height = cJSON_GetObjectItem(tileset, "tileheight");

    cJSON* canvas = cJSON_GetObjectItem(root, "canvas");
    cJSON* canvas_width = cJSON_GetObjectItem(canvas, "width");
    cJSON* canvas_height = cJSON_GetObjectItem(canvas, "height");

    int width = canvas_width->valueint / tileset_width->valueint;
    int height = canvas_height->valueint / tileset_height->valueint;
    int totalTiles = width * height;

    // Search Init
    float* map = malloc(totalTiles * sizeof(float));
    Point start = {-1, -1};
    Point target = {-1, -1};

    // Get element number at each location and assign it to global map
    for (int i = 0; i < totalTiles; i++) {
        float val = (float) cJSON_GetArrayItem(data, i)->valuedouble;
        map[i] = val; // keep 1d to make it easy to compare against json format
        int x = i % width;  // get row
        int y = i / width;  // get column

        // needed to use float abs because double and float compare (could maybe change everything to double, but got seg fault)
        if (val == START) {
            start.x = x;
            start.y = y;
        } else if (val == TARGET) {
            target.x = x;
            target.y = y;
        }
    }

    if (start.x == -1 || target.x == -1) {
        fprintf(stderr,"Error: Could not find start or target position in map.\n");
        // Clean up data to avoid memory leak
        cJSON_Delete(root);
        free(map);
        free(jsonData);
        return -5;
    }

    printf("Map loaded with dimensions %d x %d\n", width, height);
    printf("Start Pos: (%d, %d), Target Pos: (%d, %d)\n", start.x, start.y, target.x, target.y);

    bfs(map, width, height, start, target);

    // Clean up data to avoid memory leak
    cJSON_Delete(root);
    free(map);
    free(jsonData);
    return 0;
}
