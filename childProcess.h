
void initializeLapRelativeData(sharedMemory *sharedMemory, int childId);

void generateSectorsTimes(sharedMemory *sharedMemory, int childId);

void calculateLapTime(sharedMemory *sharedMemory, int childId);

void calculateTotalTime(sharedMemory *sharedMemory, int childId);

void generatePitStops(sharedMemory *sharedMemory, int childId);

void generateOut(sharedMemory *sharedMemory, int childId, int pId);

void findBestLap(sharedMemory *sharedMemory, int childId);
