#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

struct pop_entry {
  int year;
  int population;
  char boro[15];
};

int main() {
  int file = open("data.txt", O_RDONLY);
  int result = 0;

  if (file < 0) {
    printf("%s\n", strerror(errno));
    return -1;
  }

  struct pop_entry hi;
  char headings[51];
  int temp[4];
  result = read(file, headings, sizeof(headings));
  // result = read(file, temp, 4);
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return -1;
  }
  // hi -> year = temp;


  printf("%s\n", headings);

  return 0;
}
