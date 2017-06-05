#include <stdio.h>
#include <unistd.h>

long get_free_units() {
   return syscall(353);
}

long get_allocated_units() {
   return syscall(354);
}

int main() {
   long f = get_free_units();
   long a = get_allocated_units();

   float fragmentation = f / (float) a;

   printf("allocated units: %lu\n", a);
   printf("free units: %lu\n", f);
   printf("fragmentation: %f\n", fragmentation);
}
