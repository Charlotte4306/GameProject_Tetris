#ifndef _SCORE_H_
#define _SCORE_H_

struct Score {
    int score;          // Điểm số hiện tại
    int level;          // Level hiện tại
    int linesCleared;   // Tổng số dòng đã xóa
    int linesPerLevel;  // Số dòng cần xóa để tăng 1 level
    int baseDelay;      // Thời gian delay cơ bản (ms) cho Tetromino rơi
    int currentDelay;   // Thời gian delay hiện tại, giảm khi level tăng
    int combo;          // Đếm số lần xóa dòng liên tiếp (combo)
    bool lastWasTetris; // Kiểm tra xem lần xóa trước có phải là Tetris không

    Score();
    void addLines(int lines); // Cộng điểm và kiểm tra tăng level khi xóa dòng
    void reset();             // Reset điểm số, level, combo và trạng thái Tetris
    int getDelay() const;     // Lấy delay hiện tại cho Tetromino
};

#endif _SCORE_H_