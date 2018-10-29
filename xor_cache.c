#include <stdio.h>
#include <stdint.h>

static unsigned long long rdtscp(void)
{
  unsigned int hi, lo;
  __asm__ __volatile__("rdtscp" : "=a"(lo), "=d"(hi));
  return (unsigned long long)lo | ((unsigned long long)hi << 32);
}
#define CHUNKSIZE (1L<<28-2)
#define PROCSIZE  0x200000000

unsigned long long a[CHUNKSIZE], mx, before, after, rez; 

void main()
{
  unsigned long long i, rndx, rnda, rndc;
  unsigned long long proc = 0;
  
  rndx = 1;
  rnda = 1103515245; 
  rndx = 12345;
  
  for(i = 0; i < CHUNKSIZE; i++){
     a[i] = rndx = (rnda * rndx + rndc);
  }
  
  mx = CHUNKSIZE;
  while(mx >= (1L<<9)){
    before = rdtscp(); 
    rez = 0xAAAAAAAAAAAAAAAA;
    proc = 0;
    while(proc < PROCSIZE){
      //printf("size= %llx mx=%llx proc= %llx\n", CHUNKSIZE, mx, proc);
      for(i = 0; i < mx; i++){
	rez += a[i]; 
      }
      proc += mx; 
    }
    
    after = rdtscp();
    //printf("size= %llx mx=%llx proc= %llx\n", CHUNKSIZE, mx, proc);
    printf("size= %llx %lld cycles= %lld\n", mx*8, mx*8, (after - before));
    fflush(stdout); 
    mx = mx >> 2; 
  }

}
