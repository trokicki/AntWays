#include "gNode.h"


gNode::gNode(void){
}

gNode::gNode(int value, int x, int y, int weight){
	this->index= this->count++;
	this->value= value;
	this->weight= weight;
	this->setX(x);
	this->setY(y);
	this->setWidth(30);
	this->setHeight(30);
	this->next= 0;
	this->imageNo= 0;
	this->nList= 0;
	this->food= false;
	this->antGenerator= false;

	if(this->index== 0)	//TODO: posprz¹taj t¹ prowizorkê
		this->antGenerator= true;
	else if(this->index== 1)
		this->food= true;
}

int gNode::getIndex(){
	return this->index;
}

void gNode::setWeight(int weight){
	this->weight= weight;
}

int gNode::getWeight(){
	return this->weight;
}

void gNode::setImageNo(int imageNo){
	this->imageNo= imageNo;
}

int gNode::getImageNo(){
	return this->imageNo;
}

void gNode::pushN(int index, int weight){
	if(this->nList!= 0){
		gList *tmp= this->nList;
		while(tmp->next!= 0)
			tmp= tmp->next;
		tmp->next= new gList;
		tmp->next->index= index;
		tmp->next->weight= weight;
		tmp->next->hormones= 0;
		tmp->next->next= 0;
	}else{
		this->nList= new gList();
		this->nList->index= index;
		this->nList->weight= weight;
		this->nList->hormones= 0;
		this->nList->next= 0;
	}
}

bool gNode::isFood(){
	return this->food;
}

bool gNode::isAntGenerator(){
	return this->antGenerator;
}

void gNode::draw(){
	if(this->food)
		al_draw_bitmap(this->imgSugar, this->getX() - 23, this->getY() - 40, 0);
	else if(this->antGenerator)
		al_draw_bitmap(this->imgAntGenerator, this->getX() - 37, this->getY() - 37, 0);
	else{
		al_draw_bitmap(this->images[this->imageNo], this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0);
		al_draw_textf(this->font18, al_map_rgb(255, 255, 255), this->getX() - 8, this->getY() - 10, 0, "%d", this->getIndex());
	}
}


gNode::~gNode(void){
}