float generateRandomNumber(sharedMemory *sharedMemory, int max, int added);

void initializeCountersTo0(sharedMemory *sharedMemory, int childId);

void generateSectorsTimesP1(sharedMemory *sharedMemory, int childId);

void calculateLapTime(sharedMemory *sharedMemory, int childId);

void calculateTotalTime(sharedMemory *sharedMemory, int childId);

void generatePitStops(sharedMemory *sharedMemory, int childId);

void generateOut(sharedMemory *sharedMemory, int childId, int pId);

void findBestLap(sharedMemory *sharedMemory, int childId);
