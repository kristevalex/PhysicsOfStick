//{===========================================================================
//! @file       Examples\Demo\jestkie_svizi.cpp
//!
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

const int sizeX = GetSystemMetrics (SM_CXSCREEN),
          sizeY = GetSystemMetrics (SM_CYSCREEN);


struct palochka
    {
    vec t[2];
    vec v[2];

    vec c, postup;

    double angle;

    double vrach;

    palochka () noexcept :
        c   (rand()%(sizeX - 20*R) + 10*R, rand()%(sizeY - 20*R) + 10*R),
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
        int x = (GetAsyncKeyState (VK_SPACE))? 255:0;

        txSetFillColor (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x));
        txSetColor     (RGB((int)(255 - (c.y/sizeY*200)), (int)(c.x/sizeX*200 + 55), x), 2);

        txLine   (t[0].x, t[0].y, t[1].x, t[1].y);
        txCircle (t[0].x, t[0].y, r);
        txCircle (t[1].x, t[1].y, r);
        }
    };


void ottx (palochka* Main);
void otty (palochka* Main);


void dovorot (palochka* Main);
void ottalkivanie (palochka* Main);


void all_fizics (palochka* Main);


int main()
    {
    txTextCursor (false);

    _txWindowStyle &= ~ WS_CAPTION;                //= WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;

    txCreateWindow (sizeX, sizeY);

    palochka Main[ColVo];


    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        txBegin ();
        txSetFillColor (TX_BLACK);
        txSetColor     (TX_BLACK);
        txClear ();

        for (int i = 0; i < ColVo; i++)
            {
            all_fizics (&(Main[i]));

            Main[i].draw();
            }

        txEnd   ();
        }

    return 0;
    }


 void all_fizics (palochka* Main)               //осноная программа
    {
    // всё остальное   (движение)

    Main->c    += Main->postup;
    Main->angle  += Main->vrach;

    Main->t[0].x = Main->c.x + sin(Main->angle)*R;
    Main->t[0].y = Main->c.y + cos(Main->angle)*R;
    Main->t[1].x = Main->c.x - sin(Main->angle)*R;
    Main->t[1].y = Main->c.y - cos(Main->angle)*R;


    //отталкивание от стенок

    Main->v[0].x = Main->postup.x + Main->vrach*R*sin(Main->angle + M_PI_2);                                                                          // |
    Main->v[0].y = Main->postup.y + Main->vrach*R*cos(Main->angle + M_PI_2);                                                                         // |
    Main->v[1].x = Main->postup.x - Main->vrach*R*sin(Main->angle + M_PI_2);                                                                           // |
    Main->v[1].y = Main->postup.y - Main->vrach*R*cos(Main->angle + M_PI_2);

    ottalkivanie (Main);
    }

void ottalkivanie (palochka* Main)
    {
    ottx (Main);
    otty (Main);
    }

void ottx (palochka* Main)
    {
    for (int i = 0; i < 2; i++)
        if (Main->t[i].x < r || Main->t[i].x > sizeX - r)
            {
            Main->v[i].x = -Main->v[i].x;

            auto _v = Main->v[1 - i].get_rotated(Main->angle);


            _v.y = -2*(_v.x*sin(Main->angle + txPI) - Main->v[i].y - Main->v[1 - i].y)*cos(Main->angle)/(1 + cos(Main->angle)*cos(Main->angle)) - _v.y;

            Main->v[i].y = Main->v[i].y + Main->v[1 - i].y - _v.x*sin(Main->angle + txPI) - _v.y*cos(Main->angle);

            Main->v[1 - i] = _v.get_rotated(-Main->angle);

            dovorot(Main);
            }

    }

void otty (palochka* Main)
    {
    for (int i = 0; i < 2; i++)

        if (Main->t[i].y < r || Main->t[i].y > sizeY - r)
            {
            Main->v[i].y = -Main->v[i].y;

            auto _v = Main->v[1 - i].get_rotated(Main->angle);


            _v.y = -2*(_v.x*cos(Main->angle) - Main->v[i].x - Main->v[1 - i].x)*sin(Main->angle)/(1 + sin(Main->angle)*sin(Main->angle)) - _v.y;

            Main->v[i].x = Main->v[i].x + Main->v[1 - i].x - _v.x*cos(Main->angle) - _v.y*sin(Main->angle);

            Main->v[1 - i] = _v.get_rotated(-Main->angle);

            dovorot(Main);
            }
    }

void dovorot (palochka* Main)
    {
    auto new_forward_speed = (Main->v[0]+Main->v[1])/2;

    Main->postup = new_forward_speed;

    vec v_to_rotate[2] = {Main->v[0] - Main->postup,
                          Main->v[1] - Main->postup};


    double znak = 1;
    if ((v_to_rotate[0].y*(Main->t[0].x - Main->t[1].x) - v_to_rotate[0].x*(Main->t[0].y - Main->t[1].y)) > 0) znak = -1;

    Main->vrach = znak*sqrt((Main->v[0].x*Main->v[0].x +
                             Main->v[1].x*Main->v[1].x +
                             Main->v[0].y*Main->v[0].y +
                             Main->v[1].y*Main->v[1].y -
                             2*new_forward_speed.x*new_forward_speed.x -
                             2*new_forward_speed.y*new_forward_speed.y)/(2*R*R));

    for (int i = 0; i < 2; i++)
        {
        if (Main->t[i].x <         r) Main->c.x-= Main->t[i].x -         r;
        if (Main->t[i].y <         r) Main->c.y-= Main->t[i].y -         r;
        if (Main->t[i].x > sizeX - r) Main->c.x-= Main->t[i].x - sizeX + r;
        if (Main->t[i].y > sizeY - r) Main->c.y-= Main->t[i].y - sizeY + r;
        }
    }
