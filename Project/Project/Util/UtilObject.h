#ifndef __UTILOBJECT_H__
#define __UTILOBJECT_H__

//#include <>
#include <math.h>
#include <wx/wx.h>
//#include "wx/listctrl.h"

namespace gf
{
// unsigned char point
class ImageU1
{
public:
    int             m_width;      // image width
    int             m_height;     // image height
    unsigned char   *m_data;      // image data

public:
    ImageU1(int width = 0, int height = 0);
    ImageU1(int width, int height, unsigned char *data);
    ImageU1(const ImageU1 &img);
    virtual ~ImageU1();

    static ImageU1  CreateFromwxImage(const wxImage &img);
    bool            Create(int w, int h, bool clear = true);
    wxImage         ChangeTowxImage() const;
    ImageU1         GetSubImage(wxRect rect);
    void            CopyFrom(const ImageU1 &img);
    bool            CopyFromWxImage(const wxImage &img);
    bool            IsEmpty() const;
};

// limit a double with a min value and a max value
class RangeD
{
public:
    double min;     // min value
    double max;     // max value

public:
    RangeD(double v_min, double v_max);
    RangeD();

    //// if inner this limit range
    bool IsInRange(double v_value);
};


// double point
class Point2d
{
    public:
        double x;
        double y;

    public:
        Point2d();
        Point2d(double v_x, double v_y);
        Point2d operator/(const double& v_r);
        Point2d operator+(const Point2d &pt);
        Point2d operator-(const Point2d &pt);
        Point2d &operator+=(const Point2d &pt);
        Point2d &operator-=(const Point2d &pt);
        bool operator==(const Point2d &pt)const;
//        std::string ToString() const;
};

// Point2d pt_array[100];
class PtList
{
    private:
        Point2d pt_array[100];
        int head_index;
        int end_index;

    public:
        PtList(){
            head_index = 0;
            end_index = 0;
        };
        void push_back(Point2d pt){
            if(end_index + 1 == head_index){
                //cout << "PtList>> full" << endl;
                return;
            }
            pt_array[end_index] = pt;
            end_index = (end_index + 1) % 500;
        };
        Point2d pop_front(){
            if(end_index == head_index){
                //cout << "PtList>> empty" << endl;
                return Point2d(0, 0);
            }

            int old_head = head_index;
            head_index = (head_index + 1) % 500;
            return pt_array[old_head];
        };
        int size(){
            if(end_index < head_index)
                return end_index - head_index + 500;
            return end_index - head_index;
        };
        void clear(){
            head_index = 0;
            end_index = 0;
        }
};


// double rect
class RectD
{
    public:
        double x;
        double y;
        double width;
        double height;

    public:
        RectD();

        RectD(Point2d tl_pos, double v_width, double v_height);
        RectD(double v_x, double v_y, double v_width, double v_height);
        virtual ~RectD();

        // check if point(v_x, v_y) in rect
        bool    Contains(double v_x, double v_y);
        // check if point pt in rect
        bool    Contains(Point2d pt);
        // distance of point to rect
        double  Distance2Position(Point2d pt);
        // set top_left position
        void    SetPosition(Point2d pt);
        // offset rect
        void    Offset(double dx, double dy);
//        void InFlate(double dw, double dh);
        double  GetX() const;
        void    SetX(double val);
        double  GetY() const;
        void    SetY(double val);
        double  GetWidth() const;
        void    SetWidth(double val);
        double  GetHeight() const;
        void    SetHeight(double val);
};


class RotatedRect
{
public:
    Point2d center;
    double  width;
    double  height;
    double  angle;

public:
    RotatedRect();
    RotatedRect(Point2d v_center, double v_width, double v_height, double v_angle);
    virtual ~RotatedRect();

    // check if point(v_x, v_y) in rotated rect
    bool    Contains(double v_x, double v_y);
    // check if point(pt) in rotated rect
    bool    Contains(Point2d pt);
    // offset rotated rect
    void    Offset(double dx, double dy);
    // return bound rect of rotated rect
    RectD   GetBoundRect();
    // return vertex of rotated rect
    void    Points(Point2d pts[]);

    Point2d GetCenter() const;
    void    SetCenter(Point2d pt);
    double  GetWidth() const;
    void    SetWidth(double val);
    double  GetHeight() const;
    void    SetHeight(double val);
    double  GetAngle() const;
    void    SetAngle(double ang);
};


//************************************
// Returns:     Point2d point after rotate
// pt:          point to rotate
// v_ang:       rotate angle in arg
// pt_center:   Point2d rotate center
//************************************
Point2d RotatePt(Point2d pt, double v_ang, Point2d pt_center = Point2d(0, 0));

//************************************
// Returns:     distance of pt1 & pt2
// pt1:         first point
// pt2:         second point
//************************************
double Distance(Point2d pt1, Point2d pt2);

//************************************
// change wximage from u1 to u3
//************************************
void wxImageU1ToU3(wxImage &img);

//************************************
// change wximage from u3 to u1
//************************************
void wxImageU3ToU1(wxImage &img);

//************************************
// return the error code of GetLastError()
// inet = true to return error code from wininet
//************************************
std::string GetLastErrorMsg(bool inet = false);

};

#endif //__UTILOBJECT_H__