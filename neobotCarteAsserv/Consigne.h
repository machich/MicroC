#ifndef CONSIGNE_H
#define CONSIGNE_H

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

const float COMMANDE_MOTEUR_MAX = 32767.0;
const float DIST_ARRIVE_AVEC_FREINAGE = 5.0;
const float DIST_ARRIVE_SANS_FREINAGE = 5.0;

class Consigne
{
    public:
	  Consigne(float vmax, float amax, float periodeConsigne, float dccCoeff, float distArrive = DIST_ARRIVE_AVEC_FREINAGE); // 5 mm d'arriv√©e
		
      float calculConsigne(float deltaDistRealise);
      void setDemande(float dist, bool freinage = true);
      void setVmaxParcourt(float vmax);
      void setAmaxParcourt(float amax);
      void majDistAccDcc();
      void majConsigneMax();
      void majVariationConsigneMax();
      float transformeDeltaDistanceEnConsigne(float delta);
      float transformeConsigneEnVitesse(float consigne);
      bool estArrive();
	  bool etaitArrive();
      bool calcEstArrive();
	  void reset();
      
      float _consigne;
      
      float _distArrive;
      
	  enum Phase{Transitoire_acc = 0, Stationaire = 1, Arrive = 2, Transitoire_dcc = 4};
      
      float _vitessMax;
      float _vitessMaxParcourt;
      float _vitessCourrante;
      float _accelerationMaxParcourt;
      float _periodeMajConsigne;
      
      float _consignePrec;

      float _distDemande;
      
      float _sens;
      
      float _distAcc;
      float _distDcc;
      bool doitFreiner;

      float _consigneMax;
      float _variationConsigneMax;
          
      Phase _phase;
	  Phase _phasePrec;
      
      float _dccCoeff;
	  float _dccAugmetationDcc;
};

#endif // CONSIGNE_H


