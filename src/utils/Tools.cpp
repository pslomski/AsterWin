#include "Tools.hpp"
#include <filesystem>
#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "game/Consts.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////
// punkt przeciecica odcinkow
// result
// 0 - odcinki sie przecinaja; x, y zawieraja punkt przeciecia
// 1 - przeciecie lezy na o1 ale poza o2; x, y zawieraja punkt przeciecia
// 2 - przeciecie lezy na o2 ale poza o1; x, y zawieraja punkt przeciecia
// 3 - proste sie przecinaja ale poza odcinkami; x, y zawieraja punkt przeciecia
// 4 - odcinki sa rownolegle
int linesIntersection(const BoxF& o1, const BoxF& o2, Float& x, Float& y)
{
    Float ua, ub;
    Float la, lb, m; // licznik i mianownik
    m = (o2.y1 - o2.y0) * (o1.x1 - o1.x0) - (o2.x1 - o2.x0) * (o1.y1 - o1.y0);
    if (fabs(m) < 1e-6)
    {
        // jezeli mianownik=0 wtedy linie sa rownolegle
        return 4;
    }
    la = (o2.x1 - o2.x0) * (o1.y0 - o2.y0) - (o2.y1 - o2.y0) * (o1.x0 - o2.x0);
    lb = (o1.x1 - o1.x0) * (o1.y0 - o2.y0) - (o1.y1 - o1.y0) * (o1.x0 - o2.x0);

    ua = la / m;
    x = o1.x0 + ua * (o1.x1 - o1.x0);
    y = o1.y0 + ua * (o1.y1 - o1.y0);
    ub = lb / m;
    if ((ua >= 0) && (ua <= 1) && (ub >= 0) && (ub <= 1))
        return 0;
    else if ((ua >= 0) && (ua <= 1) && ((ub < 0) || (ub > 1)))
        return 1;
    else if (((ua < 0) || (ua > 1)) && (ub >= 0) && (ub <= 1))
        return 2;
    else
        return 3;
}

PointF geRotate(PointF& pt, Float alfa)
{
    PointF res;
    Float sinalfa = sin(-alfa * GE_PIover180);
    Float cosalfa = cos(-alfa * GE_PIover180);
    res.x = pt.x * cosalfa + pt.y * sinalfa;
    res.y = pt.x * sinalfa + pt.y * cosalfa;
    return res;
}

bool setVSync(int interval)
{
    auto extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    if (strstr(extensions, "WGL_EXT_swap_control") == 0)
    {
        return false;
    }
    else
    {
        using WglSwapIntervalEXT = bool(APIENTRY*)(int);
        auto wglSwapIntervalEXT = (WglSwapIntervalEXT)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(interval);
        }
        using WglGetSwapIntervalEXT = int (*)();
        auto wglGetSwapIntervalEXT = (WglGetSwapIntervalEXT)wglGetProcAddress("wglGetSwapIntervalEXT");
        if (wglGetSwapIntervalEXT)
        {
            return wglGetSwapIntervalEXT() > 0;
        }
        else
        {
            return false;
        }
    }
}

std::string getAppDir()
{
    char filepath[MAX_PATH];
    GetModuleFileName(NULL, filepath, MAX_PATH);
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}
