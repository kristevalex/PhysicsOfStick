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


const double r = 7;
const int R = 40;

const int ColVo = 100;

const int sizeX = GetSystemMetrics(SM_CXSCREEN),
          sizeY = GetSystemMetrics(SM_CYSCREEN);


struct palochka
    {
    vec t[2];
    vec v[2];

    vec c, postup;

    double angle, vrach;


    palochka() noexcept :
        c (rand()%(sizeX - 20*R) + 10*R, rand()%(sizeY - 20*R) + 10*R),
        angle (0),
        postup (3.1, -1.9),
        vrach (0.02)
        {
        t[0] = vec(c.x + sin(angle)*R, c.y + cos(angle)*R);
        t[1] = vec(c.x - sin(angle)*R, c.y - cos(angle)*R);

        postup^= (rand()% 100 + 2);
        }

    void draw() const
        {
        int x = (GetAsyncKeyState(VK_SPACE))? 255:0;

        txSetFillColor (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x));
        txSetColor     (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x), 2);

        txLine   (t[0].x, t[0].y, t[1].x, t[1].y);
        txCircle (t[0].x, t[0].y, r);
        txCircle (t[1].x, t[1].y, r);
        }


    void calculate_physics()
        {
        c+= postup;
        angle+= vrach;

        t[0].x = c.x + sin(angle)*R;
        t[0].y = c.y + cos(angle)*R;
        t[1].x = c.x - sin(angle)*R;
        t[1].y = c.y - cos(angle)*R;


        v[0].x = postup.x + vrach*R*sin(angle + M_PI_2);                                                                          // |
        v[0].y = postup.y + vrach*R*cos(angle + M_PI_2);                                                                         // |
        v[1].x = postup.x - vrach*R*sin(angle + M_PI_2);                                                                           // |
        v[1].y = postup.y - vrach*R*cos(angle + M_PI_2);

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
            if (t[i].x < r || t[i].x > sizeX - r)  //checking for collision
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
            if (t[i].y < r || t[i].y > sizeY - r)  //checking for collision
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
        postup = (v[0] + v[1])/2;

        vec v_to_rotate[2] = {v[0] - postup,
                              v[1] - postup};


        double znak = 1;
        if ((v_to_rotate[0].y*(t[0].x - t[1].x) - v_to_rotate[0].x*(t[0].y - t[1].y)) > 0) znak = -1;

        vrach = znak*sqrt((sqr(v[0].x) +
                           sqr(v[1].x) +
                           sqr(v[0].y) +
                           sqr(v[1].y) -
                           2*sqr(postup.x) -
                           2*sqr(postup.y))/(2*sqr(R)));

        for (int i = 0; i < 2; i++)
            {
            if (t[i].x <         r) c.x-= t[i].x -         r;
            if (t[i].y <         r) c.y-= t[i].y -         r;
            if (t[i].x > sizeX - r) c.x-= t[i].x - sizeX + r;
            if (t[i].y > sizeY - r) c.y-= t[i].y - sizeY + r;
            }
        }
    };



int main()
    {
    txTextCursor(false);

    _txWindowStyle &= ~ WS_CAPTION;

    txCreateWindow(sizeX, sizeY);

    palochka Main[ColVo];


    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        txBegin();
        txSetFillColor(TX_BLACK);
        txSetColor    (TX_BLACK);
        txClear();

        for (int i = 0; i < ColVo; i++)
            {
            Main[i].calculate_physics();

            Main[i].draw();
            }

        txEnd();
        }

    return 0;
    }
