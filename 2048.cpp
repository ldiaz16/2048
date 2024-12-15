#include "2048.h"

Game2048Frame::Game2048Frame() : wxFrame(nullptr, wxID_ANY, "2048", wxDefaultPosition, wxSize(400, 600)) {
    gamePanel = new wxPanel(this);
    gamePanel->Bind(wxEVT_PAINT, &Game2048Frame::OnPaint, this);
    gamePanel->Bind(wxEVT_KEY_DOWN, &Game2048Frame::OnKeyDown, this);
    gamePanel->SetFocus();
    InitializeBoard();
}

void Game2048Frame::InitializeBoard() {
    board = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    score = 0;
    highScore = LoadHighScore();
    AddRandomTile();
    AddRandomTile();
}

void Game2048Frame::ResetBoard() {
    if (score > highScore) {
        highScore = score;
    }
    InitializeBoard();
}

void Game2048Frame::SaveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << max(score, LoadHighScore());
        file.close();
    }
}

int Game2048Frame::LoadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void Game2048Frame::AddRandomTile() {
    random_device rd;
    mt19937 gen(rd());
    vector<pair<int, int>> emptyCells;
    int emptyCellCount = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if(board[i][j] == 0) {
                emptyCells.push_back({i, j});
                emptyCellCount++;
            }
        }
    }
    if(!emptyCells.empty()) {
        uniform_int_distribution<> distr(0, emptyCells.size() - 1);
        auto [row, col] = emptyCells[distr(gen)];
        board[row][col] = ((distr(gen) % 2 + 1) * 2) + 1;
        emptyCellCount--;
    }
    if (emptyCellCount == 0) {
        CheckAllMoves();
    }
}

void Game2048Frame::CheckAllMoves() {
    vector<vector<int>> originalBoard = board;
    if (!(MoveUp() || MoveDown() || MoveLeft() || MoveRight())) {
        gamePanel->Refresh();
        gamePanel->Bind(wxEVT_PAINT, &Game2048Frame::OnPaintGameOver, this);
        gamePanel->Refresh();
        SaveHighScore();
    } else {
        board = originalBoard;
    }
}

void Game2048Frame::OnKeyDown(wxKeyEvent& event) {
    if (event.GetKeyCode() == 'R') {
        ResetBoard();
        gamePanel->Unbind(wxEVT_PAINT, &Game2048Frame::OnPaintGameOver, this);
        Refresh();
        return;
    } else if (event.GetKeyCode() == 'Q') {
        Close(true);
        return;
    }
    bool moved = false;
    wxLogDebug("Key Code: %d", event.GetKeyCode()); // Logging key codes for debugging
    switch(event.GetKeyCode()) {
        case WXK_UP:
            moved = MoveUp();
            break;
        case WXK_DOWN:
            moved = MoveDown();
            break;
        case WXK_LEFT:
            moved = MoveLeft();
            break;
        case WXK_RIGHT:
            moved = MoveRight();
            break;
    }
    if(moved) {
        AddRandomTile();
        Refresh();
    }
}

void Game2048Frame::DisplayGameOverPanel(wxDC& dc) {
    wxSize size = GetClientSize();
    wxRect gameOverPanel(50, size.GetHeight() / 2 - 100, size.GetWidth() - 100, 200);

    dc.SetBrush(wxBrush(wxColour(0, 0, 0, 128)));
    dc.SetPen(*wxTRANSPARENT_PEN); // Semi-transparent black
    dc.DrawRectangle(gameOverPanel);

    dc.SetTextForeground(*wxWHITE);

    wxFont gameOverFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(gameOverFont);

    wxString gameOverText = "Game Over";
    wxSize gameOverTextSize = dc.GetTextExtent(gameOverText);
    int gameOverTextX = gameOverPanel.GetX() + (gameOverPanel.GetWidth() - gameOverTextSize.GetWidth()) / 2;
    int gameOverTextY = gameOverPanel.GetY() + 20;
    dc.DrawText(gameOverText, gameOverTextX, gameOverTextY);

    wxFont scoreFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(scoreFont);

    wxString finalScoreText = wxString::Format("Final Score: %d", score);
    wxSize finalScoreTextSize = dc.GetTextExtent(finalScoreText);
    int finalScoreTextX = gameOverPanel.GetX() + (gameOverPanel.GetWidth() - finalScoreTextSize.GetWidth()) / 2;
    int finalScoreTextY = gameOverTextY + gameOverTextSize.GetHeight() + 20;
    dc.DrawText(finalScoreText, finalScoreTextX, finalScoreTextY);

    wxString restartText = "Press R to Restart";
    wxSize restartTextSize = dc.GetTextExtent(restartText);
    int restartTextX = gameOverPanel.GetX() + (gameOverPanel.GetWidth() - restartTextSize.GetWidth()) / 2;
    int restartTextY = finalScoreTextY + finalScoreTextSize.GetHeight() + 20;
    dc.DrawText(restartText, restartTextX, restartTextY);
}

