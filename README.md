# Disk Defragmenter Simulator

Disk defragmenter simulator re-organizes files on a simulated disk. The defragmenter minimizes the number of disk accesses and CPU
time in order to achieve this. To simulate a disk, the program does not use more than eight million bytes of dynamic memory at any time.

### Description
The program rearranges the disk blocks so that: 

1) the first file starts on disk block #2, 

2) all of the blocks of each file are in sequence and contiguous, and 

3) all unused blocks are located at the end of the disk.

The `DiskDrive` class provides simulated disk block access.\
A disk is simulated with three data structures: 

1) a public array of bools, File Allocation Table (`FAT`), with each bool indicating whether the corresponding disk block is used (true) or unused (false); 

2) a public array of `DirectoryEntry`’s sorted by file names, that provide names, starting blockIDs, and sizes of the files; and 

3) a private array of `DiskBlock`’s, that contains the critical information for each block. `Defragmenter` only has access to the `nextBlockID` of a `DiskBlock`. The `filename` and `fileBlockNum` are used to check to see if you preserved the ordering of the blocks in the file.

### Disk Files

The name of a data file reflects parameters used to create it.
   * For example, `Disk20_3_80_50_892.txt` has 20 disk blocks, 
3 files, is ~80% full, ~50% of the files are fragmented, and was created using a seed of 892 for the random number generator.

The first line of the file is the capacity and number of files.

This first line is followed by a list of the directory for the disk, one file per line.

   * Each line starts with the name of a file `<int>`, then first disk block used by the file `<unsigned>`, and then its size `<int>`.

After the directory listing is a list of the contents of each of the disk blocks in the disk.
   * The first two blocks, 0 and 1, are reserved.
   *  The disk block information is arranged as follows: DiskBlockID (unsigned), File name (unsigned short),
file block number (unsigned short), NextBlockID of file (unsigned). A next of 0 indicates an empty block, a next of 1 indicates the last block in a file.

You can create your own disk file by compiling and running `CreateDisk.cpp`.

### How to Run
1. `make`
2. `./defrag.out <Disk File>`
