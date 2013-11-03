#pragma once
#include "displayobject.h"
#include "gNode.h"
#include "edge.h"

struct routeNode{
	gNode *content;
	routeNode *next;
};

class ant: public displayObject{
	private:
		routeNode *route; // stack
		gNode *bNode; // begin node - ant moves from bNode to eNode
		gNode *eNode; // end node
		float speedX;
		float speedY;
		float rotation;
		int imageNo;
		int frameNo;
		int frameDelay;
		int frameWidth;
		bool hasFood; // if has food, moves back to AntGenerator, leaving hormones
		bool followsFeromones;

	public:
		ant();
		ant(gNode *antGenerator);

		void update(gNode **graph, edge *edges);
		void draw();

		ant *next;

		static ALLEGRO_BITMAP *images[];
		static ALLEGRO_BITMAP *imgHormones;
		static ALLEGRO_BITMAP *imgSugar;
		static int hormoneParam; // number of hormones leaving
		static int cnt;
		static int speed;

		~ant(void);
};

