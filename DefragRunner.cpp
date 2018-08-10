#include <fstream>
#include <iostream>
#include "DefragRunner.h"
#include "mynew.h"
#include "CPUTimer.h"
#include "defragmenter.h"

using namespace std;

extern int maxRAM;
extern int currentRAM;
int DiskDrive::diskAccesses = 0;

void DiskDrive::readFile(const char filename[])
{
  DirectoryEntry directoryEntry;
  int ID;
  ifstream inf(filename);
  inf >> capacity >> numFiles;
  FAT = new bool[capacity];
  disk = new DiskBlockInfo[capacity];
  directory = new DirectoryEntry[numFiles];

  for(int i = 0; i < numFiles; i++)
    directory[i].read(inf);

  for(unsigned i = 0; i < capacity; i++)
  {
    inf >> ID;
    disk[ID].read(inf);
    FAT[ID] = disk[ID].fileBlockNum != 0;
  }

  FAT[0] = FAT[1] = true; // for FAT table
} // readFile()

void DiskDrive::check()
{
  unsigned i, block, size;
  bool *FAT2 = new bool[capacity];

  for(i = 0; i < capacity; i++)
    FAT2[i] = false;

  for(i = 0; i < (unsigned) numFiles; i++)
  {
    block = directory[i].getFirstBlockID();
    unsigned short fileID = directory[i].fileID;
    size = directory[i].getSize();
    for(unsigned j = 0; j < size; j++)
    {
      if(block + j >= capacity)
      {
        cout << "FileID: " << fileID << " ends beyond the disk drive capacity\n";
        break;
      }
      if(unsigned(disk[block + j].fileBlockNum) != j + 1)
        cout << "Wrong fileBlockNum: " << disk[block + j].fileBlockNum
          << " filename: " << fileID
          << " disk block: " << block + j << " should be " << j + 1 << endl;

      if(disk[block + j].fileID != fileID)
        cout << "Wrong fileID: " << disk[block + j].fileID
          << " disk block: " << block + j << " should be " << fileID << endl;

      FAT2[block + j] = true;
    }  // for each block in the file
  } // for each file

  for(i = 2; i < capacity && FAT2[i]; i++);

  for(; i < capacity && !FAT2[i]; i++);

  if(i < capacity)
    cout << "Discontinuous disk storage.  Disk block: " << i
      << " is used after " << i - 1 << " was unused.\n";
} // check()


int main(int argc, char *argv[])
{
  DiskDrive diskDrive;
  diskDrive.readFile(argv[1]);
  CPUTimer ct;
  currentRAM = maxRAM = 0;
  ct.reset();
  new Defragmenter(&diskDrive);
  cout << "CPU Time: " << ct.cur_CPUTime() << " Disk accesses: "
    << diskDrive.getDiskAccesses() << " RAM: " << maxRAM << endl;
  diskDrive.check();
  return 0;
}  // main


