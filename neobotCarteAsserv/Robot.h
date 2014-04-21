#ifndef ROBOT_H
#define ROBOT_H
#include <Arduino.h>
#include <Servo.h>
#include "Pid.h"
#include "Consigne.h"
#include "Point.h"
#include "QueueList.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"	// color sensors
#include "Parameters.h"

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */
 
/*
 * Project : Neobot
 * Version : 0.42
 * Date : 30/12/2012
 * Author : Neobot
 */


class Robot
{
  public:
	Robot(Adafruit_TCS34725 *colorSensor1, Adafruit_TCS34725 *colorSensor2, float periodAsserv = 5.0, float x = 0.0, float y = 0.0, float theta = 0.0);
  
    enum TypeDeplacement
    {
        TournePuisAvance = 0,
        TourneEtAvance = 1,
        TourneSeulement = 2,
        AvanceSeulement = 3,
    };
    
    enum TypeAsserv
    {
        Aucun = 0,
        EnAvant = 1,
        EnArriere = 2,
        Auto = 13,
    };

	enum ColorSensorStatus
	{
		ColorUnchanged = 0,
		ColorNothing = 1,
		ColorYellow = 2,
		ColorRed = 3,
	};
  
    void teleport(Point point);
    void forceObjectif(Point point);
    void ajoutPoint(Point point);
    void ajoutPoint(float x, float y, bool pointArret = false, int typeDeplacement =1, float vitessMax = 100);
    void flush();
    void stop();
    void majPosition(float pasRoueGauche, float pasRoueDroite);
    void calculConsigne();
    void calculCommande();
    float filtreCommandeRoue(float value);
    void avanceDe(float avance, bool avecFreinage = true, float vitessMax = VITESSE_MAX); // en mm
    void tourneDe(float rotation, bool avecFreinage = true, float vitessMax = VITESSE_MAX_ROT); // en rad
    void vaEnXY(float x, float y, bool estPointArret = true, float vitessMax = VITESSE_MAX);
    bool passageAuPointSuivant();
    void vaVersPointSuivant();
    bool estArrive();
    bool quelSens();
	void attend(unsigned long attente); // tps en milliseconde
    bool estEnAttente();
    void stopAttente();
	void enableColorSensor(int sensorId);
	void disableColorSensor(int sensorId);
	bool isColorSensorEnabled(int sensorId);
	int readColorSensor(int sensorId);
	void startPump(int pumpId);
	void stopPump(int pumpId);

    float pasPrecendentGauche;
    float pasPrecendentDroit;
    
    PID _pidDist;
    PID _pidOrientation;
    
    Consigne _consigneDist;
    Consigne _consigneOrientation;

    QueueList<Point> queue;
    Point pointSuivant;
    Point position;
    
    float _periodAsserv;
    
    TypeDeplacement _typeDeplacement;
    TypeAsserv _typeAsserv;
  
	float _deltaDistMm;
	float _deltaOrientRad;
      
    float _thetaTotal; // angle non borné

	int _commandeRoueGauche;
	int _commandeRoueDroite;
    
    unsigned long tempsAttenteDeplacement;
	unsigned long debutAttenteDeplacement;

	bool _pingReceived;

private:
    bool _tourneFini;

	bool _colorSensorEnabled[2];
	int _colorSensorStatus[2];
	Adafruit_TCS34725* _colorSensor[2];

};

#endif // ROBOT_H

