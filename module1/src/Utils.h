#ifndef MODULE1_UTILS 
#define MODULE1_UTILS
    namespace Utils{
        enum Sensor{
            SENSOR1 = 3, 
            SENSOR2, 
            SENSOR3, 
            SENSOR4
        };

        static const int NUM_LEVEL = 3;
        static const int NUM_SENSOR = 4;
        const int levels[NUM_LEVEL][NUM_SENSOR] = {{0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
    }

    static int sensors[Utils::NUM_SENSOR] = {0, 0, 1, 0};
#endif