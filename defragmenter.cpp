// Author Eunsub Lee
#include <iostream>
#include <cstddef>
#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "QuadraticProbing.h"
#include "SplayTree.h"

using namespace std;


Defragmenter::Defragmenter(DiskDrive *diskDrive) : indexLimit(2), count(1), hashTable(IndexMap(0, 0), 200000), splay(-1), tableIndex(-1) // in QuadraticProbing too
{
  tempArray = new DiskBlock*[12000];

  for (int i = 1 ; i < 12000; i++)
  {
    tempArray[i] = NULL;
    splay.insert(i);
  }

  int numFiles = diskDrive->getNumFiles();

  for (int i = 0; i < numFiles; i++)
    insertEachFile(diskDrive, diskDrive->directory[i]);
} // Defragmenter()


void Defragmenter::insertEachFile(DiskDrive *diskDrive, DirectoryEntry& directory)
{
  int blockID = directory.getFirstBlockID();
  int size = directory.getSize();
  directory.setFirstBlockID(indexLimit);

  for (int i = 0; i < size; i++)
  {
    if (blockID < indexLimit) // look at hash table
      inHashTable(diskDrive, blockID);

    else // in DiskDrive
      inDiskDrive(diskDrive, blockID);
  } // for each file

  if (count > 9500) // tempArray getting full
  { 
    writeBack(diskDrive);
    hashTable.rehashToSame();
  }
}


void Defragmenter::writeBack(DiskDrive* diskDrive)
{
  int counter = 0; // how many to write back

  for (int j = diskDrive->getCapacity() - 1; j > 2 && counter < 8000; j--) // start at the end
  {
    if (diskDrive->FAT[j] == false) // empty space
    {
      int n = findNext();
      IndexMap indexMap(hashTable.find(n));
      int arrayPos = indexMap.getDest() * -1;
      hashTable.remove(indexMap);
      indexMap.setDest(j); // j > 0
      hashTable.insert(indexMap);
      DiskBlock* diskBlock = tempArray[arrayPos];
      diskDrive->writeDiskBlock(diskBlock, j);
      diskDrive->FAT[j] = true;

      tempArray[arrayPos] = NULL;
      splay.insert(arrayPos);
      count--;

      delete diskBlock;
      counter++;
    }
  } // for
}


void Defragmenter::inDiskDrive(DiskDrive *diskDrive, int& blockID)
{
  DiskBlock* diskBlock;
  diskBlock = diskDrive->readDiskBlock(blockID);
  diskDrive->FAT[blockID] = false;
  write(diskDrive, diskBlock, blockID);
}


void Defragmenter::inHashTable(DiskDrive *diskDrive, int& blockID)
{
  DiskBlock* diskBlock;
  IndexMap indexMap(hashTable.find(blockID));

  if (indexMap.getDest() < 0) // in tempArray, easy part
  {
    int pos = indexMap.getDest() * -1;
    diskBlock = tempArray[pos];
    tempArray[pos] = NULL;
    splay.insert(pos);
    count--;

    hashTable.remove(indexMap);
    tableIndex.remove(blockID);

    write(diskDrive, diskBlock, blockID);
  } // if in tempArray

  else // in DiskDrive, more complicated
  {
    int prev = 0, pos = blockID;

    while (1)
    {
      IndexMap index(hashTable.find(pos));
      prev = pos;
      pos = index.getDest();
      hashTable.remove(index);

      if (pos < 0)
      {
        tableIndex.remove(prev);
        diskBlock = tempArray[-1 * pos];
        tempArray[-1 * pos] = NULL;
        splay.insert(-1 * pos);
        count--;
        break;
      }

      if (pos >= indexLimit) 
      {
        diskBlock = diskDrive->readDiskBlock(pos);
        diskDrive->FAT[pos] = false;
        break;
      }
    } // while()

    write(diskDrive, diskBlock, blockID);
  } // else, in DiskDrive
}


void Defragmenter::write(DiskDrive *diskDrive, DiskBlock* diskBlock, int& blockID)
{
  if (diskDrive->FAT[indexLimit] == true) // need to store it in tempArray and hash table
  {
    DiskBlock* ptr = diskDrive->readDiskBlock(indexLimit);
    int pos = nextEmpty();
    tempArray[pos] = ptr;
    splay.remove(pos);
    count++;

    IndexMap map(indexLimit, -1 * pos); // negative to indicate in tempArray
    hashTable.insert(map);
    tableIndex.insert(indexLimit);
  }// if

  diskDrive->writeDiskBlock(diskBlock, indexLimit);
  diskDrive->FAT[indexLimit] = true; // used space now
  indexLimit++;
            
  blockID = diskBlock->getNext(); 

  delete diskBlock;
} // write()


int Defragmenter::nextEmpty()
{
  return splay.findMin();
}


int Defragmenter::findNext()
{
  int n = tableIndex.findMin();
  tableIndex.remove(n);
  return n;
}
