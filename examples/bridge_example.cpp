//          Copyright Malcolm Noyes 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "any_bridge.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>

namespace ab = any_bridge;

namespace
{
    // In reality, XWindows and PMWindows probably couldn't co-exist...we'll ignore
    // this here to how the example...
    enum WindowType {XWindowType, PMWindowType};

    typedef int Coord;
    struct Point
    {
        Point() : x(0), y(0) {}
        Point(int x1, int y1) : x(x1), y(y1) {}
        Coord x;
        Coord y;
    };

    class WindowImp
    {
    public:
        virtual ~WindowImp() {}
        //.. ImpTop etc not shown...

        virtual void DeviceRect(Coord, Coord, Coord, Coord) = 0;
        virtual void DeviceBitmap(const char*, Coord, Coord) = 0;
    protected:
        WindowImp();
    };

    WindowImp::WindowImp()
    {}

    class XWindowImp : public WindowImp
    {
    public:
        XWindowImp(std::vector<std::string>* v) : m_log(v) {}
        virtual void DeviceRect(Coord x1, Coord y1, Coord x2, Coord y2)
        {
            assert(m_log);
            std::ostringstream oss;
            oss << "X:Rect:" << x1 << "," << y1 << "," << x2 << "," << y2;
            m_log->push_back(oss.str());
        }
        virtual void DeviceBitmap(const char* name, Coord x1, Coord y1)
        {
            assert(m_log);
            std::ostringstream oss;
            oss << "X:Bitmap:" << name << "," << x1 << "," << y1;
            m_log->push_back(oss.str());
        }
    private:
        std::vector<std::string>* m_log;
    };

    class PMWindowImp : public WindowImp
    {
    public:
        PMWindowImp(std::vector<std::string>* v) : m_log(v) {}
        virtual void DeviceRect(Coord x1, Coord y1, Coord x2, Coord y2)
        {
            assert(m_log);
            std::ostringstream oss;
            oss << "PM:Rect:" << x1 << "," << y1 << "," << x2 << "," << y2;
            m_log->push_back(oss.str());
        }
        virtual void DeviceBitmap(const char* name, Coord x1, Coord y1)
        {
            assert(m_log);
            std::ostringstream oss;
            oss << "PM:Bitmap:" << name << "," << x1 << "," << y1;
            m_log->push_back(oss.str());
        }
    private:
        std::vector<std::string>* m_log;
    };

    typedef ab::any<ab::interfaces<WindowImp>, ab::not_comparable> AnyWindow;

}

////////////////////////////////////////////////
// Template implementations and specializations
//
namespace any_bridge
{
    // forward to imp methods
    template <>
    class forwarder<any<interfaces<WindowImp>, not_comparable> > : public WindowImp
    {
    public:
        virtual void DeviceRect(Coord x1, Coord y1, Coord x2, Coord y2)
        {
            static_cast<any<interfaces<WindowImp>, not_comparable>*>(this)->content->DeviceRect(x1,y1,x2,y2);
        }
        virtual void DeviceBitmap(const char* name, Coord x2, Coord y2)
        {
            static_cast<any<interfaces<WindowImp>, not_comparable>*>(this)->content->DeviceBitmap(name,x2,y2);
        }
    };

    // Implementation of operations on WindowImp
    template <typename Derived, typename Base>
    class value_type_operations<Derived, Base, XWindowImp> : public Base
    {
    public:
        typedef typename Base::PlaceholderType PlaceholderType;
        virtual void DeviceRect(Coord x1, Coord y1, Coord x2, Coord y2)
        {
            static_cast<Derived*>(this)->held.DeviceRect(x1, y1, x2, y2);
        }
        virtual void DeviceBitmap(const char* name, Coord x1, Coord y1)
        {
            static_cast<Derived*>(this)->held.DeviceBitmap(name, x1, y1);
        }
    };

