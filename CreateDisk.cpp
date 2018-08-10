#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <climits>
#include <set>
#include <iostream>

// Next blocks of zero indicate empty block, of 1 indicates end of file

using namespace std;

class DiskBlock
{
public:
  unsigned short name;
  unsigned short fileBlockNum;
  unsigned nextBlockID;
  DiskBlock(): name(0), fileBlockNum(0), nextBlockID(0){}
}; // class DiskBlock

class DirectoryEntry
{
  unsigned short name;
  unsigned firstBlock;
  int size;
public:
  DirectoryEntry(unsigned short n, unsigned f, int s) : name(n), firstBlock(f), size(s){}
  bool operator< (const DirectoryEntry &d)const {return name < d.name;}
  bool operator== (const DirectoryEntry &d)const {return name == d.name;}
  void print(ostream &outf) const
    {outf << name << " " << firstBlock << " " << size << endl;}
};

void  createFragmentedFile(set<DirectoryEntry> &directory, DiskBlock *disk,
  vector <bool> &FAT, unsigned short name, int size, int capacity)
{
  int ID, oldID, fileBlockNum = 0;;

  do
  {
    ID = (abs(rand() * rand()) % (capacity - 2)) + 2; // 1 < ID < capacity
  } while(FAT[ID]);

  directory.insert(DirectoryEntry(name, ID, size));
  disk[ID].name = name;
  disk[ID].fileBlockNum = ++fileBlockNum;
  FAT[ID] = true;

  for(int i = 1; i < size; i++)
  {
    oldID = ID;
    do
    {
      ID = (abs(rand() * rand()) % (capacity - 2)) + 2; // // 1 < ID < capacity
    } while(FAT[ID]);

    disk[oldID].nextBlockID = ID;
    disk[ID].name = name;
    disk[ID].fileBlockNum = ++fileBlockNum;
    FAT[ID] = true;
  } // for i

  disk[ID].nextBlockID = 1;  // indicates end of file

} // createFragmentedFile()

void findBigBlock(const vector <bool> &FAT, int size, int capacity,
  int &bestCount, int &bestID)
{
  int ID, count, oldID;

  bestCount = -1;

  for(int i = 0; i < 10; i++)
  {
    do
    {
      ID = (abs(rand() * rand()) % (capacity - 2)) + 2; // 1 < ID < capacity
    } while(FAT[ID]);

    oldID = ID;

    for(count = 0; ID < capacity && !FAT[ID] && count < size; ID++, count++);

    if(bestCount < count)
    {
      bestID = oldID;
      bestCount = count;
    }

    if(count == size)
      break; // found large enough set of blocks to hold the whole file.
  }
} // findBigBlock()

void  createUnFragmentedFile(set<DirectoryEntry> &directory, DiskBlock *disk,
  vector <bool> &FAT, unsigned short name, int size, int capacity)
{
  int ID, oldID, bestCount, count, bestID, fileBlockNum = 0;

  findBigBlock(FAT, size, capacity, bestCount, bestID);
  directory.insert(DirectoryEntry(name, bestID, size));

  oldID = -1;
  for(count = 0, ID = bestID ; ID < capacity && !FAT[ID] && count < size;
    ID++, count++)
  {
    if(oldID > -1) //first time through oldID is invalid.
      disk[oldID].nextBlockID = ID;
    FAT[ID] = true;
    disk[ID].name = name;
    disk[ID].fileBlockNum = ++fileBlockNum;
    disk[ID].nextBlockID = 1;  // indicates end of file
    oldID = ID;
  }
  size -= bestCount;

  while(size > 0)
  {
    findBigBlock(FAT, size, capacity, bestCount, bestID);

    for(count = 0, ID = bestID ; ID < capacity && !FAT[ID] && count < size;
      ID++, count++)
    {
      FAT[ID] = true;
      disk[oldID].nextBlockID = ID;
      disk[ID].name = name;
      disk[ID].fileBlockNum = ++fileBlockNum;
      disk[ID].nextBlockID = 1;  // indicates end of file
      oldID = ID;
    }
    size -= bestCount;
  }
} // creatUnFragmentedFile()

void getValues(int &capacity, int &files, int &percentage, int &fragmentation,
  int &seed)
{
  cout << "Capacity with first two reserved (in disk blocks): ";
  cin >> capacity;
  cout << "Number of files: ";
  cin >> files;
  cout << "Percentage used by files: ";
  cin >> percentage;

  cout << "Percentage fragmented: ";
  cin >> fragmentation;
  cout << "Seed: ";
  cin >> seed;
 } // getValues()

void writeFile(const char filename[], const vector<bool> &FAT,
  const set <DirectoryEntry> &directory, const DiskBlock *disk, int capacity,
  int files)
{
  ofstream outf(filename);
  outf << capacity << " " << files << endl;
  for(set <DirectoryEntry>::const_iterator itr = directory.begin();
    itr != directory.end(); itr++)
      itr->print(outf);

  for(int i = 0; i < capacity; i++)
    outf << i << ' ' << disk[i].name << ' ' << disk[i].fileBlockNum << ' '
      << disk[i].nextBlockID << endl;
 /*
  for(int i = 0; i < capacity; i++)
    if(FAT[i])
      outf << 1 << " ";
    else
      outf << 0 << " ";

  outf << endl;
 */
} // writeFile()

int main()
{
  int files, capacity, seed, size, unUsed, fragmentation, percentage;
  DiskBlock *disk;
  unsigned short name;
  char filename[80];
  set <DirectoryEntry> directory;

  getValues(capacity, files, percentage, fragmentation, seed);
  srand(seed);
  sprintf(filename,"Disk%d_%d_%d_%d_%d.txt", capacity, files, percentage,
    fragmentation, seed);

  vector <bool> FAT(capacity, false);
  disk = new DiskBlock[capacity];

  unUsed = (int) (capacity * percentage / 100.0);
  for(int i = 0; i < files; i++)
  {
    size = (abs(rand() * rand()) % (2 * (unUsed) / (files - i))) + 1;
    if(size > unUsed)
      size = unUsed;
    unUsed -= size;
    do
    {  // find a unique file name
      name = abs(rand() * rand()) % USHRT_MAX;
    } while(directory.find(DirectoryEntry(name, 0, size)) != directory.end());

    if(rand() % 100 < fragmentation)
      createFragmentedFile(directory, disk, FAT, name, size, capacity);
    else
      createUnFragmentedFile(directory, disk, FAT, name, size, capacity);
  }

  writeFile(filename, FAT, directory, disk, capacity, files);
  delete [] disk;
  return 0;
}

