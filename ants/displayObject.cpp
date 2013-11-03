#include "displayObject.h"


displayObject::displayObject(void){
	this->x= 0;
	this->y= 0;
	this->visible= false;
}

displayObject::displayObject(int x, int y){
	this->x= x;
	this->y= y;
	this->visible= true;
}
		
int displayObject::getX(){
	return this->x;
}

void displayObject::setX(int inpX){
	this->x= inpX;
}

int displayObject::getY(){
	return this->y;
}

void displayObject::setY(int inpY){
	this->y= inpY;
}
		
bool displayObject::isVisible(){
	return this->visible;
}

void displayObject::setVisible(bool visible){
	this->visible= visible;
}
		
int displayObject::getWidth(){
	return this->width;
}

void displayObject::setWidth(int width){
	this->width= width;
}

int displayObject::getHeight(){
	return this->height;
}

void displayObject::setHeight(int height){
	this->height= height;
}

void displayObject::draw(ALLEGRO_BITMAP *image){
	al_draw_bitmap(image, this->x, this->y, 0);
}

displayObject::~displayObject(void){
}
