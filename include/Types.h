//
// Created by mikhail on 5/20/21.
//

#ifndef BOXING2D_2_TYPES_H
#define BOXING2D_2_TYPES_H
#include <vector>
#include <SDL2/SDL.h>

namespace type{
    struct Vector2i {
        int x, y;

        Vector2i(int p_x, int p_y);

        Vector2i operator+(const Vector2i &p_change) const;
        Vector2i operator-(const Vector2i &p_change) const;

        void operator+=(const Vector2i &p_change);

        Vector2i operator/(const Vector2i &p_change) const;
        Vector2i operator*(const Vector2i &p_change) const;

    };

    struct Vector2d {
        double x, y;

        Vector2d(double p_x, double p_y);

        Vector2d operator+(const Vector2d &p_change) const;

        void operator+=(const Vector2d &p_change);

        Vector2d operator/(const Vector2d &p_change) const;
        Vector2d operator*(const Vector2d &p_change) const;

    };

    class Sprite {
        Vector2i m_position, m_size;
        int m_counter{};
        std::vector<std::vector<SDL_Texture *>> textures;
    public:
        //Setter
        void setPosition(Vector2i p_newPos);
        void setSize(Vector2i p_newSize);

        //Changer
        void changePosition(Vector2i p_change);

        //Getter
        SDL_Rect getPosition() const;

        //Functions
        int updateTexture();

        //Constructor
        Sprite(const std::string &p_baseSpriteAssetDir, type::Vector2i p_position, type::Vector2i p_size);
    };
}


#endif //BOXING2D_2_TYPES_H
