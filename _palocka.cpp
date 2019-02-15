//{===========================================================================
//! @brief      жесткие связи
//!
//!
//! @date       2015
//! @author     Кристев Алексей, 8 класс
//          (C) Кристев Алексей, 8 класс, 2015
//}===========================================================================

#include "TXLib.h"
#include "vec.h"


const int BallsSize = 7;
const int StickRadius = 40;

const int NumberOfSticks = 100;

const int sizeX = GetSystemMetrics(SM_CXSCREEN),
          sizeY = GetSystemMetrics(SM_CYSCREEN);


struct stick
    {
    vec t[2];
    vec v[2];

    vec c, forward_speed;

    double angle, rotation_speed;


    stick() noexcept :
        c (rand()%(sizeX - 20*StickRadius) + 10*StickRadius, rand()%(sizeY - 20*StickRadius) + 10*StickRadius),
        angle (0),
        forward_speed (3.1, -1.9),
        rotation_speed (0.02)
        {
        t[0] = vec(c.x + sin(angle)*StickRadius, c.y + cos(angle)*StickRadius);
        t[1] = vec(c.x - sin(angle)*StickRadius, c.y - cos(angle)*StickRadius);

        forward_speed^= (rand()% 100 + 2);
        }

    void draw() const
        {
        int x = (GetAsyncKeyState(VK_SPACE))? 255:0;

        txSetFillColor (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x));
        txSetColor     (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x), 2);

        txLine   (t[0].x, t[0].y, t[1].x, t[1].y);
        txCircle (t[0].x, t[0].y, BallsSize);
        txCircle (t[1].x, t[1].y, BallsSize);
        }


    void calculate_physics()
        {
        c+= forward_speed;
        angle+= rotation_speed;

        t[0].x = c.x + sin(angle)*StickRadius;
        t[0].y = c.y + cos(angle)*StickRadius;
        t[1].x = c.x - sin(angle)*StickRadius;
        t[1].y = c.y - cos(angle)*StickRadius;


        v[0].x = forward_speed.x + rotation_speed*StickRadius*sin(angle + M_PI_2);                                                                          // |
        v[0].y = forward_speed.y + rotation_speed*StickRadius*cos(angle + M_PI_2);                                                                         // |
        v[1].x = forward_speed.x - rotation_speed*StickRadius*sin(angle + M_PI_2);                                                                           // |
        v[1].y = forward_speed.y - rotation_speed*StickRadius*cos(angle + M_PI_2);

        repulsion();
        }

    void repulsion()
        {
        repulsion_x();
        repulsion_y();
        }

    void repulsion_x()
        {
        for (int i = 0; i < 2; i++)  //running through both point of stick
            if (t[i].x < BallsSize || t[i].x > sizeX - BallsSize)  //checking for collision
                {
                v[i].x = -v[i].x;

                auto _v = v[1 - i].get_rotated(angle);


                _v.y = -2*(_v.x*sin(angle + M_PI) - v[i].y - v[1 - i].y)*cos(angle)/(1 + cos(angle)*cos(angle)) - _v.y;

                v[i].y = v[i].y + v[1 - i].y - _v.x*sin(angle + txPI) - _v.y*cos(angle);

                v[1 - i] = _v.get_rotated(-angle);

                convert_back();
                }
        }

    void repulsion_y()
        {
        for (int i = 0; i < 2; i++)  //running through both point of stick
            if (t[i].y < BallsSize || t[i].y > sizeY - BallsSize)  //checking for collision
                {
                v[i].y = -v[i].y;

                auto _v = v[1 - i].get_rotated(angle);


                _v.y = -2*(_v.x*cos(angle) - v[i].x - v[1 - i].x)*sin(angle)/(1 + sin(angle)*sin(angle)) - _v.y;

                v[i].x = v[i].x + v[1 - i].x - _v.x*cos(angle) - _v.y*sin(angle);

                v[1 - i] = _v.get_rotated(-angle);
                convert_back();
                }
        }

    void convert_back()
        {
        forward_speed = (v[0] + v[1])/2;

        vec left_to_rotate[2] = {v[0] - forward_speed,
                                 v[1] - forward_speed};


        double course = ((left_to_rotate[0].y*(t[0].x - t[1].x) - left_to_rotate[0].x*(t[0].y - t[1].y)) > 0)? -1:1;

        rotation_speed = course*sqrt((sqr(v[0].x) +
                                      sqr(v[1].x) +
                                      sqr(v[0].y) +
                                      sqr(v[1].y) -
                                      2*sqr(forward_speed.x) -
                                      2*sqr(forward_speed.y))/(2*sqr(StickRadius)));

        for (int i = 0; i < 2; i++)
            {
            if (t[i].x <         BallsSize) c.x-= t[i].x -         BallsSize;
            if (t[i].y <         BallsSize) c.y-= t[i].y -         BallsSize;
            if (t[i].x > sizeX - BallsSize) c.x-= t[i].x - sizeX + BallsSize;
            if (t[i].y > sizeY - BallsSize) c.y-= t[i].y - sizeY + BallsSize;
            }
        }
    };



int main()
    {
    txTextCursor(false);

    _txWindowStyle &= ~ WS_CAPTION;

    txCreateWindow(sizeX, sizeY);

    stick Sticks[NumberOfSticks];


    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        txBegin();
        txSetFillColor(TX_BLACK);
        txSetColor    (TX_BLACK);
        txClear();

        for (int i = 0; i < NumberOfSticks; i++)
            {
            Sticks[i].calculate_physics();

            Sticks[i].draw();
            }

        txEnd();
        }

    return 0;
    }
