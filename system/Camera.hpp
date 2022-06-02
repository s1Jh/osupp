#pragma once

#include "Vec2.hpp"
#include "Matrix.hpp"
#include "define.hpp"
#include "Vec3.hpp"

namespace GAME_TITLE {

    class Camera2D {
    public:
        Camera2D();

        enum class Direction {
            Up, Down, Left, Right, Out, In
        };

        void setPosition(const fvec2d &in);

        void translate(const fvec2d &amount);

        void setScale(const fvec2d &in);

        void scale(const fvec2d &amount);

        void setRotation(float in);

        void rotate(float amount);

        void setAspectRatio(float ratio);

        [[nodiscard]] const Mat3f &getMatrix() const;

        void recalculateMatrix();

    private:
        float aspectRatio;
        fvec2d position;
        float rotation;
        fvec2d viewScale;
        Mat3f cameraMatrix;
    };
}