void Game2048Frame::OnPaintGameOver(wxPaintEvent& event) {
    wxPaintDC dc(gamePanel);
    DisplayGameOverPanel(dc);
}

void Game2048Frame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(gamePanel);
    wxSize size = GetClientSize();
    wxSize cellSize(size.GetWidth() / GRID_SIZE, (size.GetHeight() - 200) / GRID_SIZE);
    wxSize scoreSize(size.GetWidth() / 2, (size.GetHeight() - 400));

    wxFont font1(30, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont font2(15, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(font1);

    wxRect bckg(size);
    dc.SetBrush(GetColorForValue(1));
    dc.DrawRectangle(bckg);

    wxRect scoreSquare(10, 10, scoreSize.GetWidth() - 20, scoreSize.GetHeight());
    dc.SetBrush(GetColorForValue(128));
    dc.DrawRectangle(scoreSquare);
    wxString scoreText = wxString::Format("%d", score);
    wxString scoreLabel = wxString::Format("Total Score");
    wxSize scoreTextSize = dc.GetTextExtent(scoreText);
    wxSize scoreLabelSize = dc.GetTextExtent(scoreLabel);

    wxRect highScoreSquare(scoreSize.GetWidth() + 10, 10, scoreSize.GetWidth() - 20, scoreSize.GetHeight());
    dc.DrawRectangle(highScoreSquare);
    wxString highScoreText = wxString::Format("%d", highScore);
    wxString highScoreLabel = wxString::Format("High Score");
    wxSize highScoreTextSize = dc.GetTextExtent(highScoreText);
    wxSize highScoreLabelSize = dc.GetTextExtent(highScoreLabel);

    int scoreTextX = scoreSquare.GetX() + (scoreSquare.GetWidth() - scoreTextSize.GetWidth()) / 2;
    int scoreTextY = scoreSquare.GetY() + (scoreSquare.GetHeight() - scoreTextSize.GetHeight()) / 2;
    dc.DrawText(scoreText, scoreTextX, scoreTextY);

    int highScoreTextX = highScoreSquare.GetX() + (highScoreSquare.GetWidth() - highScoreTextSize.GetWidth()) / 2;
    int highScoreTextY = highScoreSquare.GetY() + (highScoreSquare.GetHeight() - highScoreTextSize.GetHeight()) / 2;
    dc.DrawText(highScoreText, highScoreTextX, highScoreTextY);

    int scoreLabelX = scoreSquare.GetX() + (scoreSquare.GetWidth() - scoreLabelSize.GetWidth()) / 2;
    int scoreLabelY = scoreSquare.GetY() + (scoreSquare.GetHeight() - scoreLabelSize.GetHeight()) / 2;
    dc.DrawText(scoreLabel, scoreLabelX, scoreLabelY - 60);

    int highScoreLabelX = highScoreSquare.GetX() + (highScoreSquare.GetWidth() - highScoreLabelSize.GetWidth()) / 2;
    int highScoreLabelY = highScoreSquare.GetY() + (highScoreSquare.GetHeight() - highScoreLabelSize.GetHeight()) / 2;
    dc.DrawText(highScoreLabel, highScoreLabelX, highScoreLabelY - 60);

    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            // Calculate the rectangle for each cell
            wxRect rect(j * cellSize.GetWidth() + 10, i * cellSize.GetHeight() + 210 , cellSize.GetWidth() - 20, cellSize.GetHeight() - 20);
            dc.SetBrush(GetColorForValue(board[i][j]));
            dc.DrawRectangle(rect);
            
            if(board[i][j] != 0) {
                dc.SetFont(font1);
                wxString text = wxString::Format("%d", board[i][j]);
                wxSize textSize = dc.GetTextExtent(text);
                int x = rect.GetX() + (rect.GetWidth() - textSize.GetWidth()) / 2;
                int y = rect.GetY() + (rect.GetHeight() - textSize.GetHeight()) / 2;

                if (board[i][j] % 2 == 0) {
                    dc.DrawText(text, x, y);
                } else {
                    board[i][j] -= 1;
                    text = wxString::Format("%d", board[i][j]);
                    textSize = dc.GetTextExtent(text);
                    x = rect.GetX() + (rect.GetWidth() - textSize.GetWidth()) / 2;
                    y = rect.GetY() + (rect.GetHeight() - textSize.GetHeight()) / 2;    
                    dc.DrawText(text, x, y);

                    text = wxString::Format("New");
                    dc.SetFont(font2);
                    wxSize newTextSize = dc.GetTextExtent(text);
                    int newX = rect.GetX() + (rect.GetWidth() - newTextSize.GetWidth()) / 2;
                    int newY = y + textSize.GetHeight() + 5; // Position "New" text below the number
                    dc.DrawText(text, newX, newY);
                }
            }
        }
    }
}


