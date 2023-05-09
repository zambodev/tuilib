#include "../include/Window.hpp"


Window::Window(std::string title)
{	
#ifdef __linux__
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cols = w.ws_col;
	rows = w.ws_row;
#elif _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

	buffer = (wchar_t *)malloc(sizeof(wchar_t) * cols * rows);

	// Fill with blank
	for(int i = 0; i < cols * rows; ++i)
		buffer[i] = u'\u0020';

	boxes.insert({"main", new Box(0, 0, cols-1, rows-1, title)});
}

void Window::refresh(void)
{
	wprintf(L"\x1b[0;0H");

	for(int i = 0; i < cols * rows; ++i)
		wprintf(L"%lc", buffer[i]);

	fflush(stdout);
}

void Window::create_box(std::string id, int x1, int y1, int x2, int y2, std::string title, std::string text)
{
	boxes.try_emplace(id, new Box(x1, y1, x2, y2, title, text));
}

void Window::delete_box(std::string id)
{
	boxes[id]->~Box();
	boxes.erase("id");
}


Window::Box * Window::get_box(std::string id)
{
	return boxes[id];
}
