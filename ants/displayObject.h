#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class displayObject{
	protected:
		bool visible;
		float x;
		float y;
		int width;
		int height;

	public:
		displayObject(void);
		displayObject(int x, int y);
		
		int getX();
		void setX(int inpX);
		int getY();
		void setY(int inpY);
		bool isVisible();
		void setVisible(bool visible);
		int getWidth();
		void setWidth(int width);
		int getHeight();
		void setHeight(int height);
		void draw(ALLEGRO_BITMAP *image);

		~displayObject(void);
};

