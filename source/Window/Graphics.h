#ifndef GRAPHICS_H
#define GRAPHICS_H



namespace OddityEngine::Graphics {
    /**
     * Initilizes everything needed for Graphics
     */
    void init();

    /**
     * Terminates everything needed for Graphics
     */
    void terminate();

    /**
     * Updates all Graphical elements like the active windows
     * @return true if there are things still needing updating
     */
    bool update();
};



#endif //GRAPHICS_H
