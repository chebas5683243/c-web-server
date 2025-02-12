#include <stdio.h>

void printSeparator() {
  puts("-----------------------------------");
}

void printSection(char *sectionName){
  printSeparator();
  printf("%s\n", sectionName);
  printSeparator();
}
