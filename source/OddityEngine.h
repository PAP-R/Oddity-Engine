#ifndef ODDITYENGINE_H
#define ODDITYENGINE_H



namespace OddityEngine {
    /**
     * Initializes everything used by the engine
     */
    void init();

    /**
     * Terminates everything used by the engine
     */
    void terminate();

    /**
     * Updates everything there is to update
     * @return
     */
    bool update();
};



#endif //ODDITYENGINE_H
