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

long long filesize(char * name) {
  struct stat stats;
  stat(name, &stats);
  return stats.st_size;
}

void read_file(char * name) {
  int file = open(name, O_RDONLY);
  if (file < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  char data[filesize(name)];
  int result = 0;

  result = read(file, data, sizeof(data));
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  // int hold[3];
  // printf("%s\n", data + 6);
  // sscanf(data + 6, "%d, %d, %d", &hold[0], &hold[1], &hold[2]);
  //
  // printf("%d\n%d\n%d\n", hold[0], hold[1], hold[2]);

  int i = 0;
  int lines = -1;
  for (i = 0; i < sizeof(data); i ++) {
    if (data[i] == '\n') lines += 1;
  }

  int bytes[lines];
  int lineNum = 0;
  for (i = 0; i < sizeof(data); i ++) {
    if (data[i] == '\n') {
      if (lineNum == 0) bytes[lineNum] = i;
      else bytes[lineNum] = i - bytes[lineNum - 1];
      lineNum += 1;
    }
  }

  int file1 = open(name, O_RDONLY);
  char temp[bytes[0]];
  result = read(file1, temp, sizeof(temp));

  struct pop_entry arr[lines * 5];
  char *borough[5] = {"Manhattan", "Brooklyn", "Queens", "Bronx", "Staten Island"};

  for (i = 1; i < lines; i ++) {
    char temp[bytes[i]];
    result = read(file1, temp, sizeof(temp));

    int hold[6];
    sscanf(temp, "%d, %d, %d, %d, %d, %d", &hold[0], &hold[1], &hold[2], &hold[3], &hold[4], &hold[5]);
    printf("%d\t%d\t\n", hold[0], hold[1]);
  }


}

int main() {
  read_file("data.txt");

  return 0;
}
