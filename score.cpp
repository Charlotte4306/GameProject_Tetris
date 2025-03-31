#include "score.h"

Score::Score() {
    score = 0;
    level = 1;
    linesCleared = 0;
    linesPerLevel = 10; // Cần xóa 10 dòng để tăng 1 level
    baseDelay = 500;    // Delay cơ bản: 500ms
    currentDelay = baseDelay;
    combo = 0;          // Khởi tạo combo
    lastWasTetris = false; // Khởi tạo trạng thái Tetris liên tiếp
}

void Score::addLines(int lines) {
    if (lines == 0) {
        combo = 0;
        lastWasTetris = false;
        return;
    }
    int points = 0;
    bool isTetris = (lines == 4);
    if (isTetris) {
        if (lastWasTetris) {
            points = 1200; // Tetris liên tiếp: 1200 điểm
        }
        else {
            points = 800;  // Tetris đầu tiên: 800 điểm
        }
    }
    else {
        switch (lines) {
        case 1: points = 100; break; // Xóa 1 dòng: 100 điểm
        case 2: points = 300; break; // Xóa 2 dòng: 300 điểm
        case 3: points = 500; break; // Xóa 3 dòng: 500 điểm
        }
    }

    // Tính điểm combo: mỗi lần xóa liên tiếp nhân điểm cơ bản với (1 + combo * 0.5)
    combo++;
    int comboMultiplier = 1 + (combo - 1) * 0.5; // Hệ số combo: 1x, 1.5x, 2x, ...
    points *= comboMultiplier;

    // Cộng điểm vào tổng
    score += points;

    // Cập nhật trạng thái Tetris liên tiếp
    lastWasTetris = isTetris;

    // Cộng số dòng đã xóa
    linesCleared += lines;

    // Kiểm tra tăng level
    int newLevel = (linesCleared / linesPerLevel) + 1;
    if (newLevel > level) {
        level = newLevel;
        // Giảm delay khi tăng level (tăng tốc độ rơi)
        currentDelay = baseDelay - (level - 1) * 50; // Mỗi level giảm 50ms
        if (currentDelay < 100) currentDelay = 100;  // Giới hạn delay tối thiểu
    }
}

void Score::reset() {
    score = 0;
    level = 1;
    linesCleared = 0;
    currentDelay = baseDelay;
    combo = 0;          // Reset combo
    lastWasTetris = false; // Reset trạng thái Tetris
}

int Score::getDelay() const {
    return currentDelay;
}