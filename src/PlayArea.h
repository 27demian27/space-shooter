#ifndef PLAYAREA
#define PLAYAREA

#include "Rect.h"

class PlayArea {

public:
    PlayArea(int width, int height);

    bool contains(float x, float y);

private:
    Rect bounds;

};

#endif //PLAYAREA