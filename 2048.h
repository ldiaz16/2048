#ifndef GAME2048_H
#define GAME2048_H

#include <wx/wx.h>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

class Game2048Frame : public wxFrame {
public:
    Game2048Frame();
private:
    static const int GRID_SIZE = 4;
    vector<vector<int>> board;
    int score;
    int highScore;
    wxPanel* gamePanel;

    void InitializeBoard();
    void ResetBoard();

    void SaveHighScore();
    int LoadHighScore();

    void AddRandomTile();
    void CheckAllMoves();

    void DisplayGameOverPanel(wxDC& dc);
    void OnPaintGameOver(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnPaint(wxPaintEvent& event);
    wxColour GetColorForValue(int value);

    vector<int> columnify(int colKey, const vector<vector<int>>& board);
    vector<int> rowify(int rowKey, const vector<vector<int>>& board);
    
    void shiftLeft(int idx, vector<int>& columnValues);
    void shiftRight(int idx, vector<int>& columnValues);

    bool MoveUp();
    bool MoveDown();
    bool MoveLeft();
    bool MoveRight();
};

class Game2048App : public wxApp {
public:
    virtual bool OnInit();
};

#endif // GAME2048_H
