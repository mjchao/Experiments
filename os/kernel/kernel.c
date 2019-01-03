// ???
void entrypoint() {
}

void main() {
  // video memory is at 0xb8000 and points to top-left of screen
  char* video_memory = (char*) 0xb8000;
  
  // write 'X' to top-left of screen
  *video_memory = 'X';
}

