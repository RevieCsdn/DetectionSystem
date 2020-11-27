#include "UtilObject.h"
using namespace gf;

Point2d::Point2d()
{
    x = 0;
    y = 0;
}

Point2d::Point2d( double v_x, double v_y )
{
    x = v_x;
    y = v_y;
}

Point2d Point2d::operator/( const double& v_r )
{
    if(abs(v_r) < 0.001) return Point2d(0,0);
    Point2d rs;
    rs.x = x/v_r;
    rs.y = y/v_r;
    return rs;
}

Point2d Point2d::operator+( const Point2d &pt )
{
    Point2d rs;
    rs.x = x + pt.x;
    rs.y = y + pt.y;
    return rs;
}

Point2d Point2d::operator-( const Point2d &pt )
{
    Point2d rs;
    rs.x = x - pt.x;
    rs.y = y - pt.y;
    return rs;
}

Point2d & Point2d::operator+=( const Point2d &pt )
{
    x += pt.x;
    y += pt.y;
    return *this;
}

Point2d & Point2d::operator-=( const Point2d &pt )
{
    x -= pt.x;
    y -= pt.y;
    return *this;
}

bool Point2d::operator==( const Point2d &pt ) const
{
    return (x == pt.x && y == pt.y);
}

//std::string Point2d::ToString() const
//{
//}


RectD::RectD()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

RectD::RectD( Point2d tl_pos, double v_width, double v_height )
{
    x = tl_pos.x;
    y = tl_pos.y;
    width = v_width;
    height = v_height;
}

RectD::RectD( double v_x, double v_y, double v_width, double v_height )
{
    x = v_x;
    y = v_y;
    width = v_width;
    height = v_height;
}

RectD::~RectD()
{}

bool RectD::Contains( double v_x, double v_y )
{
    v_x -= x;
    v_y -= y;

    if(v_x < 0 ||
        v_x > width ||
        v_y < 0 ||
        v_y > height)
        return false;

    return true;
}

bool RectD::Contains( Point2d pt )
{
    return Contains(pt.x, pt.y);
}

double RectD::Distance2Position( Point2d pt )
{
//    if(Contains(pt))
//        return 0.;
    double rs = 0.;

    if(pt.x < x){
        if(pt.y < y)
            rs = Distance(pt, Point2d(x, y));
        else if(pt.y > y + height)
            rs = Distance(pt, Point2d(x, y + height));
        else
            rs = x - pt.x;

    } else if(pt.x > x + width){
        if(pt.y < y)
            rs = Distance(pt, Point2d(x + width, y));
        else if(pt.y > y + height)
            rs = Distance(pt, Point2d(x + width, y + height));
        else
            rs = pt.x - x - width;

    } else {
        if( pt.y < y )
            rs = y - pt.y;
        else if( pt.y > y + height )
            rs = pt.y - y - height;

    }

    return rs;
}

void RectD::SetPosition( Point2d pt )
{
    x = pt.x;
    y = pt.y;
}

void RectD::Offset( double dx, double dy )
{
    x += dx;
    y += dy;
}

// void RectD::InFlate( double dw, double dh )
// {
//     width += dw;
//     height += dh;
// }

double RectD::GetX() const
{
    return x;
}

void RectD::SetX( double val )
{
    x = val;
}

double RectD::GetY() const
{
    return y;
}

void RectD::SetY( double val )
{
    y = val;
}

double RectD::GetWidth() const
{
    return width;
}

void RectD::SetWidth( double val )
{
    width = val;
}

double RectD::GetHeight() const
{
    return height;
}

void RectD::SetHeight( double val )
{
    height = val;
}

RangeD::RangeD( double v_min, double v_max )
{
    min = v_min;
    max = v_max;
}

RangeD::RangeD()
{
    min = 0;
    max = 0;
}

bool RangeD::IsInRange( double v_value )
{
    return (v_value >= min) && (v_value <= max);
}

RotatedRect::RotatedRect()
{
    center = Point2d(0, 0);
    width = 0;
    height = 0;
    angle = 0;
}

RotatedRect::RotatedRect( Point2d v_center, double v_width, double v_height, double v_angle )
{
    center = v_center;
    width = v_width;
    height = v_height;
    angle = v_angle;
}

