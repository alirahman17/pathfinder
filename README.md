# Battlefield Path Finder

## Implementation
- Written and tested on Ubuntu Server 24.04 and gcc 13.3.0 (gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0)
- JSON Parsing
- Breadth First Search (BFS) was used because all moves can be treated as equal and I was most familiar with it
- Algorithm returns the shortest path when one is available (ties will return the first path completed)
- Queue to check valid moves from starting position
- Error checking to find and avoid segmentation faults
- Used JSON files created on risky lab (https://riskylab.com/tilemap)
- Defined tile values to allow for different formatting (useful for the provided example as it differed from the given instructions)
- Created a struct to map x and y coordinates to array traversal
- Used a queue to collect elements to iterate through in order adhere to breadth first search
- Assumes the relevant data is on the world layer

## External Libraries
Used the cJSON library (https://github.com/DaveGamble/cJSON) \
This library was used to parse and manipulate the input JSON files to be easily read \
This library was chosen as it was the one I had the most familiarity with

## Compile Instructions
gcc pathfinder.c -o pathfinder -lcjson -lm

## How to Run
Install cJSON: `sudo apt-get install libcjson-dev` \
Compile program: `gcc pathfinder.c -o pathfinder -lcjson -lm ` \
Run Program: `./pathfinder <map.json>`

## Sample Runs and Outputs
take_home_project.json (provided but change the 8.1 to 8.3):
```
~$ ./pathfinder take_home_project.json
Map loaded with dimensions 32 x 32
Start Pos: (0, 25), Target Pos: (30, 8)
Path found:
(0, 25)
(0, 26)
(1, 26)
(2, 26)
(2, 27)
(2, 28)
(2, 29)
(3, 29)
(4, 29)
(5, 29)
(5, 28)
(6, 28)
(6, 27)
(6, 26)
(7, 26)
(8, 26)
(8, 27)
(8, 28)
(8, 29)
(8, 30)
(8, 31)
(9, 31)
(10, 31)
(11, 31)
(12, 31)
(13, 31)
(14, 31)
(15, 31)
(16, 31)
(17, 31)
(18, 31)
(19, 31)
(19, 30)
(19, 29)
(19, 28)
(19, 27)
(20, 27)
(21, 27)
(22, 27)
(23, 27)
(24, 27)
(24, 26)
(24, 25)
(24, 24)
(24, 23)
(24, 22)
(24, 21)
(24, 20)
(23, 20)
(23, 19)
(22, 19)
(21, 19)
(21, 20)
(20, 20)
(19, 20)
(18, 20)
(17, 20)
(16, 20)
(15, 20)
(14, 20)
(14, 19)
(14, 18)
(14, 17)
(15, 17)
(16, 17)
(16, 16)
(17, 16)
(18, 16)
(19, 16)
(20, 16)
(20, 15)
(20, 14)
(20, 13)
(20, 12)
(20, 11)
(20, 10)
(20, 9)
(20, 8)
(20, 7)
(21, 7)
(21, 6)
(22, 6)
(23, 6)
(23, 5)
(24, 5)
(24, 4)
(25, 4)
(26, 4)
(26, 5)
(27, 5)
(27, 6)
(28, 6)
(28, 7)
(28, 8)
(29, 8)
(30, 8)
```
provided map_5_7.json:
```
~$ ./pathfinder map_5_7.json
Map loaded with dimensions 5 x 7
Start Pos: (2, 5), Target Pos: (0, 0)
Path found:
(2, 5)
(2, 4)
(2, 3)
(2, 2)
(2, 1)
(2, 0)
(1, 0)
(0, 0)
```

provided map_5_7_obstacles.json:
```
~$ ./pathfinder map_5_7_obstacles.json
Map loaded with dimensions 5 x 7
Start Pos: (2, 5), Target Pos: (0, 0)
Path found:
(2, 5)
(2, 6)
(3, 6)
(4, 6)
(4, 5)
(4, 4)
(4, 3)
(4, 2)
(4, 1)
(4, 0)
(3, 0)
(2, 0)
(1, 0)
(0, 0)
```

provided map_6_8_complete.json:
```
~$ ./pathfinder map_6_8_complete.json
Map loaded with dimensions 8 x 6
Start Pos: (6, 4), Target Pos: (0, 0)
Path found:
(6, 4)
(6, 5)
(5, 5)
(4, 5)
(3, 5)
(2, 5)
(1, 5)
(0, 5)
(0, 4)
(0, 3)
(0, 2)
(0, 1)
(0, 0)
```

provided map_6_8_none.json:
```
~$ ./pathfinder map_6_8_none.json
Map loaded with dimensions 8 x 6
Start Pos: (6, 4), Target Pos: (0, 0)
No path found.
```
