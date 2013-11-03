#pragma once
#include "displayobject.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

struct gList{
	int index;
	int weight;
	long hormones;
	gList *next;
};

class gNode: public displayObject{
	private:
		int index;
		int value;
		int weight;

		int imageNo;
		bool food;
		bool antGenerator;

	public:
		gNode(void);
		gNode(int value, int x= 0, int y= 0, int weight= 1);

		static int count;

		int getIndex();
		void setWeight(int weight);
		int getWeight();
		void setImageNo(int imageNo);
		int getImageNo();
		void pushN(int index, int weight);
		bool isFood();
		bool isAntGenerator();

		void draw();

		gNode *next; // temporary public
		gList *nList;

		static ALLEGRO_BITMAP *images[];
		static ALLEGRO_BITMAP *imgSugar;
		static ALLEGRO_BITMAP *imgAntGenerator;
		static ALLEGRO_FONT *font18;

		~gNode(void);
};