wxColour Game2048Frame::GetColorForValue(int value) {
    // Define colors for different tile values
    switch(value) {
        case 1: return wxColour(255,255,255);
        case 0: return wxColour(204, 192, 179);
        case 2: return wxColour(255, 229, 204);
        case 3: return wxColour(255, 229, 204);
        case 4: return wxColour(255, 204, 153);
        case 5: return wxColour(255, 204, 153);
        case 8: return wxColour(255, 178, 102);
        case 16: return wxColour(255, 153, 51);
        case 32: return wxColour(243, 124, 65);
        case 64: return wxColour(249, 48, 8);
        case 128: return wxColour(243, 213, 113);
        case 256: return wxColour(238, 194, 50);
        case 512: return wxColour(239, 188, 20);
        case 1024: return wxColour(216, 166, 0);
        case 2048: return wxColour(201, 154, 0);
        case 4096: return wxColour(129, 228, 0);
        case 8192: return wxColour(212, 0, 191);
        // Add more colors for higher values
        default: return wxColour(237, 194, 146);
    }
}

vector<int> Game2048Frame::columnify(int colKey, const vector<vector<int>>& board) {
    vector<int> colVals(4); // Ensure the vector is of size 4
    for (int i = 0; i < 4; i++) {
        colVals.at(i) = board[i][colKey];
    }
    return colVals;
}

vector<int> Game2048Frame::rowify(int rowKey, const vector<vector<int>>& board) {
    vector<int> rowVals(4); // Ensure the vector is of size 4
    for (int i = 0; i < 4; i++) {
        rowVals.at(i) = board[rowKey][i];
    }
    return rowVals;
}

void Game2048Frame::shiftLeft(int idx, vector<int>& columnValues) {
    try {
        for (int arrayIndex = idx; arrayIndex < 4; arrayIndex++) {
            if (arrayIndex == 3) {
                columnValues.at(arrayIndex) = 0;
            } else {
                columnValues.at(arrayIndex) = columnValues.at(arrayIndex + 1);
            }
            if (arrayIndex != 0) {
                if (columnValues.at(arrayIndex - 1) == 0 && columnValues.at(arrayIndex) != 0) {
                    columnValues.at(arrayIndex - 1) = columnValues.at(arrayIndex);
                    columnValues.at(arrayIndex) = 0;
                }
            }
        }
        if (columnValues.at(0) == 0) {
            columnValues.at(0) = columnValues.at(1);
            columnValues.at(1) = 0;
        }
    } catch (const out_of_range& e) {
        wxLogError("Index out of range: %s", e.what());
    }
}

void Game2048Frame::shiftRight(int idx, vector<int>& columnValues) {
    try {
        for (int arrayIndex = idx; arrayIndex >= 0; arrayIndex--) {
            if (arrayIndex == 0) {
                columnValues.at(arrayIndex) = 0;
            } else {
                columnValues.at(arrayIndex) = columnValues.at(arrayIndex - 1);
            }
            if (arrayIndex != 3) {
                if (columnValues.at(arrayIndex) != 0 && columnValues.at(arrayIndex + 1) == 0) {
                    columnValues.at(arrayIndex + 1) = columnValues.at(arrayIndex);
                    columnValues.at(arrayIndex) = 0;
                }
            }
        }
        if (columnValues.at(3) == 0) {
            columnValues.at(3) = columnValues.at(2);
            columnValues.at(2) = 0;
        }
    } catch (const out_of_range& e) {
        wxLogError("Index out of range: %s", e.what());
    }
}

