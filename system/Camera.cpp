#include "Camera.hpp"

#include "Math.hpp"

NS_BEGIN


Camera2D::Camera2D() :
        aspectRatio(1.f),
        position({0.f, 0.f}),
        rotation(0.f),
        viewScale({1.f, 1.f}) {
    recalculateMatrix();
}

void Camera2D::recalculateMatrix() {
    cameraMatrix =
            MakeTranslationMatrix(position) *
            MakeRotationMatrix<float>(rotation) *
            MakeScaleMatrix(viewScale / fvec2d{aspectRatio, 1.f});
}

const Mat3f &Camera2D::getMatrix() const {
    return cameraMatrix;
}

void Camera2D::setAspectRatio(float ratio) {
    aspectRatio = ratio;
}

void Camera2D::rotate(float amount) {
    rotation += amount;
}

void Camera2D::setRotation(float in) {
    rotation = in;
}

void Camera2D::scale(const fvec2d &amount) {
    viewScale += amount;
}

void Camera2D::setScale(const fvec2d &in) {
    viewScale = in;
}

void Camera2D::translate(const fvec2d &amount) {
    position += Rotate(amount, rotation);
}

void Camera2D::setPosition(const fvec2d &in) {
    position = in;
}

NS_END