#include <sciter-x.h>
#include <sciter-x-window.hpp>

////////////////////////////////////////////////////////////////////////////////

class TFrame
    : public sciter::window
{
public:
    SOM_PASSPORT_BEGIN_EX(frame, TFrame)
    SOM_FUNCS(
      SOM_FUNC_EX(nativeMessage, NativeMessage)
    )
    SOM_PASSPORT_END

public:
    TFrame()
        : window(
            SW_TITLEBAR |
            SW_RESIZEABLE |
            SW_CONTROLS |
            SW_MAIN |
            SW_ENABLE_DEBUG)
    {}

    sciter::string NativeMessage()
    {
        return WSTR("Hello C++ World");
    }
};

////////////////////////////////////////////////////////////////////////////////

#include "resources.cpp"

int uimain(std::function<int()> run )
{
    sciter::archive::instance().open(aux::elements_of(resources));

    sciter::om::hasset<TFrame> pwin = new TFrame();

    pwin->load(WSTR("this://app/main.html"));
    pwin->expand();

    return run();
}
