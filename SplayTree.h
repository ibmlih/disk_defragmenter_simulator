#ifndef _SPLAY_TREE_H_
#define _SPLAY_TREE_H_

#include "dsexceptions.h"
#include "mynew.h"
#include <cstdlib>

        // SplayTree class
        //
        // CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Comparable find( x )   --> Return item that matches x
        // Comparable findMin( )  --> Return smallest item
        // Comparable findMax( )  --> Return largest item
        // boolean isEmpty( )     --> Return true if empty; else false
        // void makeEmpty( )      --> Remove all items
        // void printTree( )      --> Print tree in sorted order


          // Node and forward declaration because g++ does
          // not understand nested classes.
template <class Comparable>
class SplayTree;

template <class Comparable>
class SplayBinaryNode
{
	Comparable  element;
  SplayBinaryNode *left;
  SplayBinaryNode *right;

  SplayBinaryNode( ) : left( NULL ), right( NULL ) { }
  SplayBinaryNode( const Comparable & theElement, SplayBinaryNode *lt, SplayBinaryNode *rt )
  : element( theElement ), left( lt ), right( rt ) { }
  friend class SplayTree<Comparable>;
};


template <class Comparable>
class SplayTree
{
public:
            explicit SplayTree( const Comparable & notFound );
            SplayTree( const SplayTree & rhs );
            ~SplayTree( );

            const Comparable & findMin( );
            const Comparable & findMax( );
            const Comparable & find( const Comparable & x );
            bool isEmpty( ) const;
            void printTree( ) const;

            void makeEmpty( );
            void insert( const Comparable & x );
            void remove( const Comparable & x );

            const SplayTree & operator=( const SplayTree & rhs );

          private:


            SplayBinaryNode<Comparable> *root;
            SplayBinaryNode<Comparable> *nullNode;
            const Comparable ITEM_NOT_FOUND;

            const Comparable & elementAt( SplayBinaryNode<Comparable> *t ) const;

            void reclaimMemory( SplayBinaryNode<Comparable> * t ) const;
            void printTree( SplayBinaryNode<Comparable> *t ) const;
            SplayBinaryNode<Comparable> * clone( SplayBinaryNode<Comparable> *t ) const;

                // Tree manipulations
            void rotateWithLeftChild( SplayBinaryNode<Comparable> * & k2 ) const;
            void rotateWithRightChild( SplayBinaryNode<Comparable> * & k1 ) const;
            void splay( const Comparable & x, SplayBinaryNode<Comparable> * & t ) const;
        };

        #include "SplayTree.cpp"
        #endif
