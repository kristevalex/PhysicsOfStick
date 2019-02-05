

double atan_ (double x, double y)
    {
    if (x == 0) x = 0.00001;
    double returned = atan(y/x);
    if (x < 0) returned += M_PI;
    return returned;
    }

class vec
    {
    public:
    double x, y;
    double k, l;

    vec ()
        {
        x = 0;
        y = 0;
        k = 0;
        l = 0;
        };

    vec (double x1, double y1)
        {
        x = x1;
        y = y1;
        l = sqrt(x*x + y*y);
        k = atan_(x, y);
        };


    void print ()
        {
        printf ("{%lf, %lf}:{%lf, %lf}:{%lf, %lf}\n", x, y, k, l, (l)*cos(k), (l)*sin(k));
        }

    void setXY ()
        {
        x = l*cos(k);
        y = l*sin(k);
        }

    void setLK ()
        {
        l = sqrt(x*x + y*y);
        k = atan_(x, y);
        }

    vec operator=  (vec vec1)
        {
        x = vec1.x;
        y = vec1.y;
        l = vec1.l;
        k = vec1.k;
        return *this;
        }

    vec operator= (POINT xy)
        {
        x = xy.x;
        y = xy.y;
        l = sqrt(x*x + y*y);
        k = atan_(x, y);
        return *this;
        }

    vec operator+ (vec vec1) const
        {
        vec returned;
        returned.x = x + vec1.x;
        returned.y = y + vec1.y;
        returned.l = sqrt(returned.x*returned.x + returned.y*returned.y);
        returned.k = atan_(returned.x, returned.y);
        return returned;
        }

    vec operator+= (vec vec1)
        {
        vec returned;
        x+= vec1.x;
        y+= vec1.y;
        l = sqrt(x*x + y*y);
        k = atan_(x, y);

        }

    vec operator- (vec vec1) const
        {
        vec returned;
        returned.x = x - vec1.x;
        returned.y = y - vec1.y;
        returned.l = sqrt(returned.x*returned.x + returned.y*returned.y);
        returned.k = atan_(returned.x, returned.y);
        return returned;
        }

    vec operator-= (vec vec1)
        {
        vec returned;
        x = x - vec1.x;
        y = y - vec1.y;
        l = sqrt(x*x + y*y);
        k = atan_(x, y);
        return *this;
        }

    vec operator-= (double l1)
        {
        l-= l1;
        x = l*cos(k);
        y = l*sin(k);
        return *this;
        }

    vec operator+= (double l1)
        {
        if (l >= 0) l+= l1;
        else l-= l1;
        x = l*cos(k);
        y = l*sin(k);
        return *this;
        }

    vec operator/ (double x1) const
        {
        vec returned;
        returned.l = l/x1;
        returned.k = k;
        returned.x = returned.l*cos(returned.k);
        returned.y = returned.l*sin(returned.k);
        return returned;
        }

    vec operator/= (double x1)
        {
        l/= x1;
        x = l*cos(k);
        y = l*sin(k);
        return *this;
        }

    vec operator* (double x1) const
        {
        vec returned;
        returned.l = l*x1;
        returned.k = k;
        returned.x = returned.l*cos(returned.k);
        returned.y = returned.l*sin(returned.k);
        return returned;
        }

    vec operator*= (double x1)
        {
        l*= x1;
        x = l*cos(k);
        y = l*sin(k);
        return *this;
        }

    vec operator^ (double x1) const
        {
        vec returned;
        returned.l = l;
        returned.k = k+x1;
        returned.x = returned.l*cos(returned.k);
        returned.y = returned.l*sin(returned.k);
        return returned;
        }

    vec operator^= (double x1)
        {
        k+= x1;
        x = l*cos(k);
        y = l*sin(k);
        return *this;
        }

    vec turn(double alf)
        {
        vec ret;
        ret.x = x*cos(alf) - y*sin(alf);
        ret.y = x*sin(alf) + y*cos(alf);

        return ret;
        }
    };



vec xy_vec (double x, double y)
    {
    vec returned;
    returned.x = x;
    returned.y = y;
    returned.l = sqrt(returned.x*returned.x + returned.y*returned.y);
    returned.k = atan_(returned.x, returned.y);
    return returned;
    }

vec lk_vec (double l, double k)
    {
    vec returned;
    returned.l = l;
    returned.k = k;
    returned.x = returned.l*cos(returned.k);
    returned.y = returned.l*sin(returned.k);
    return returned;
    }

void color (COLORREF color)
    {
    txSetFillColor (color);
    txSetColor     (color);
    }
