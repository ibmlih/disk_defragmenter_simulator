#ifndef MYNEW_H
  #define MYNEW_H
#include <stdexcept>
#include <new>

void* operator new(std::size_t size) throw (std::bad_alloc);
/* The call new T results in a call to operator new(sizeof(T))
   to allocate sizeof(T) bytes of memory, and then a constructor to T is called
   in the location returned, based on the parameters used after T
*/

void operator delete(void* to_delete) throw ();
/* all this needs to do is the inverse of what *you* did in operator new
you don't need to call destructors yourself or anything like that */

void* operator new[](size_t size) throw (std::bad_alloc);
/* The call new T[n] results in a call to operator new(sizeof(T)*n)
   to allocate sizeof(T)*n bytes of memory, and then the default constructor
   to T is called on each space allocated
*/

void operator delete[](void* to_delete) throw ();
/* all this needs to do is the inverse of what *you* did in operator new[]
you don't need to call destructors yourself or anything like that*/

#endif
