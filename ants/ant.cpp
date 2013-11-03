#include "ant.h"

#include <iostream>

ant::ant(){}

ant::ant(gNode *antGenerator){
	this->setX(antGenerator->getX());
	this->setY(antGenerator->getY());
	this->hasFood= false;
	this->followsFeromones= false;
	this->bNode= antGenerator;
	this->route= 0;
	this->eNode= 0;
	this->next= 0;
	this->speedX= 0;
	this->imageNo= 0;
	this->speedY= 0;
	this->rotation= 0;
	this->frameNo= 0;
	this->frameDelay= 0;
	this->frameWidth= 33;
	this->cnt++;
}

void ant::update(gNode **graph, edge *edges){
	if(this->bNode!= 0 && this->eNode== 0){
		if(this->bNode->nList!= 0){
			gList *tmp= this->bNode->nList;
			int maxIndex= tmp->index;
			int maxHormones= tmp->hormones;
			tmp= tmp->next;
			while(tmp!= 0){
				if(tmp->hormones >= maxHormones){
					maxIndex= tmp->index;
					maxHormones= tmp->hormones;
				}
				tmp= tmp->next;
			}
			this->eNode= graph[maxIndex];

			this->speedX= this->speed * (float)(this->eNode->getX() - this->bNode->getX()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
			this->speedY= this->speed * (float)(this->eNode->getY() - this->bNode->getY()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
			this->rotation= atan((float)(this->eNode->getY() - this->bNode->getY()) / (this->eNode->getX() - this->bNode->getX()));


			routeNode *tmpNode= new routeNode;
			tmpNode->content= this->eNode;
			tmpNode->next= new routeNode;
			tmpNode->next->content= this->bNode;
			tmpNode->next->next= this->route;
			this->route= tmpNode;
		}
	}else if(this->x > this->eNode->getX() - this->eNode->getWidth() / 3 && this->x < this->eNode->getX() + this->eNode->getWidth() / 3 && this->y > this->eNode->getY() - this->eNode->getHeight() / 3 && this->y < this->eNode->getY() + this->eNode->getHeight() / 3){
		this->x= this->eNode->getX();
		this->y= this->eNode->getY();
		if(this->eNode->isFood()){
			routeNode *tmpToDelete= this->route;
			this->route= this->route->next;
			delete tmpToDelete;
	
			this->hasFood= true;
			this->followsFeromones= false;
		}
		//find next node

		if(!this->hasFood){
			int parentIndex= this->bNode->getIndex();
			this->bNode= this->eNode;
			if(this->eNode->nList!= 0){
				gList *tmp= this->eNode->nList;
				int maxIndex= tmp->index;
				int maxHormones= 0;
				int cnt= 0;
				//tmp= tmp->next;
				while(tmp!= 0){
					if(tmp->hormones > maxHormones){
						if(tmp->index!= parentIndex)
							maxHormones= tmp->hormones;
						maxIndex= tmp->index;
					}
					tmp= tmp->next;
					cnt++;
				}
				//std::cout << "--- Trasa: " << parentIndex << " -> " << maxIndex << " hormonow: " << maxHormones << std::endl;
				if(maxHormones == 0 || maxIndex == parentIndex){
					//randomizuj do maxIndex
					if(cnt > 0){
						srand(time(0) * (this->cnt + 1));
						int seekCnt= 0;
						while(seekCnt < 10){
							tmp= this->eNode->nList;
							maxIndex= tmp->index;
							while(tmp!= 0){
								if(tmp->index!= parentIndex)
									if(rand() % cnt == 0){
										maxIndex= tmp->index;
										seekCnt= 10;
									}
								tmp= tmp->next;
							}
							seekCnt++;
						}
					}
					//std::cout << "[losowy: " << maxIndex << "] ";
				}else{
					this->hasFood= false;
					this->followsFeromones= true;
				}
				this->eNode= graph[maxIndex];	

				this->speedX= this->speed * (float)(this->eNode->getX() - this->bNode->getX()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
				this->speedY= this->speed * (float)(this->eNode->getY() - this->bNode->getY()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
				
				sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));

				this->rotation= atan((float)(this->eNode->getY() - this->bNode->getY()) / (this->eNode->getX() - this->bNode->getX()));

				//std::cout << " znaleziony: #" << maxIndex << " [" << this->eNode->getX() << ", " << this->eNode->getY() << "]" << std::endl;
				routeNode *tmpNode= new routeNode;
				tmpNode->content= this->eNode;
				tmpNode->next= this->route;
				this->route= tmpNode;
			}
		}else{
			this->bNode= this->eNode;
			if(this->route!= 0){
				this->eNode= this->route->content;
				routeNode *tmpToDelete= this->route;
				this->route= this->route->next;
				delete tmpToDelete;

				this->speedX= this->speed * (float)(this->eNode->getX() - this->bNode->getX()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
				this->speedY= this->speed * (float)(this->eNode->getY() - this->bNode->getY()) / sqrt(pow(abs((float)this->eNode->getX() - this->bNode->getX()), 2) + pow(abs((float)this->eNode->getY()) - this->bNode->getY(), 2));
				this->rotation= atan((float)(this->eNode->getY() - this->bNode->getY()) / (this->eNode->getX() - this->bNode->getX()));

				//std::cout << " sciagam ze stosu: #" << this->eNode->getIndex() << std::endl;
				gList *tmp= graph[this->bNode->getIndex()]->nList;
				while(tmp->index != this->eNode->getIndex() && tmp!= 0){
					tmp= tmp->next;
				}
				tmp->hormones+= this->hormoneParam; //TODO: dodawanie hormonow
				tmp= graph[this->eNode->getIndex()]->nList;
				while(tmp->index != this->bNode->getIndex() && tmp!= 0){
					tmp= tmp->next;
				}
				tmp->hormones+= this->hormoneParam; //TODO: dodawanie hormonow
				//std::cout << "Dodane hormony dla galezi: " << this->bNode->getIndex() << " - " << this->eNode->getIndex() << std::endl;

				edge *tmpEdge= edges;
			
				while(tmpEdge->next!= 0){
					if(tmpEdge->nodes[0] == this->bNode && tmpEdge->nodes[1] == this->eNode || tmpEdge->nodes[1] == this->bNode && tmpEdge->nodes[0] == this->eNode )
						break;
					tmpEdge= tmpEdge->next;
				}
			
				tmpEdge->setHormones(tmpEdge->getHormones() + this->hormoneParam);
			}else{
				this->eNode= 0;
				this->hasFood= false;
				this->followsFeromones= false;
			}
		}
	}
	if(this->eNode!= 0 && this->bNode!= 0){
		this->x+= this->speedX;
		this->y+= this->speedY;

		this->frameDelay++;
		if(this->frameDelay >= 3){
			if(this->frameNo < 5)
				this->frameNo++;
			else
				this->frameNo= 0;
			this->frameDelay= 0;
		}

	}
}

void ant::draw(){
	//al_draw_rotated_bitmap(this->images[this->imageNo], 15, 8, this->x, this->y, this->rotation, 0);
	if(this->speedX< 0)
		al_draw_rotated_bitmap(this->images[this->frameNo], 24, 16, this->x, this->y, this->rotation + 4.71, 0);
	else
		al_draw_rotated_bitmap(this->images[this->frameNo], 24, 16, this->x, this->y, this->rotation + 1.57, 0);
	
	if(this->hasFood)
		al_draw_bitmap(this->imgSugar, this->x, this->y - 40, 0);
	else if(this->followsFeromones)
		al_draw_bitmap(this->imgHormones, this->x, this->y - 40, 0);
}

ant::~ant(void){
}
