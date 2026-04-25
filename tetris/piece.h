#include "./display_data.h"
#include "./tetronimos.h"
class TetrisPiece{
  public: 
    int x;
    int y;
    bool piece[4][4] = {false};
    void show(){
      show(true);
    }
    void hide(){
      show(false);
    }
    void show(bool visible){
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (piece[i][j]) {
            display_data[x + i][y + j] = visible;
          }
        }
      }
    } 
    void rotate(){
      // Implement rotation logic here
      bool temp[4][4] = {false};
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          temp[j][3 - i] = piece[i][j];
        }
      }
      memcpy(piece, temp, sizeof(piece));
    }
    bool is_colliding(){
      return is_colliding(x, y);
    }
    bool is_colliding(int x, int y){
      // Implement collision detection logic here
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (piece[i][j] && (x + i >= 16 || y + j < 0 || y + j >= 32 || display_data[x + i][y + j])) {
            return true;
          }
        }
      }
      return false;
    }
    void switchPiece(int new_type){
      memcpy(piece, tetronimos[new_type], sizeof(piece));
    }
};