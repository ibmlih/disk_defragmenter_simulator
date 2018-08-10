#define _USE_OLD_RW_STL

#include <cstdlib> //needed for malloc
#include <iostream>
#include "mynew.h"
// #define SHOW_SIZE
using namespace std;

int maxRAM = 0;
int currentRAM = 0;

void* operator new(std::size_t size) throw (std::bad_alloc) //size is in bytes
{
  size_t realSize;
  
  if(size < 25)
    realSize = 32;
  else
    realSize = 40 + ((size - 25) / 16) * 16;
  
  currentRAM += realSize;
#ifdef SHOW_SIZE
  cout << "Requesting: " << size << " Getting: " << realSize << endl;
#endif
  
  if(currentRAM > maxRAM)
    maxRAM = currentRAM;
  
	size_t * ret = (size_t*) malloc(sizeof(size_t) + size);
  *ret = realSize;
  return (void*) &ret[1];
}

void operator delete(void* to_delete) throw ()
{
#ifdef SHOW_SIZE
  cout << "Releasing: " << ((size_t*) to_delete)[-1] << endl;
#endif
  currentRAM -= ((size_t*) to_delete)[-1];
	free((size_t*)to_delete - 1);
}

void* operator new[] (size_t size) throw (std::bad_alloc) //size is in bytes
{
  size_t realSize;
  
  if(size < 25)
    realSize = 32;
  else
    realSize = 40 + ((size - 25) / 16) * 16;
  
  currentRAM += realSize;
#ifdef SHOW_SIZE
  cout << "Requesting: " << size << " Getting: " << realSize << endl;
#endif
  
  if(currentRAM > maxRAM)
    maxRAM = currentRAM;
  
	size_t * ret = (size_t*) malloc(sizeof(size_t) + size);
  *ret = realSize;
  return (void*) &ret[1];
 }

void operator delete[] (void* to_delete) throw ()
{
#ifdef SHOW_SIZE
  cout << "Releasing: " << ((size_t*) to_delete)[-1] << endl;
#endif
  currentRAM -= ((size_t*) to_delete)[-1];
	free((size_t*)to_delete - 1);
}


