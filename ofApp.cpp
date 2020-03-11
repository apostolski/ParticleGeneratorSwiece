#include "ofApp.h"
#include <math.h>
#include <omp.h>
#define pi	3.14

void ofApp::setup() {
	ofBackground(0);	//		x	y	szer.wys. kat vektor generatorow
	Particle_Generator generator(500,700,50,100,0,&generatory);
	Particle_Generator generator2(300, 500, 50, 100, 0, &generatory);	//konstruktor emitera
}

void Particle::setup(int x, int y) {
	max_czas_zycia = ofRandom(3, 7); //Po ilu tickach czasteczka zniknie jezeli sie pali
	radius = ofRandom(1, 5);
	m = 2 * pi * pow(radius, 2);	//poczatkowe wartosci
	p.x = ofRandom(x, x+50);
	p.y = y;
	v.x,v.y = 0,0;
	colour = { 220,220,220 };
}

void ofApp::update() {

	vector<int> do_usuniecia;	//zeruje vektor partikli do usuniecia
#pragma omp parallel for
	for (int i = 0; i < particles.size(); i++) {
		if (particles[i].lifespan > 200) {
			do_usuniecia.push_back(i);	//dodaje do vektora partikli w kolejce do usuniecia
		}
		else {
			particles[i].update(&particles, i);	//updatuje particle jezeli nie sa do usuniecia
		}
	}
	for (int i = 0; i < generatory.size(); i++) {
		generatory[i].update(&particles);	//update na kazdym generatorze
	}
	for (int i = do_usuniecia.size() - 1; i >= 0; i--) {
		particles.erase(particles.begin() + do_usuniecia[i]);	//usuwa partikle w kolejce do usuniecia
	}
}

void Particle::addForce(ofVec2f wektor,float skalar) {
	sily += wektor/skalar;
}

void Particle::update(vector<Particle>* particles, int i) {
	float da = 1.2041; // gestosc otoczenia
	float dg = 0.8;	//gestosc czasteczek
	float g = 9.81;	//przyspieszenie grawitacyjne
	float wypornosc = da * g*pi*radius;
	sily = sily * 0.83; //wygaszanie sil z czasem


	maxX = ofGetWidth();	//granice obszaru
	maxY = ofGetHeight();	//

	if (podpalona) {
		czas_palenia++;
		colour.r = (254 / max_czas_zycia-1)*czas_palenia;
		colour.g = 0;					//zmiana koloru z czasem
		colour.b = 240-(180/max_czas_zycia-1)*czas_palenia;
		radius = int(max_czas_zycia*5-czas_palenia*2);
		wypornosc = wypornosc / 3;
		for (int i = 0; i < particles->size(); i++) {
			if (particles->at(i).podpalona == false) {
				float dystans = sqrt(powf((p.x - particles->at(i).p.x), 2.0) + powf((p.y - particles->at(i).p.y), 2));
				if (dystans < radius+20) {
					particles->at(i).podpalona = true;	//podpal nastepna czasteczke jezeli jest w zasiegu
				}
			}
		}
		if (czas_palenia > max_czas_zycia){
			lifespan = 500;
		}
	}
	else {
		lifespan++;
		for (int i = 0; i < particles->size(); i++) {
			float dystans = sqrt(powf((p.x - particles->at(i).p.x), 2.0) + powf((p.y - particles->at(i).p.y), 2));
			if (dystans < 50) {
				this->addForce(p - particles->at(i).p, dystans);
				particles->at(i).addForce(particles->at(i).p - p, dystans);	//odepchnij czasteczki jezeli sa blisko siebie
			}
		}
	}
	v.y = -wypornosc / 10;
	if (p.x > maxX || p.x < minX) {
		v *= reverseX;
	}
	if (p.y > maxY || p.y < minY) {		//odbij od sciany
		v *= reverseY;
		sily.y += 50;
	}
	v += sily;
	v = v * 0.99;	//wygaszanie predkosci z czasem
	p += v;
}

void Particle_Generator::update(vector<Particle>* particles) {
	Particle particle;
	particle.setup(this->p.x, this->p.y);	//generowanie czasteczki, zainicjalizowanie wartosci i dodanie do vektora
	particles->push_back(particle); 

	Particle particle2;
	particle.setup(this->p.x, this->p.y);
	particles->push_back(particle);
}

void Particle::draw() {
	ofSetColor(colour);
	ofDrawCircle(p.x, p.y, radius);	//rysuje na ekranie
}

void Particle_Generator::draw() {
	ofSetColor(colour);
	ofDrawRectangle(p,rozmiar.x,rozmiar.y);
}

void ofApp::draw() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();	//narysuj wszystkie czasteczki
	}
	for (int i = 0; i < generatory.size(); i++) {
		generatory[i].draw();	//narysuj wszystkie generatory
	}
	ofSetColor(255);
}

void ofApp::keyPressed(int key) {
	if (key == 100) {
		//d, zdus plomienie
		for (int i = 0; i < particles.size(); i++) {
			if (particles[i].podpalona == true) {
				particles[i].lifespan = 999;
			}
		}
	}
	else if(key == 101) {
		//e
		particles[particles.size() - 1].colour.b = 254;
		particles[particles.size() - 1].colour.r = 0;
		particles[particles.size() - 1].colour.g = 0;
	}
	else if (key == 0xe00E) { //strzalka w prawo, dodaj kolejny generator

		Particle_Generator generator(ofRandom(0, ofGetWidth() - 100), ofRandom(400, 700), 50, 100, 0, &generatory);
	}
	else if(key== 0xe00C){// strzalka w lewo, odejmij pierwszy element vektora generatorow
		generatory.erase(generatory.begin());
	};
}

void ofApp::mouseReleased(int x, int y, int button) {
		Particle particle;
		particle.setup(x, y); \
			particle.podpalona = true;		//tworzenie podpalonej zasteczki
		particles.push_back(particle);
}

Particle_Generator::Particle_Generator(int x, int y, int width, int height, float angle, vector<Particle_Generator>* generatory) {
	p.x = x;
	p.y = y;
	rozmiar.x = width;
	rozmiar.y = height;
	colour.r = 50;
	colour.g = 30;
	colour.b = 100;
	generatory->push_back(*this);
}