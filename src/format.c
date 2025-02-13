#include <stdio.h>

void print_separator() {
  puts("-----------------------------------");
}

void print_section(char *sectionName){
  print_separator();
  printf("%s\n", sectionName);
  print_separator();
}
