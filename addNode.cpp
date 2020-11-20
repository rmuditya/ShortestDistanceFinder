#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State
{
    kEmpty,
    kObstacle,
    kClosed,
};

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

//? 1st step search function now it is just printing not found!
vector<vector<State>> search(const vector<vector<State>> board, int start[2], int goal[2])
{
    cout << "not found!!!"
         << "\n";
    vector<vector<State>> v = {{}};
    return v;
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

int main()
{
    vector<vector<State>> board = ReadBoardFile("1.board");
    // PrintBoard(board);
    //? define the start and goal
    int start[2] = {0, 0};
    int goal[2] = {4, 5};
    //TODO: calling a Search function and sending heuristic map, start and goal
    vector<vector<State>> solution = search(board, start, goal);
    // PrintBoard(solution);
}