RotatedRect::~RotatedRect()
{
}

bool RotatedRect::Contains( double x, double y )
{
    double dx = x - center.x;
    double dy = y - center.y;
    // rotate -angle;
    double cosa = cos(-angle);
    double sina = sin(-angle);
    x = dx * cosa - dy * sina;
    y = dx * sina + dy * cosa;

    if(abs(x * 2) > width || abs(y * 2) > height)
        return false;

    return true;
}

bool RotatedRect::Contains( Point2d pt )
{
    return Contains( pt.x, pt.y );
}

void RotatedRect::Offset( double dx, double dy )
{
    center += Point2d(dx, dy);
    return;
}

RectD RotatedRect::GetBoundRect()
{
    Point2d pts[4];
    Points(pts);

    double minx = pts[0].x;
    double maxx = pts[0].x;
    double miny = pts[0].y;
    double maxy = pts[0].y;

    for(int i = 1; i < 4; ++i){
        if(minx > pts[i].x) minx = pts[i].x;
        if(maxx < pts[i].x) maxx = pts[i].x;
        if(miny > pts[i].y) miny = pts[i].y;
        if(maxy < pts[i].y) maxy = pts[i].y;
    }

    return RectD(minx, miny, maxx - minx, maxy - miny);
}

void RotatedRect::Points( Point2d pts[] )
{
    pts[0] = RotatePt(Point2d(-width / 2., height / 2.), angle) + center;
    pts[1] = RotatePt(Point2d(width / 2., height / 2.), angle) + center;
    pts[2] = RotatePt(Point2d(-width / 2., -height / 2.), angle) + center;
    pts[3] = RotatePt(Point2d(width / 2., -height / 2.), angle) + center;
    return;
}

Point2d RotatedRect::GetCenter() const
{
    return center;
}

void RotatedRect::SetCenter( Point2d pt )
{
    center = pt;
}

double RotatedRect::GetWidth() const
{
    return width;
}

void RotatedRect::SetWidth( double val )
{
    width = val;
}

double RotatedRect::GetHeight() const
{
    return height;
}

void RotatedRect::SetHeight( double val )
{
    height = val;
}

double RotatedRect::GetAngle() const
{
    return angle;
}

void RotatedRect::SetAngle( double ang )
{
    angle = ang;
}

Point2d gf::RotatePt( Point2d pt, double v_ang, Point2d pt_center /*= Point2d(0, 0)*/ )
{
    double x_pos = pt.x - pt_center.x;
    double y_pos = pt.y - pt_center.y;
    pt.x = x_pos * cos(v_ang) - y_pos * sin(v_ang);
    pt.y = x_pos * sin(v_ang) + y_pos * cos(v_ang);
    pt += pt_center;
    return pt;
}

double gf::Distance( Point2d pt1, Point2d pt2 )
{
    return sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));
}

void gf::wxImageU1ToU3( wxImage &img )
{
    if(!img.IsOk())
        return;

    int w = img.GetWidth();
    int h = img.GetHeight();
    unsigned char *img_data = img.GetData();
    for(int i = w * h - 1; i >= 0; --i){
        img_data[i * 3] = img_data[i];
        img_data[i * 3 + 1] = img_data[i];
        img_data[i * 3 + 2] = img_data[i];
    }
}

void gf::wxImageU3ToU1( wxImage &img )
{
    if(!img.IsOk())
        return;

    int w = img.GetWidth();
    int h = img.GetHeight();
    unsigned char *img_data = img.GetData();
    for(int i = w * h - 1; i >= 0; --i){
        img_data[i] = img_data[i*3];
    }
}

gf::ImageU1::ImageU1( int width, int height )
{
    m_width = width;
    m_height = height;
    if(m_width * m_height > 0)
        m_data = new unsigned char[width * height];
    else
        m_data = NULL;
}

gf::ImageU1::ImageU1( const ImageU1 &img )
{
    m_width = img.m_width;
    m_height = img.m_height;
    if(m_width * m_height > 0)
    {
        m_data = new unsigned char[m_width * m_height];
        memcpy(m_data, img.m_data, m_width * m_height);
    } else
        m_data = NULL;
}

