#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;

enum class State
{
    kEmpty,
    kObstacle,
    kClosed,
    kPath,
    kStart,
    kFinish,
};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line)
{
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',')
    {
        if (n == 0)
        {
            row.push_back(State::kEmpty);
        }
        else
        {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path)
{
    ifstream myfile(path);
    vector<vector<State>> board{};
    if (myfile)
    {
        string line;
        while (getline(myfile, line))
        {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

string CellString(State cell)
{
    switch (cell)
    {
    case State::kObstacle:
        return "⛰️   ";
    case State::kPath:
        return "🚗   ";
    case State::kStart:
        return "🚦   ";
    case State::kFinish:
        return "🏁 ";
    default:
        return "0   ";
    }
}

void PrintBoard(const vector<vector<State>> board)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

//? 2nd step, heuristic function
int heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1); //? abs = absolute postive, modulus function
}

//? 3rd step, add node
void addToNode(int x, int y, int g, int h, vector<vector<int>> &open, vector<vector<State>> &grid)
{
    vector<int> node = {x, y, g, h};
    open.push_back(node);
    grid[x][y] = State::kClosed;
}

//? 4th step, compare two node function
bool compare(vector<int> nodeOne, vector<int> nodeTwo)
{
    int f1 = nodeOne[2] + nodeOne[3];
    int f2 = nodeTwo[2] + nodeTwo[3];
    if (f1 > f2)
        return true;
    else
        return false;
}

//? 5th step, setup algo
void cellSort(vector<vector<int>> *v)
{
    sort(v->begin(), v->end(), compare);
}

// ? 6th step checkValidCell()
bool checkValidCell(int x, int y, vector<vector<State>> grid)
{
    int rowLen = grid[0].size();
    int colLen = grid.size();
    if (x < 0 || x > colLen - 1)
        return false;
    if (y < 0 || y > rowLen - 1)
        return false;
    if (grid[x][y] == State::kEmpty)
        return true;
    return false;
}
//? 7th step, ExpandNeighbour
void ExpandNeighbour(vector<int> current, int goal[2], vector<vector<int>> &open, vector<vector<State>> &grid)
{
    int x = current[0];
    int y = current[1];
    int g = current[2];
    int h = current[3];

    // TODO: Loop through current node's potential neighbors.
    for (auto neighbor : delta)
    {
        // TODO: Check that the potential neighbor's x2 and y2
        //       values are on the grid and not closed.
        int x2 = x + neighbor[0];
        int y2 = y + neighbor[1];

        if (checkValidCell(x2, y2, grid) == 1)
        {
            // TODO: Increment g value, compute h value,
            //       and add neighbor to open list.
            int g2 = g + 1;
            int h2 = heuristic(x2, y2, goal[0], goal[1]);
            addToNode(x2, y2, g2, h2, open, grid);
        }
    }
    // } TODO: End the function
}

//? 1st step search function now it is just printing not found!
vector<vector<State>> search(vector<vector<State>> grid, int start[2], int goal[2])
{
    vector<vector<int>> open{};
    int x = start[0];
    int y = start[1];
    int g = 0;
    int h = heuristic(start[0], start[1], goal[0], goal[1]);
    addToNode(x, y, g, h, open, grid);

    while (open.size() > 0)
    {
        cellSort(&open);
        vector<int> current = open[open.size() - 1];
        open.pop_back();

        int x = current[0];
        int y = current[1];
        grid[x][y] = State::kPath;

        if (x == goal[0] && y == goal[1])
        {
            grid[start[0]][start[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }

        ExpandNeighbour(current, goal, open, grid);
    }

    cout << "not found"
         << "\n";
    return std::vector<vector<State>>{};
}

int main()
{
    vector<vector<State>> board = ReadBoardFile("1.board");
    // PrintBoard(board);
    //? define the start and goal
    int start[2] = {0, 0};
    int goal[2] = {4, 5};
    //TODO: calling a Search function and sending heuristic map, start and goal
    vector<vector<State>> solution = search(board, start, goal);
    PrintBoard(solution);
}