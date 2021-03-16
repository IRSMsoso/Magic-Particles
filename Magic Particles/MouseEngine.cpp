#include "MouseEngine.h"

MouseEngine::MouseEngine() {

	baseCursor = new sf::Image;
	baseCursor->loadFromFile("basecursor.png");

	modifiedCursor = new sf::Image;

	coreRank = Core::CoreRank::Black;

	shouldUpdate = true;
	
	points = 0;
}

void MouseEngine::setCoreRank(Core::CoreRank newRank) {
	if (coreRank != newRank) {
		coreRank = newRank;
		shouldUpdate = true;
	}
}

void MouseEngine::update() {
	updateRankFromPoints();

	if (shouldUpdate) {
		processImage();
		setCursorFromSFMLImage(modifiedCursor);
		shouldUpdate = false;
	}
	//std::cout << "Points: " << points << std::endl;
}

void MouseEngine::updateRankFromPoints() {
	double rankValue = sqrt((double)points / 510.0);
	int rankInt = (int)floor(rankValue);

	if (rankValue > 14) {
		setCoreRank(Core::Purple);
	}
	else {
		setCoreRank((Core::CoreRank)rankInt);
	}
}

void MouseEngine::setCursorFromSFMLImage(sf::Image* image) {

	HDC hDC = GetDC(NULL);
	HDC hBitmapDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, 32, 32);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);


	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			sf::Color newcolor = image->getPixel(x, y);
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

void MouseEngine::processImage()
{
	*modifiedCursor = *baseCursor; //Copy over image.

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			if (modifiedCursor->getPixel(x, y) == HEARTCOLOR) {
				modifiedCursor->setPixel(x, y, getCoreColor());
			}
		}
	}
}

