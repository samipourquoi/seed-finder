#include "main.h"

// fuck pointers
Pos strongholds[COUNT];

int main()
{
    printf("seed: %ld\n", SEED);

    initBiomes();
    LayerStack g = setupGenerator(MC_1_16);

    clock_t begin = clock();
    // int64_t seed = SEED; // Debug seed
    int found = 0;

    for (int64_t seed = INT64_MIN; !found; seed++)
    {
        applySeed(&g, seed);

        int64_t randomSeed = seed;

        setSeed(&randomSeed);
        getStrongholds(&randomSeed, &g);

        struct StructureCount count = {0};
        // printf("searching %lld\n", seed);

        /*
        Loops through the strongholds.
        It only checks once for each structure, and it doesn't reuse strongholds.
        */
        for (int n = 0; n < COUNT; n++)
        {
            if (count.outpostCount == 0)
            {
                checkStructure(SEED, strongholds[n], OUTPOST_CONFIG, g, &count);
                if (count.outpostCount > 2)
                {
                    printf("Found with seed %lld! [%d, %d]", seed, strongholds[n].x*16, strongholds[n].z*16);
                    found = 1;
                }
                else count.outpostCount = 0;
            }
        }
    }

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("%f", time_spent);

    freeGenerator(g);
    return 0;
}

void getStrongholds(int64_t *seed, LayerStack *g)
{
    double d = nextDouble(seed) * PI * 2.0;
    int perRingCount = 0, m = 0, perRing = SPREAD;
    int* validStrongholdBiomes = getValidStrongholdBiomes();

    for (int n = 0; n < COUNT; n++)
    {
        double e = (4 * DISTANCE + DISTANCE * m * 6) + (nextDouble(seed) - 0.5) * DISTANCE * 2.5;
        Pos chunk = {
            round(cos(d) * e),
            round(sin(d) * e)
        };

        Pos blockPos = findBiomePosition(MC_1_16, *g, NULL, (chunk.x << 4) + 8, (chunk.z << 4) + 8, 112, validStrongholdBiomes, seed, NULL);

        if (blockPos.x != (chunk.x << 4) + 8 && blockPos.z != (chunk.z << 4) + 8)
        {
            chunk.x = blockPos.x >> 4;
            chunk.z = blockPos.z >> 4;
        }

        strongholds[n] = chunk;

        d += PI * 2 / (double) perRing;
        perRingCount++;

        if (perRingCount == perRing)
        {
            m++;
            perRingCount = 0;
            perRing += 2 * perRing / (m + 1);
            perRing = (perRing >= COUNT - n) ? COUNT - n : perRing;
            d += nextDouble(seed) * PI * 2.0;
        }
    }
}

void checkStructure(int64_t seed, Pos strongholdPos, StructureConfig config, LayerStack g, struct StructureCount *count)
{
    Pos originalRegionPos = {
        strongholdPos.x >> 5,
        strongholdPos.z >> 5
    };

    /*
    Index 0: original region
    Index 1: X axis
    Index 2: Z axis
    Index 3: corner
    */
    Pos regionPositions[4];
    regionPositions[0] = originalRegionPos;
    int size = 1;
    int type = configToStructureName(config);

    // X axis
    if (floor((strongholdPos.x + CHUNKS_FROM_PORTAL) / config.regionSize) != originalRegionPos.x)
    {
        regionPositions[1].x = originalRegionPos.x - 1;
        regionPositions[1].z = originalRegionPos.z;
        size++;
    }
    else if (floor((strongholdPos.x - CHUNKS_FROM_PORTAL) / config.regionSize) != originalRegionPos.x)
    {
        regionPositions[1].x = originalRegionPos.x + 1;
        regionPositions[1].z = originalRegionPos.z;
        size++;
    }

    // Z axis
    if (floor((strongholdPos.z + CHUNKS_FROM_PORTAL) / config.regionSize) != originalRegionPos.z)
    {
        regionPositions[2].x = originalRegionPos.x;
        regionPositions[2].z = originalRegionPos.z - 1;
        size++;
    }
    else if (floor((strongholdPos.z - CHUNKS_FROM_PORTAL) / config.regionSize) != originalRegionPos.z)
    {
        regionPositions[2].x = originalRegionPos.x;
        regionPositions[2].z = originalRegionPos.z + 1;
        size++;
    }

    // Corner
    if (size == 3)
    {
        regionPositions[3].x = regionPositions[1].x;
        regionPositions[3].z = regionPositions[2].z;
        size++;
    }

    /* Debug
    printf("[%d, %d], [%d, %d], [%d, %d], [%d, %d], size: %d\n",
        regionPositions[0].x,
        regionPositions[0].z,
        regionPositions[1].x,
        regionPositions[1].z,
        regionPositions[2].x,
        regionPositions[2].z,
        regionPositions[3].x,
        regionPositions[3].z,
        size);
    */

    for (int n = 0; n < size; n++)
    {
        Pos regionPos = regionPositions[n];
        Pos structureChunkRegion = getStructureChunkInRegion(DESERT_PYRAMID_CONFIG, seed, regionPos.x, regionPos.z);
        Pos structurePos = {
            regionPos.x * 32 + structureChunkRegion.x,
            regionPos.z * 32 + structureChunkRegion.z
        };

        int distanceX = abs(structurePos.x - strongholdPos.x);
        int distanceZ = abs(structurePos.z - strongholdPos.z);

        if (0 <= distanceX && distanceX <= CHUNKS_FROM_PORTAL / 2 && 0 <= distanceZ && distanceZ <= CHUNKS_FROM_PORTAL / 2)
        {
            // printf("{%d, %d}\n", structurePos.x * 16, structurePos.z * 16);
            count->outpostCount++;
        }
    }
}

int configToStructureName(StructureConfig config)
{
    switch (config.seed)
    {
        case 165745296:         return Outpost;
        case 14357617:          return Desert_Pyramid;
        default:                return 0;
    }
}

Pos getPyramidPos(int64_t seed, int x, int z)
{
    int regionX = x;
    int regionZ = z;

    int64_t regionSeed = regionX * 341873128712 + regionZ * 132897987541 + seed + 14357617; //165745296;
    setSeed(&regionSeed);

    Pos pos = {
        regionX * 32 + nextInt(&regionSeed, 24),
        regionZ * 32 + nextInt(&regionSeed, 24)
    };

    return pos;
}