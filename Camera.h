//
// Created by User on 09/07/2026.
//

#ifndef BERESHITCORE_CAMERA_H
#define BERESHITCORE_CAMERA_H

#include <string>

class Camera {
    public:
        int POV = 120;
        int width=1920;
        int hight=1920;
        double VIEWER_DISTANCE=0;
        std::string shading = "wire";
        Camera();
};


#endif //BERESHITCORE_CAMERA_H
