#include "cubiomes/finders.c"
#include "cubiomes/javarnd.h"
#include "math.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>

#define SEED -6463883208784934046 //6880093502557252720 //-6463883208784934046
#define DISTANCE 32
#define SPREAD 3
#define COUNT 128 //34 // Minecraft generates 128 strongholds spread into 8 rings around 0,0. We only care about the 4 first rings = 34 strongholds.
#define DOUBLE_UNIT 1.0 / (1L << 53)
#define PI 3.141592653589793
// 12 chunks. Limit would be 16, but as we can't calculate the exact position fo the portal room (too expensive), we remove 4 chunks to be sure.
#define CHUNKS_FROM_PORTAL 16 //192

struct StructureCount
{
    int outpostCount;
};

void getStrongholds(int64_t *seed, LayerStack *g);
void checkStructure(int64_t seed, Pos strongholdPos, StructureConfig config, LayerStack g, struct StructureCount *count);
int configToStructureName(StructureConfig config);
Pos getPyramidPos(int64_t seed, int x, int z);