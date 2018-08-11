#ifndef DefragRunnerH
#define DefragRunnerH

#include <iostream>
#include <fstream>
using namespace std;

//Need to organize by file names and file block numbers

class DiskDrive;
class DiskBlock;

class DiskBlockInfo // CANNOT ACCESS
{
  unsigned nextBlockID;
  unsigned short fileID;
  unsigned short fileBlockNum;
  friend class DiskDrive;
  friend class DiskBlock;
  void read(ifstream &inf){inf >> fileID >> fileBlockNum >> nextBlockID; }
public:
  DiskBlockInfo(): nextBlockID(0), fileID(0), fileBlockNum(0) {}
}; // class DiskBlockInfo


class DiskBlock // readDiskBlock returns this object
{
  friend class DiskDrive;
  DiskBlockInfo blockInfo;
  DiskBlock(DiskBlockInfo &blockInf) {blockInfo = blockInf;}
public:
  DiskBlock(){}
  unsigned getNext() const  {return blockInfo.nextBlockID;}
  short getFileBlockNum() const {return blockInfo.fileBlockNum;} // USEFUL
}; // class DiskBlock

class DirectoryEntry // Files
{
  unsigned short fileID;
  unsigned firstBlockID;
  unsigned size;
  friend class DiskDrive;
  void read(istream &inf) {inf >> fileID >> firstBlockID >> size;}
public:
  DirectoryEntry(unsigned short n = 0, unsigned f = 0, int s = 0) : fileID(n),
    firstBlockID(f), size(s){}
  bool operator< (const DirectoryEntry &d)const {return fileID < d.fileID;}
  bool operator== (const DirectoryEntry &d)const {return fileID == d.fileID;}
  DirectoryEntry& operator= (const DirectoryEntry *d){return *this;}
    // prevents cheating by altering original
  void print(ostream &outf) const
    {outf << fileID << ' ' << firstBlockID << ' ' << size << endl;}
  unsigned short getFileID() const {return fileID;}
  unsigned getFirstBlockID() const {return firstBlockID;}
  unsigned getSize() const {return size;}
  void setFirstBlockID(unsigned blockID) {firstBlockID = blockID;} // USEFUL?
}; // DirectoryEntry class

class DiskDrive
{
  int numFiles;
  unsigned capacity; // # of disk spaces
  static int diskAccesses;
  DiskBlockInfo *disk;       // cannot access, NEED TO REORDER THIS ARRAY using readDiskBlock and writeDiskBlock
public:
  DirectoryEntry *directory; // sorted by file names. provides file names, starting blockIDs, and sizes of the files
  bool *FAT;                 // each bool indicates whether the corresponding disk block is used(true) or unused(false)
  DiskDrive(){}
  void readFile(const char filename[]);
  int getNumFiles()const {return numFiles;}
  int getCapacity()const {return capacity;}
  int getDiskAccesses() const {return diskAccesses; }
  DiskBlock* readDiskBlock(int diskBlockID) // USE
    { diskAccesses++ ; return new DiskBlock(disk[diskBlockID]); }
  void writeDiskBlock(DiskBlock *diskBlock, int diskBlockID) // USE
  {
    disk[diskBlockID] = diskBlock->blockInfo;
    diskAccesses++;
  }  // copies diskBlockInfo
  void check();
} ;  // DiskDrive class


#endif