gf::ImageU1::ImageU1( int width /*= 0*/, int height /*= 0*/, unsigned char *data )
{
    m_width = width;
    m_height = height;
    m_data = data;
}

gf::ImageU1::~ImageU1()
{
    if(m_data != NULL)
        delete[] m_data;
}

wxImage gf::ImageU1::ChangeTowxImage() const
{
    wxImage rs(m_width, m_height);
    if(m_width * m_height <= 0) return rs;

    unsigned char * rs_data = rs.GetData();

    for(int i = m_width * m_height - 1; i >= 0; --i)
    {
        rs_data[i * 3]     = m_data[i];
        rs_data[i * 3 + 1] = m_data[i];
        rs_data[i * 3 + 2] = m_data[i];
    }
    return rs;
}

gf::ImageU1 gf::ImageU1::CreateFromwxImage( const wxImage &img )
{
    if(!img.IsOk())
        return ImageU1(0, 0);

    int w = img.GetWidth();
    int h = img.GetHeight();
    unsigned char *img_data = img.GetData();
    ImageU1 rs(w, h);
    for(int i = w * h - 1; i >= 0; --i){
        rs.m_data[i] = img_data[i*3];
    }

    return rs;
}

gf::ImageU1 gf::ImageU1::GetSubImage( wxRect rect )
{
    if(rect.width > m_width)    rect.width = m_width;
    if(rect.height > m_height)  rect.height = m_height;
    if(rect.x < 0) rect.x = 0;
    if(rect.y < 0) rect.y = 0;
    if(rect.x + rect.width > m_width) rect.x = m_width - rect.width;
    if(rect.y + rect.height > m_height) rect.y = m_height - rect.height;

    int w = rect.width;
    int h = rect.height;
    ImageU1 rs(w, h);
    for(int j = 0; j < h; ++j){
        for(int i = 0; i < w; ++i){
            rs.m_data[j * w + i] = m_data[(rect.y + j) * m_width + rect.x + i];
        }
    }

    return rs;
}

void gf::ImageU1::CopyFrom(const ImageU1 &img )
{
    if(m_data != NULL)
        delete[] m_data;

    m_width = img.m_width;
    m_height = img.m_height;

    if(m_width*m_height>0)
    {
        m_data = new unsigned char[m_width * m_height];
        memcpy(m_data, img.m_data, m_width * m_height);
    }else
        m_data = NULL;
}

bool gf::ImageU1::CopyFromWxImage( const wxImage &img )
{
    if(!img.IsOk()) return false;
    if(m_data != NULL) delete[] m_data;

    m_width = img.GetWidth();
    m_height = img.GetHeight();
    try
    {
        if(m_width * m_height > 0)
        {
            m_data = new unsigned char[m_width * m_height];
            unsigned char *img_data = img.GetData();
            for(int i = m_width * m_height - 1; i >= 0; --i)
                m_data[i] = img_data[i*3];
        }
        else
            m_data = NULL;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool gf::ImageU1::IsEmpty() const
{
    return m_width <= 0 || m_height <= 0;
}

bool gf::ImageU1::Create( int w, int h, bool clear /*= true*/ )
{
    if(m_data != NULL) delete[] m_data;

    m_width = w;
    m_height = h;
    try
    {
        if(m_width * m_height > 0)
            m_data = new unsigned char[m_width * m_height];
        else
            m_data = NULL;
        if(clear)
            memset(m_data, 0, m_width*m_height);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

//************************************
// return the error code of GetLastError()
// inet = true to return error code from wininet
//************************************
std::string gf::GetLastErrorMsg(bool inet)
{
    DWORD le = GetLastError();
    //std::cout << le << std::endl;
    LPVOID lpBuf;
    if(inet)
    {
        HANDLE h = GetModuleHandleA("wininet.dll");
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS |
            FORMAT_MESSAGE_FROM_HMODULE,
            h,
            le,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),//0,//MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),
            (LPSTR) &lpBuf,
            0,
            NULL);
    }
    else
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        le,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),//0,//MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),
        (LPSTR) &lpBuf,
        0,
        NULL);

    if(lpBuf == NULL)
        return "";

    std::string ss((char *)lpBuf);
    LocalFree(lpBuf);
    return ss;
}
