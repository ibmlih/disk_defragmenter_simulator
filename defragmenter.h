// Author Eunsub Lee
#ifndef defragmenterH
  #define defragmenterH

#include <iostream>
#include <cstddef>
#include "mynew.h"
#include "DefragRunner.h"
#include "QuadraticProbing.h"
#include "SplayTree.h"

class IndexMap
{
  int index; // original position, key in hash table
  int dest; // negative = in tempArray, positive = in DiskDrive
  friend class Defragmenter;
public:
  IndexMap(int i = 0, int d = 0) : index(i), dest(d) {}
  ~IndexMap() {}
  IndexMap(const IndexMap& rhs) : index(rhs.getIndex()), dest(rhs.getDest()) {}
  int getIndex() const {return index;}
  int getDest() const {return dest;}
  void setDest(int destination) {dest = destination;}

  bool operator!= (const IndexMap& rhs) const
  {
    if (getIndex() == rhs.getIndex())
      return false;

    return true;
  }
}; // class Index Map


class Defragmenter
{
  int indexLimit; // less than index limit go to hash table
  int count; // for tempArray
  QuadraticHashTable <IndexMap> hashTable;
  DiskBlock** tempArray;
  SplayTree <int> splay; // for tracking empty spaces in tempArray
  SplayTree <int> tableIndex; // track dest < 0 in hashTable
  friend class IndexMap;
public:
  Defragmenter(DiskDrive *diskDrive);
  ~Defragmenter() {}
  void insertEachFile(DiskDrive *diskDrive, DirectoryEntry& directory);
  void inDiskDrive(DiskDrive *diskDrive, int& blockID);
  void inHashTable(DiskDrive *diskDrive, int& blockID);
  void write(DiskDrive *diskDrive, DiskBlock* diskBlock, int& blockID);
  int nextEmpty() ;
  int findNext();
  void writeBack(DiskDrive* diskDrive);
}; // class Defragmenter



#endif
