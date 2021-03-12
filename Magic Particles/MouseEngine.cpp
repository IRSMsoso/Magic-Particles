#include "MouseEngine.h"

MouseEngine::MouseEngine() {

	baseCursor.loadFromFile("basecursor.png");

	setCursorFromSFMLImage(baseCursor);

}

void MouseEngine::setCursorFromSFMLImage(sf::Image& image) {

	HDC hDC = GetDC(NULL);
	HDC hBitmapDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, 32, 32);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);


	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			sf::Color newcolor = image.getPixel(x, y);
			COLORREF newcolorref = RGB(newcolor.r, newcolor.g, newcolor.b);
			SetPixel(hBitmapDC, x, y, newcolorref);
		}
	}


	DeleteDC(hBitmapDC);
	ReleaseDC(NULL, hDC);

	HCURSOR hCursor = CColorCursor::CreateCursorFromBitmap(hBitmap, RGB(255, 255, 255), 0, 0);

	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);

	SetSystemCursor(hCursor, 32512);

}

