//
// Created by yaly on 23/07/2026.
//

#ifndef BERESHITCORE_VECTOR2_H
#define BERESHITCORE_VECTOR2_H


class Vector2 {
    public:
        double x;
        double y;

        Vector2(double x = 0.0, double y = 0.0);

        [[nodiscard]] Vector2 operator+(const Vector2& other) const;
        [[nodiscard]] Vector2 operator-(const Vector2& other) const;



        [[nodiscard]] Vector2 operator-() const;


        [[nodiscard]] double dot(const Vector2& other) const;


};


#endif //BERESHITCORE_VECTOR2_H
