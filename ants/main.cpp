#include <stdio.h>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include "displayObject.h"
#include "gNode.h"
#include "edge.h"
#include "ant.h"
 
const float FPS= 30;
const int SCREEN_W= 1024;
const int SCREEN_H= 768;

#include <iostream>
using namespace std;

int gNode::count= 0;
ALLEGRO_BITMAP *gNode::images[2];
ALLEGRO_FONT *gNode::font18;
int ant::hormoneParam= 250;
int ant::cnt= 0;
int ant::speed= 3;
ALLEGRO_BITMAP *ant::images[6];
ALLEGRO_BITMAP *ant::imgHormones;
ALLEGRO_BITMAP *ant::imgSugar;

ALLEGRO_BITMAP *gNode::imgSugar;
ALLEGRO_BITMAP *gNode::imgAntGenerator;

ALLEGRO_FONT *edge::font18;

void writeDownNList(gNode **graph, int gNodeCnt);


int main(int argc, char **argv){
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font20;
	bool redraw = true;

	displayObject *btnStart= new displayObject(10, 10);
	btnStart->setWidth(180);
	btnStart->setHeight(40);
	ALLEGRO_BITMAP *bmpBtnStart;
	ALLEGRO_BITMAP *imgBg;
 
	if(!al_init()){
		cout << "Lipa z inicjalizacja allegro." << endl;
		return -1;
	}

	 if(!al_init_primitives_addon()){
		 cout << "Lipa z inicjalizacja primitives." << endl;
			return -1;
	 }

	 if(!al_init_image_addon()){
		 cout << "Lipa z inicjalizacja image." << endl;
			return -1;
	 }
 
   if(!al_install_mouse()){
      cout << "Lipa z inicjalizacja myszy." << endl;
      return -1;
   }
 
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      cout << "Lipa z inicjalizacja timera." << endl;
      return -1;
   }
 
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display){
      cout << "Lipa z tworzeniem displaya." << endl;
      al_destroy_timer(timer);
      return -1;
   }

   event_queue = al_create_event_queue();
   if(!event_queue) {
      cout << "Lipa z tworzeniem event queue." << endl;
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	al_start_timer(timer);

	al_init_font_addon();
	al_init_ttf_addon();
	gNode::font18= al_load_ttf_font("./others/arial.ttf", 18, 0);
	if(!gNode::font18)
		cout << "Lipa z ladowaniem fonta." << endl;
	font20= al_load_ttf_font("./others/arial.ttf", 20, 0);
	if(!font20)
		cout << "Lipa z ladowaniem fonta." << endl;
	edge::font18= al_load_ttf_font("./others/arial.ttf", 18, 0);
	if(!edge::font18)
		cout << "Lipa z ladowaniem fonta." << endl;


	bmpBtnStart= al_create_bitmap(btnStart->getWidth(), btnStart->getHeight());
	al_set_target_bitmap(bmpBtnStart);
	al_draw_filled_rectangle(0, 0, btnStart->getWidth(), btnStart->getHeight(), al_map_rgb(50, 0, 168));
	al_draw_text(font20, al_map_rgb(255, 255, 255), 13, 8, 0, "GENERUJ GRAF");
	al_set_target_bitmap(al_get_backbuffer(display));
	
	imgBg= al_load_bitmap("./graphics/bg.png");

	gNode::images[0]= al_create_bitmap(30, 30);
	al_set_target_bitmap(gNode::images[0]);
	al_draw_filled_circle(15, 15, 15, al_map_rgb(0, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(display));

	gNode::images[1]= al_create_bitmap(30, 30);
	al_set_target_bitmap(gNode::images[1]);
	al_draw_filled_circle(15, 15, 15, al_map_rgb(0, 168, 0));
	al_set_target_bitmap(al_get_backbuffer(display));


	ALLEGRO_BITMAP *tmpBitmap= al_load_bitmap("./graphics/ant.png");
	for(int i=0; i< 6; i++)
		ant::images[i]= al_create_sub_bitmap(tmpBitmap, 48 * i, 0, 45, 45);
	ant::imgHormones= al_load_bitmap("./graphics/thFeromones.png");
	ant::imgSugar= al_load_bitmap("./graphics/thSugar.png");

	gNode::imgSugar= al_load_bitmap("./graphics/sugar.png");
	gNode::imgAntGenerator= al_load_bitmap("./graphics/antGenerator.png");;
	/*
	ant::images[0]= al_create_bitmap(30, 16); // ant sprite sheet
	al_set_target_bitmap(ant::images[0]);
	al_draw_filled_rectangle(0, 0, 30, 16, al_map_rgb(90, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(display));
	*/

	gNode *gNodeList= 0;
	gNode *gNodeListTail= 0;
	int gNodeCnt= 0;

	edge *edgeList= 0;
	edge *edgeListTail= 0;

	gNode **graph;

	int drawFlag= 0; // 0- draw node; 1- node selected, draw line;
	int mousePosition[2]= {};

	gNode *drawLineS= 0;
	gNode *drawLineE= 0;

	ant *antList= 0;

	bool clearArea= true;
	bool graphReady= false;
	int delay= 0;
	int antGeneratingDelay= 80;
	int antCount= 24;
	int cursor= 1; // 0- standard, 1- antGenerator, 2- sugar

	while(true){ // MAIN LOOP
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
 
		if(ev.type == ALLEGRO_EVENT_TIMER){
			redraw= true;
			if(ant::cnt < antCount && graphReady){
				if(delay >= antGeneratingDelay){
					//------ generate ant --------------
					if(antList== 0){
						antList= new ant(graph[0]);
						//cout << "Utworzona pierwsza mrowka. W sumie: " << ant::cnt << endl;
					}else{
						ant *tmp= antList;
						while(tmp->next!= 0)
							tmp= tmp->next;
						tmp->next= new ant(graph[0]);
						//cout << "Utworzona kolejna mrowka. W sumie: " << ant::cnt << endl;
					}
					delay= 0;
				}else{
					delay++;
				}
			}
			ant *tmp= antList;
			while(tmp!= 0){
				tmp->update(graph, edgeList);
				tmp= tmp->next;
			}
			edge *tmpEdge= edgeList;
			while(tmpEdge!= 0){
				tmpEdge->setHormones(tmpEdge->getHormones() - 1);
				tmpEdge= tmpEdge->next;
			}
			//--- GRAPH - hormones update -------
			if(graphReady){
				gList *tmpGL= 0;
				for(int i=0; i<gNodeCnt; i++){
					tmpGL= graph[i]->nList;
					while(tmpGL!= 0){
						if(tmpGL->hormones> 0)
							tmpGL->hormones--;
						tmpGL= tmpGL->next;
					}
				}
			}
		}else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			break;
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY){
			mousePosition[0]= ev.mouse.x;
			mousePosition[1]= ev.mouse.y;
			if(gNodeList!= 0){
				gNode *tmp= gNodeList;
				while(tmp!= 0 && drawFlag== 1){
					if(sqrt(pow((double)abs(ev.mouse.x - tmp->getX()), 2) + pow((double)abs(ev.mouse.y - tmp->getY()), 2)) <= tmp->getWidth()){
						tmp->setImageNo(1);
					}else if(tmp!= drawLineS){
						tmp->setImageNo(0);
					}
					tmp= tmp->next;
				}
			}
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			clearArea= true;
			//cout << "Button: " << ev.mouse.button << " pressed." << endl;

			if(mousePosition[0]>= btnStart->getX() && mousePosition[0]<= btnStart->getX() + btnStart->getWidth() && mousePosition[1] >= btnStart->getY() && mousePosition[1] <= btnStart->getY() + btnStart->getHeight()){
				cout << "Tworzy graf. Wezlow: " << gNodeCnt << endl;
				clearArea= false;
				graph= new gNode*[gNodeCnt];
				gNode *tmp= gNodeList;
				edge *tmpEdge= edgeList;
				for(int i=0; i<gNodeCnt; i++){
					graph[i]= tmp;
					tmpEdge= edgeList;
					while(tmpEdge!= 0){
						if(tmpEdge->nodes[0]== graph[i]){
							graph[i]->pushN(tmpEdge->nodes[1]->getIndex(), tmpEdge->getWeight());
						}else if(tmpEdge->nodes[1]== graph[i]){
							graph[i]->pushN(tmpEdge->nodes[0]->getIndex(), tmpEdge->getWeight());
						}
						tmpEdge= tmpEdge->next;
					}
					tmp= tmp->next;
				}
				//--------------------
				cout << "Utworzony." << endl;
				graphReady= true;
				writeDownNList(graph, gNodeCnt);
			}

			if(clearArea && !graphReady){
				if(gNodeList!= 0){
					gNode *tmp= gNodeList;
					while(tmp!= 0 && drawFlag!= 1){
						if(sqrt(pow((double)abs(ev.mouse.x - tmp->getX()), 2) + pow((double)abs(ev.mouse.y - tmp->getY()), 2)) <= tmp->getWidth()){
							drawFlag= 1;
							drawLineS= tmp;
							drawLineS->setImageNo(1);
							cout << "Rysuje od: " << drawLineS->getX() << ", " << drawLineS->getY() << endl;
						}else{
							tmp= tmp->next;
						}
					}
				}
			}

		}else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
			//cout << "Button: " << ev.mouse.button << " released." << endl;
			clearArea= true;
			if(mousePosition[0]>= btnStart->getX() && mousePosition[0]<= btnStart->getX() + btnStart->getWidth() && mousePosition[1] >= btnStart->getY() && mousePosition[1] <= btnStart->getY() + btnStart->getHeight())
				clearArea= false;

			if(clearArea && !graphReady){
				bool hitTest= false;
				drawLineE= 0;
				if(gNodeList!= 0){
					gNode *tmp= gNodeList;
					while(tmp!= 0 && !hitTest){
						if(tmp!= drawLineS && sqrt(pow((double)abs(ev.mouse.x - tmp->getX()), 2) + pow((double)abs(ev.mouse.y - tmp->getY()), 2)) <= tmp->getWidth()){
							if(drawFlag== 1){
								drawLineE= tmp;
								cout << "Konczy na: " << drawLineE->getX() << ", " << drawLineE->getY() << endl;
								drawFlag= 0;
							}
							hitTest= true;
						}else{
							tmp= tmp->next;
						}
					}
					if(drawLineE!= 0){ //creates edge
							cout << "TWORZY KRAWEDZ";
							bool exists= false;
							if(edgeList!= 0){
								edge *tmp= edgeList;
								while(tmp!= 0 && !exists){
									if(tmp->nodes[0]== drawLineS && tmp->nodes[1]== drawLineE || tmp->nodes[0]== drawLineE && tmp->nodes[1]== drawLineS)
										exists= true;
									tmp= tmp->next;
								}
							}

							if(!exists){
								int weight= sqrt(pow((double)abs(drawLineE->getX() - drawLineS->getX()), 2) + pow((double)abs(drawLineE->getY() - drawLineS->getY()), 2));
								if(edgeList!= 0){
									edgeListTail->next= new edge(drawLineS, drawLineE, weight);
									edgeListTail= edgeListTail->next;
								}else{
									edgeList= new edge(drawLineS, drawLineE, weight);
									edgeListTail= edgeList;
								}
								cout << " - utworzyl. Waga: " << weight;
							}
							cout << endl;
						
							drawLineE->setImageNo(0);
					}
					if(drawLineS!= 0)
						drawLineS->setImageNo(0);					
				}
				if(!hitTest && drawFlag== 0){ //creates new node			
					if(gNodeList!= 0){
						gNodeListTail->next= new gNode(gNodeCnt++, ev.mouse.x, ev.mouse.y);
						gNodeListTail= gNodeListTail->next;
					}else{
						gNodeList= new gNode(gNodeCnt++, ev.mouse.x, ev.mouse.y);
						gNodeListTail= gNodeList;
					}
					if(gNodeCnt > 1)
						cursor= 0;
					else if(gNodeCnt== 0)
						cursor= 1;
					else if(gNodeCnt== 1)
						cursor= 2;
				}
			}
			drawFlag= 0;
			drawLineS= 0;
		}
 
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw= false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_bitmap(imgBg, 0, 0, 0);

			if(drawLineS!= 0){
				int weight= sqrt(pow((double)abs(mousePosition[0] - drawLineS->getX()), 2) + pow((double)abs(mousePosition[1] - drawLineS->getY()), 2));
				al_draw_line(drawLineS->getX(), drawLineS->getY(), mousePosition[0], mousePosition[1], al_map_rgb(128, 128, 128), 2);
				//al_draw_textf(font20, al_map_rgb(127, 192, 40), (mousePosition[0] + drawLineS->getX()) / 2, (mousePosition[1] + drawLineS->getY()) / 2, 0, "%d", weight);
				al_draw_textf(font20, al_map_rgb(127, 192, 40), mousePosition[0], mousePosition[1] + 25, 0, "%d", weight);
			}

			if(edgeList!= 0){
				edge *tmp= edgeList;
				while(tmp!= 0){
					tmp->draw();
					tmp= tmp->next;
				}
			}

			if(gNodeList!= 0){
				gNode *tmp= gNodeList;
				while(tmp!= 0){
					tmp->draw();
					tmp= tmp->next;
				}
			}

			if(antList!= 0){
				ant *tmp= antList;
				while(tmp!= 0){
					tmp->draw();
					tmp= tmp->next;
				}
			}

			switch(cursor){
				case 1:
					al_draw_tinted_bitmap(gNode::imgAntGenerator, al_map_rgba_f(1, 1, 1, 0.9), mousePosition[0] - 37, mousePosition[1] - 37, 0);
					break;
				case 2:
					al_draw_tinted_bitmap(gNode::imgSugar, al_map_rgba_f(1, 1, 1, 0.9), mousePosition[0] - 23, mousePosition[1] - 40, 0);
					break;
			}

			//al_draw_textf(font20, al_map_rgb(255, 255, 255), 400, 8, 0, "%s", "Ustaw mrowisko.");

			if(!graphReady)
				btnStart->draw(bmpBtnStart);

			al_flip_display();
		}
	}
	 //---------- clean it up -------
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 
   return 0;
}

void writeDownNList(gNode **graph, int gNodeCnt){
	gList *tmpGList;
	for(int i=0; i<gNodeCnt; i++){
		cout << "Index " << graph[i]->getIndex();
		if(graph[i]->nList!= 0){
			tmpGList= graph[i]->nList;
			cout << ": H";
			while(tmpGList!= 0){
				cout << "-[" << tmpGList->weight << "]->";
				cout << tmpGList->index;
				tmpGList= tmpGList->next;
			}
		}
		cout << endl;
	}
}