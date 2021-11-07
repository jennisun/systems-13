#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

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

  for (i = 1; i <= lines; i ++) {
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

  file = open("nyc_pop.data", O_CREAT | O_WRONLY, 0644);
  if (file < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  result = write(file, arr, sizeof(arr));
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  printf("Wrote %lu bytes to nyc_pop.data\n", sizeof(arr));
}

void read_data() {
  int size = filesize("nyc_pop.data");
  int file = open("nyc_pop.data", O_RDONLY);
  if (file < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  struct pop_entry *arr = malloc(file);
  int result = read(file, arr, size);
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  int i = 0;
  struct pop_entry hi;
  for (i = 0; i < 10; i ++) {
    printf("Entry %d\t\tYear: %d\tPopulation: %d\tBorough: %s\n", i, arr[i].year, arr[i].population, arr[i].boro);
  }
  for (int i = 10; i < size / sizeof(hi); i ++) {
    printf("Entry %d\tYear: %d\tPopulation: %d\tBorough: %s\n", i, arr[i].year, arr[i].population, arr[i].boro);
  }
}

void add_data() {
  printf("Enter year, population, and borough: ");

  struct pop_entry hi;
  if (scanf("%d %d %s", &hi.year, &hi.population, hi.boro) != 3) {
    printf("Invalid format. Enter [year] [population] [borough]\n");
    return;
  }

  int file = open("nyc_pop.data", O_WRONLY | O_APPEND);
  if (file < 0) {
    printf("%s\n", strerror(errno));
    return;
  }
  int result = write(file, &hi, sizeof(hi));
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  printf("Appended data to file: \nYear: %d\t Borough: %s\tPopulation: %d\n", hi.year, hi.boro, hi.population);
}

void update_data() {
  read_data();

  struct pop_entry hi;
  int lines = filesize("nyc_pop.data") / sizeof(hi);
  int entry;

  printf("Update entry number: ");
  scanf("%d", &entry);
  if (0 > entry || entry > lines) {
    printf("Invalid entry number\n");
    return;
  }

  printf("Enter year, population, and borough: ");
  if (scanf("%d %d %s", &hi.year, &hi.population, hi.boro) != 3) {
    printf("Invalid format. Enter [year] [population] [borough]\n");
    return;
  }

  int file2 = open("nyc_pop.data", O_WRONLY, 0777);
  lseek(file2, entry * sizeof(hi), SEEK_SET);
  int result = write(file2, &hi, sizeof(hi));
  if (result < 0) {
    printf("%s\n", strerror(errno));
    return;
  }

  printf("File updated\n");
}

int main(int argc, char const *argv[]) {
  if (argv[1] == NULL) {
    printf("Enter -read_csv, -read_data, -add_data, or -update_data\n");
    return 0;
  }
  if (strcmp(argv[1], "-read_csv") == 0) read_csv("data.txt");
  if (strcmp(argv[1], "-read_data") == 0) read_data();
  if (strcmp(argv[1], "-add_data") == 0) add_data();
  if (strcmp(argv[1], "-update_data") == 0) update_data();

  return 0;
}