    template <typename Derived, typename Base>
    class value_type_operations<Derived, Base, PMWindowImp> : public Base
    {
    public:
        typedef typename Base::PlaceholderType PlaceholderType;
        virtual void DeviceRect(Coord x1, Coord y1, Coord x2, Coord y2)
        {
            static_cast<Derived*>(this)->held.DeviceRect(x1, y1, x2, y2);
        }
        virtual void DeviceBitmap(const char* name, Coord x1, Coord y1)
        {
            static_cast<Derived*>(this)->held.DeviceBitmap(name, x1, y1);
        }
    };
}

namespace
{
    template <WindowType T>
    AnyWindow createWindowImp(std::vector<std::string>* v);

    template <>
    AnyWindow createWindowImp<XWindowType>(std::vector<std::string>* v)
    {
        return AnyWindow(XWindowImp(v));
    }

    template <>
    AnyWindow createWindowImp<PMWindowType>(std::vector<std::string>* v)
    {
        return AnyWindow(PMWindowImp(v));
    }

    template <WindowType T>
    class Window
    {
    public:
        Window(std::vector<std::string>* v) : m_imp(createWindowImp<T>(v)), m_log(v) {};
        virtual ~Window() {}
        //.. lots of methods not shown...
        virtual void DrawContents()
        {
            DrawRect(Point(11,13),Point(19,23));
        }

        virtual void DrawRect(Point topLeft, Point bottomRight)
        {
            m_imp.DeviceRect(topLeft.x,topLeft.y,bottomRight.x,bottomRight.y);
        }
    protected:
        AnyWindow m_imp;
        std::vector<std::string>* m_log;
    };

    template <WindowType T>
    class ApplicationWindow : public Window<T>
    {
    public:
        ApplicationWindow(const std::string& name, std::vector<std::string>* v)
            : Window<T>(v), m_name(name)
        {}
        virtual void DrawContents()
        {
            this->m_log->push_back("***** ApplicationWindow::DrawContents,begin#" + m_name);
            Window<T>::DrawContents();
            assert(this->m_log);
            this->m_log->push_back("***** ApplicationWindow::DrawContents,end#" + m_name);
        }
    private:
        std::string m_name;
    };

    template <WindowType T>
    class IconWindow : public Window<T>
    {
    public:
        IconWindow(const std::string& name, std::vector<std::string>* v)
            : Window<T>(v), m_bitmapName(name)
        {}
        virtual void DrawContents()
        {
            this->m_log->push_back("***** IconWindow::DrawContents,begin#bitmap:" + m_bitmapName);
            Window<T>::DrawContents();
            this->m_imp.DeviceBitmap(m_bitmapName.c_str(), 57, 63);
            assert(this->m_log);
            this->m_log->push_back("***** IconWindow::DrawContents,end#bitmap:" + m_bitmapName);
        }
    private:
        std::string m_bitmapName;
    };
}

void xwindow_example()
{
    std::vector<std::string> log;

    log.push_back("*******************************************************************************");
    log.push_back("                            XWindow example                                    ");

    ApplicationWindow<XWindowType> w1("AppWindow", &log);
    IconWindow<XWindowType> w2("Icon1", &log);
    IconWindow<XWindowType> w3("Icon2", &log);
    
    w1.DrawContents();
    w2.DrawContents();
    w3.DrawContents();

    log.push_back("*******************************************************************************");

    for(std::vector<std::string>::iterator it = log.begin(), end = log.end(); it != end; ++it )
    {
        std::cout << *it << std::endl;
    }
}

void pmwindow_example()
{
    std::vector<std::string> log;

    log.push_back("*******************************************************************************");
    log.push_back("                           PMWindow example                                    ");

    ApplicationWindow<PMWindowType> w1("AppWindow", &log);
    IconWindow<PMWindowType> w2("Icon1", &log);
    IconWindow<PMWindowType> w3("Icon2", &log);
    
    w1.DrawContents();
    w2.DrawContents();
    w3.DrawContents();

    log.push_back("*******************************************************************************");

    for(std::vector<std::string>::iterator it = log.begin(), end = log.end(); it != end; ++it )
    {
        std::cout << *it << std::endl;
    }
}

void bridge_example()
{
    xwindow_example();
    pmwindow_example();
}
