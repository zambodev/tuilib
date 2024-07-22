#include <vector>
#include <tuple>
#include <cstring>
#include "windowManager.hpp"
#include "utils.hpp"


WindowManager::WindowManager()
    : m_Width(UTILS::getTerminalWidth()), m_Height(UTILS::getTerminalHeight())
{
    this->m_Buffer = new wchar_t[this->m_Width * this->m_Height];
    this->m_BufferLayerMap = new int[this->m_Width * this->m_Height];
    
    for(int i = 0; i < this->m_Width * this->m_Height; ++i)
    {
        this->m_Buffer[i] = U_SPACE;
        this->m_BufferLayerMap[i] = 0;
    }
}

WindowManager::~WindowManager()
{
    delete this->m_Buffer;
}

int WindowManager::getIndexOf(Window& window)
{
    int idx = 0;
    std::vector<Window*>::iterator itr;

    for(itr = this->m_visibilityLayerList.begin(); itr != this->m_visibilityLayerList.end(); ++itr)
    {
        if(*(*itr) == window)
            return idx;

        ++idx;
    }

    return 0;
}

void WindowManager::addWindow(Window& window)
{
    Window *tmp = &window;

    this->m_visibilityLayerList.emplace_back(tmp);

    auto size = window.getSize();
    for(int i = 0; i < size.height; ++i)
        for(int j = 0; j < size.width; ++j)
            this->m_BufferLayerMap[(size.y + i) * this->m_Width + (size.x + j)]
                = this->m_visibilityLayerList.size();
}

void WindowManager::removeWindow(int id)
{

}

void WindowManager::render(void)
{
    int depth;

    for(int i = 0; i < this->m_Height; ++i)
    {
        for(int j = 0; j < this->m_Width; ++j)
        {
            if((depth = this->m_BufferLayerMap[i * this->m_Width + j]) == 0)
                continue;

            Window* window = this->m_visibilityLayerList.at(depth - 1);
            WindowSize_t& size = window->getSize();

            this->m_Buffer[i * this->m_Width + j] = window->getBuffer()[(i - size.y) * size.width + (j - size.x)];
        }
    }

    std::wcout << L"\x1b[s\x1b[0;0H" << this->m_Buffer << L"\x1b[u";
}