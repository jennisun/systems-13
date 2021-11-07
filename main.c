#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

struct pop_entry {
  int year;
  int population;
  char boro[15];
};

void read_file(char * name) {
  int file = open(name, O_RDONLY);
  struct stat stats;
  stat(name, &stats);
  long long size = stats.st_size;

  char data[size];
  int result = 0;

  if (file < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  result = read(file, data, sizeof(data));
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }



}

int main() {
  read_file("data.txt");

  return 0;
}
