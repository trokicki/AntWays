#pragma once

#include "gNode.h"

class edge{
	private:
		int weight;
		long hormones;

	public:
		edge();
		edge(gNode *node1, gNode *node2);
		edge(gNode *node1, gNode *node2, int weight);

		void draw();
		int getWeight();
		void setWeight(int weight);
		long getHormones();
		void setHormones(long hormones);
		
		gNode *nodes[2];
		edge *next;


		static ALLEGRO_FONT *font18;



		~edge(void);
};

