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

void read_csv(char * name) {
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

  int i, j = 0;
  int lines = -1;
  for (i = 0; i < sizeof(data); i ++) {
    if (data[i] == '\n') lines += 1;
  }

  int bytes[lines];
  int lineNum = 0;
  for (i = 0; i < sizeof(data); i ++) {
    if (data[i] == '\n') {
      bytes[lineNum] = i;
      lineNum += 1;
    }
  }

  int file1 = open(name, O_RDONLY);
  char temp[bytes[0]];
  result = read(file1, temp, sizeof(temp));

  struct pop_entry arr[lines * 5];
  char *borough[5] = {"Manhattan", "Brooklyn", "Queens", "Bronx", "Staten Island"};

  for (i = 1; i < lines; i ++) {
    char temp[bytes[i] - bytes[i - 1]];
    result = read(file1, temp, sizeof(temp));

    int hold[6];
    sscanf(temp, "%d, %d, %d, %d, %d, %d", &hold[0], &hold[1], &hold[2], &hold[3], &hold[4], &hold[5]);

    for (j = 1 ; j < 6; j ++) {
      int index = 5 * (i - 1) + j - 1;
      arr[index].year = hold[0];
      arr[index].population = hold[j];
      strcpy(arr[index].boro, borough[j - 1]);
      printf("%d\t%d\t%s\n", arr[index].year, arr[index].population, arr[index].boro);


    }
  }


}

int main() {
  read_csv("data.txt");

  return 0;
}
