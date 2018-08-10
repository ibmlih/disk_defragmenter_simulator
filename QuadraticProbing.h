        #ifndef _QUADRATIC_PROBING_H_
        #define _QUADRATIC_PROBING_H_

        #include "vector.h"
        #include "mystring.h"
        #include "mynew.h"
        #include "defragmenter.h"
        #include "DefragRunner.h"

        // QuadraticProbing Hash table class
        //
        // CONSTRUCTION: an initialization for ITEM_NOT_FOUND
        //               and an approximate initial size or default of 101
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Hashable find( x )     --> Return item that matches x
        // void makeEmpty( )      --> Remove all items
        // int hash( String str, int tableSize )
        //                        --> Static method to hash strings

        template <class HashedObj>
        class QuadraticHashTable
        {
          public:
            explicit QuadraticHashTable( const HashedObj & notFound, int size = 101 );
            QuadraticHashTable( const QuadraticHashTable & rhs )
              : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                array( rhs.array ), currentSize( rhs.currentSize ) { }

            //const HashedObj & find( const HashedObj & x ) const;
            const HashedObj & find( int blockID ) const;

            void makeEmpty( );

            void insert(  HashedObj & x );
            void remove( const HashedObj & x );
            void rehashToSame();


            const QuadraticHashTable & operator=( const QuadraticHashTable & rhs );

            //enum EntryType { ACTIVE, EMPTY, DELETED };
            int getCurrentSize() {return currentSize;}
          private:
            friend class IndexMap;
            friend class Defragmenter;

            struct HashEntry
            {
                HashedObj element;
                int info; // 0 is EMPTY, 1 is ACTIVE, -1 is DELETED

                HashEntry( const HashedObj & e = HashedObj( ), int i = 0 )
                  : element( e ), info( i ) { }
            };

            vector<HashEntry> array;
            int currentSize;
            const HashedObj ITEM_NOT_FOUND;
            bool isPrime( int n ) const;
            int nextPrime( int n ) const;
            bool isActive( int currentPos ) const;
            int findPos( const HashedObj & x ) const;
            int findPos2( int blockID ) const;
            int hash( const string & key, int tableSize ) const;
            int hash( int key, int tableSize ) const;
            void rehash( );
        };

        #include "QuadraticProbing.cpp"
        #endif