bool Game2048Frame::MoveUp() {
    cout << "-------------------MoveUp-----------------" << endl;
    vector<vector<int>> originalBoard = board;
    bool moved = false;
    for (int columnIdx = 0; columnIdx < GRID_SIZE; columnIdx++) {
        vector<int> colVals = columnify(columnIdx, board);
        for (int rowIdx = 0; rowIdx < 4; rowIdx++) {
            if (colVals.at(rowIdx) == 0) {
                shiftLeft(rowIdx, colVals);
            }
        }
        for (int q = 0; q < 3; q++) {
            if (colVals.at(q) == colVals.at(q + 1) && colVals.at(q) != 0) {
                colVals.at(q) += colVals.at(q + 1);
                score += colVals.at(q);
                shiftLeft(q + 1, colVals);
            }
        }
        for (int rowIdx = 0; rowIdx < 4; rowIdx++) {
            if (board[rowIdx][columnIdx] != colVals.at(rowIdx)) {
                moved = true;
            }
            board[rowIdx][columnIdx] = colVals.at(rowIdx);
        }
    }
    return moved;
}

bool Game2048Frame::MoveDown() {
    vector<vector<int>> originalBoard = board;
    bool moved = false;
    cout << "-------------------MoveDown-----------------" << endl;
    for (int columnIdx = 0; columnIdx < GRID_SIZE; columnIdx++) {
        vector<int> colVals = columnify(columnIdx, board);
        for (int rowIdx = 3; rowIdx >= 0; rowIdx--) {
            if (colVals.at(rowIdx) == 0) {
                shiftRight(rowIdx, colVals);
            }
        }
        for (int q = 3; q > 0; q--) {
            if (colVals.at(q) == colVals.at(q - 1) && colVals.at(q) != 0) {
                colVals.at(q) += colVals.at(q - 1);
                score += colVals.at(q);
                shiftRight(q - 1, colVals);
            }
        }
        for (int rowIdx = 0; rowIdx < 4; rowIdx++) {
            if (board[rowIdx][columnIdx] != colVals.at(rowIdx)) {
                moved = true;
            }
            board[rowIdx][columnIdx] = colVals.at(rowIdx);
        }
    }
    return moved;
}

bool Game2048Frame::MoveLeft() {
    vector<vector<int>> originalBoard = board;
    bool moved = false;
    cout << "-------------------MoveLeft-----------------" << endl;
    for (int rowIdx = 0; rowIdx < 4; rowIdx++) {
        vector<int> rowVals = rowify(rowIdx, board);
        for (int colIdx = 0; colIdx < 4; colIdx++) {
            if (rowVals.at(colIdx) == 0) {
                shiftLeft(colIdx, rowVals);
            }
        }
 
        for (int colIdx2 = 0; colIdx2 < 3; colIdx2++) {
            if (rowVals.at(colIdx2) == rowVals.at(colIdx2 + 1) && rowVals.at(colIdx2) != 0) {
                rowVals.at(colIdx2) += rowVals.at(colIdx2 + 1);
                score += rowVals.at(colIdx2);
                shiftLeft(colIdx2 + 1, rowVals);
            }
        }
        for (int colIdx = 0; colIdx < 4; colIdx++) {
            if (board[rowIdx][colIdx] != rowVals.at(colIdx)) {
                moved = true;
            }
            board[rowIdx][colIdx] = rowVals.at(colIdx);
        }
    }
    return moved;
}

bool Game2048Frame::MoveRight() {
    cout << "-------------------MoveRight-----------------" << endl;
    vector<vector<int>> originalBoard = board;
    bool moved = false;
    for (int rowIdx = 0; rowIdx < 4; rowIdx++) {
        vector<int> rowVals = rowify(rowIdx, board);
        for (int colIdx = 3; colIdx >= 0; colIdx--) {
            if (rowVals.at(colIdx) == 0) {
                shiftRight(colIdx, rowVals);
            }
        }
        for (int colIdx = 3; colIdx > 0; colIdx--) {
            if (rowVals.at(colIdx) == rowVals.at(colIdx - 1) && rowVals.at(colIdx) != 0) {
                rowVals.at(colIdx) += rowVals.at(colIdx - 1);
                score += rowVals.at(colIdx);
                shiftRight(colIdx - 1, rowVals);
            }
        }
        for (int colIdx = 0; colIdx < 4; colIdx++) {
            if (board[rowIdx][colIdx] != rowVals.at(colIdx)) {
                board[rowIdx][colIdx] = rowVals.at(colIdx);
                moved = true;
            }
        }
    }
    return moved;
}

bool Game2048App::OnInit() {
    Game2048Frame* frame = new Game2048Frame();
    frame->Show(true);
    return true;
}

