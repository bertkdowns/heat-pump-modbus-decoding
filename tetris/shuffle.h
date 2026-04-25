int piece_buffer[7] = {0, 1, 2, 3, 4, 5, 6};
int index = 0;
void shuffle_buffer(){
  for (int i = 0; i < 7; i++) {
    int j = random(0, 7);
    int temp = piece_buffer[i];
    piece_buffer[i] = piece_buffer[j];
    piece_buffer[j] = temp;
  }
}
int chooseRandomPiece() {
  if (index >= 7) {
    shuffle_buffer();
    index = 0;
  }
  return piece_buffer[index++];
}