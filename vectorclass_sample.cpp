#include<stdio.h>
#include"vectorclass.h"

int main () {

Vec4i a(10,11,12,13);
Vec4i b(20,21,22,23);

Vec4i c = a + b;

for (int i = 0; i < c.size(); i++) {
printf(" %5i", c[i]);
}
printf("\n");
return 0;
}