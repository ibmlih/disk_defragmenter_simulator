        #include "QuadraticProbing.h"
        #include "mynew.h"
        #include "defragmenter.h"
        #include "DefragRunner.h"


        /**
         * Internal method to test if a positive number is prime.
         * Not an efficient algorithm.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isPrime( int n ) const
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        }

        /**
         * Internal method to return a prime number at least as large as n.
         * Assumes n > 0.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::nextPrime( int n ) const
        {
            if( n % 2 == 0 )
                n++;

            for( ; !isPrime( n ); n += 2 )
                ;

            return n;
        }

        /**
         * Construct the hash table.
         */
        template <class HashedObj>
        QuadraticHashTable<HashedObj>::QuadraticHashTable( const HashedObj & notFound, int size )
          : array( nextPrime( size ) ), ITEM_NOT_FOUND( notFound )
        {
            makeEmpty( );
        }

        /**
         * Insert item x into the hash table. If the item is
         * already present, then do nothing.
         */


        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::insert(  HashedObj & x )
        {
                // Insert x as active
            int currentPos = findPos( x );
            if( isActive( currentPos ) )
                return;
            array[ currentPos ] = HashEntry( x, 1 );

            currentSize++;
        }

        /**
         * Expand the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::rehash( )
        {
            vector<HashEntry> oldArray = array;

                // Create new double-sized, empty table
            array.resize( nextPrime( 2 * oldArray.size( ) ) );
            for( int j = 0; j < array.size( ); j++ )
                array[ j ].info = 0;

                // Copy table over
            currentSize = 0;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == 1 )
                    insert( oldArray[ i ].element );
        }


        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::rehashToSame( )
        {
            vector<HashEntry> oldArray = array;


            for( int j = 0; j < array.size( ); j++ )
                array[ j ].info = 0;

                // Copy table over
            currentSize = 0;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == 1 ) // && old
                    insert( oldArray[ i ].element);
        }

        /**
         * Method that performs quadratic probing resolution.
         * Return the position where the search for x terminates.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::findPos( const HashedObj & x ) const
        {
            // Linear probing

/* 1*/      //int collisionNum = 0;
/* 2*/      int currentPos = hash( x.getIndex(), array.size( ) );

/* 3*/      while( array[ currentPos ].info != 0 &&
                   array[ currentPos ].element != x )
            {
/* 4*/          currentPos += 1;  // Compute ith probe
/* 5*/          if( currentPos >= array.size( ) )
/* 6*/              currentPos -= array.size( );
            }

/* 7*/      return currentPos;
        }


        /**
         * Remove item x from the hash table.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::remove( const HashedObj & x )
        {
            int currentPos = findPos( x );
            if( isActive( currentPos ) )
            {
                array[ currentPos ].info = -1;
            }
        }



        template <class HashedObj>
        const HashedObj & QuadraticHashTable<HashedObj>::find(int blockID) const
        {
            int currentPos = findPos2( blockID );
            return isActive( currentPos ) ? array[ currentPos ].element : ITEM_NOT_FOUND;
        }


        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::findPos2( int blockID ) const
        {
            // Linear Probing
/* 1*/      //int collisionNum = 0;
/* 2*/      int currentPos = hash( blockID, array.size( ) );

/* 3*/      while( array[ currentPos ].info != 0 &&
                   array[ currentPos ].element.getIndex() != blockID)
            {
/* 4*/          currentPos += 1;  // Compute ith probe
/* 5*/          if( currentPos >= array.size( ) )
/* 6*/              currentPos -= array.size( );
            }

/* 7*/      return currentPos;
        }




        /**
         * Make the hash table logically empty.
         */
        template <class HashedObj>
        void QuadraticHashTable<HashedObj>::makeEmpty( )
        {
            currentSize = 0;
            for( int i = 0; i < array.size( ); i++ )
                array[ i ].info = 0;
        }

        /**
         * Deep copy.
         */
        template <class HashedObj>
        const QuadraticHashTable<HashedObj> & QuadraticHashTable<HashedObj>::
        operator=( const QuadraticHashTable<HashedObj> & rhs )
        {
            if( this != &rhs )
            {
                array = rhs.array;
                currentSize = rhs.currentSize;
            }
            return *this;
        }


        /**
         * Return true if currentPos exists and is active.
         */
        template <class HashedObj>
        bool QuadraticHashTable<HashedObj>::isActive( int currentPos ) const
        {
            return array[ currentPos ].info == 1;
        }

        /**
         * A hash routine for string objects.
         */
        template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( const string & key, int tableSize ) const
        {
            int hashVal = 0;

            for( int i = 0; i < key.length( ); i++ )
                hashVal = 37 * hashVal + key[ i ];

            hashVal %= tableSize;
            if( hashVal < 0 )
                hashVal += tableSize;

            return hashVal;
        }


        /**
         * A hash routine for ints.
         */
         template <class HashedObj>
        int QuadraticHashTable<HashedObj>::hash( int key, int tableSize ) const
        {
            if( key < 0 ) key = -key;
            return key % tableSize;
        }

