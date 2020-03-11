#pragma once

#include "ofMain.h"

class Particle : public ofBaseApp {
public:
	void setup(int x, int y);
	void update(vector<Particle>* particles, int i);
	void draw();
	void addForce(ofVec2f p, float dystans);
	void setParticles(vector<Particle>* particles);
	void updateParticles(Particle * particle2);

	bool podpalona = false;

	int max_czas_zycia;	//przez ile tickow czateczka sie pali

	float lifespan = 0;	//ile tickow czasteczka jest w egzystencji
	float minX = 0;	//dolna granica X obszaru
	float minY = 0; //dolna granica Y obszaru
	float maxX;	//gorna granica X obszaru
	float maxY;	//gorna granica Y obszaru
	float m; // masa

	ofVec2f p; //Pozycja
	ofVec2f v; //predkosc czasteczki
	ofVec2f sily; // wektor sil x,y
	ofVec2f reverseX = { -1, 1 };
	ofVec2f reverseY = { 1, -1 };
	ofColor colour; //kolor czasteczki r,g,b

	int radius;
	int czas_palenia=0; //ile tickow czasteczka sie pali

	vector<Particle>* particles; //referencja do vektora wszystkich czasteczek ktory nalezy do ofApp
};
class Particle_Generator : public ofBaseApp {
public:
	Particle_Generator(int x, int y, int width, int height, float angle, vector<Particle_Generator>* generatory); //x,y, szerokosc, wysokosc, kat generatora, vektor generatorow

	void draw();
	void update(vector<Particle>* particles);

	ofColor colour;
	ofVec2f p; //pozycja lewego gornego rogu generatora x,y
	ofVec2f rozmiar; // rozmiar szerokosc, wysokosc
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mouseReleased(int x, int y, int button);

	vector<Particle> particles; //vektor wszystkich czasteczek
	vector<Particle_Generator> generatory;
};