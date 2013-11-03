#include "edge.h"


edge::edge(){
	this->nodes[0]= 0;
	this->nodes[1]= 0;
	this->next= 0;
	this->hormones= 0;
}

edge::edge(gNode *node1, gNode *node2){
	this->nodes[0]= node1;
	this->nodes[1]= node2;
	this->next= 0;
	this->hormones= 0;
}

edge::edge(gNode *node1, gNode *node2, int weight){
	this->nodes[0]= node1;
	this->nodes[1]= node2;
	this->weight= weight;
	this->next= 0;
	this->hormones= 0;
}

int edge::getWeight(){
	return this->weight;
}

void edge::setWeight(int weight){
	this->weight= weight;
}

long edge::getHormones(){
	return this->hormones;
}

void edge::setHormones(long hormones){
	if(hormones>= 0)
		this->hormones= hormones;
}

void edge::draw(){
	if(this->nodes[0]!= 0 && this->nodes[1]!= 0)
		al_draw_line(this->nodes[0]->getX(), this->nodes[0]->getY(), this->nodes[1]->getX(), this->nodes[1]->getY(), al_map_rgb(128, 128, 128), 2);

	al_draw_textf(this->font18, al_map_rgb(0, 128, 0), (this->nodes[1]->getX() + this->nodes[0]->getX()) / 2, (this->nodes[1]->getY() + this->nodes[0]->getY()) / 2, 0, "%d", this->hormones);
}

edge::~edge(void){}
