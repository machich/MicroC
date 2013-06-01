/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "com.h"
#include "Point.h"
#include "Protocol.h"

#define INSTR_COORD 100
#define INSTR_ISARRIVED 102
#define INSTR_INIT_DONE 120
#define INSTR_GO 121
#define INSTR_RESTART 122
#define INSTR_QUIT 123
#define INSTR_AR 255
#define INSTR_DEST_ADD 1
#define INSTR_DEST_REPLACE 2
#define INSTR_FLUSH 3
#define INSTR_SET_POS 10
#define INSTR_ISBLOCKED 103
#define INSTR_AVOIDING 110
#define INSTR_CONSIGNE 104
#define INSTR_LOG 124
#define INSTR_SEND_PARAMETERS 125
#define INSTR_SEND_PARAMETERS_NAMES 125
#define INSTR_SET_PARAMETERS 50
#define INSTR_ASK_PARAMETERS 51
#define INSTR_PING 254
#define INSTR_SEND_MICROSWITCH 111
#define INSTR_ACTION 60

const float ANGLE_FACTOR = 1000.0;

Protocol protocol;

Comm::Comm(Robot* r) : robot(r)
{
}

uint8_t* Comm::readInt8(uint8_t* data, uint8_t& value)
{
	value = data[0];
	return data + 1;
}

uint8_t* Comm::readInt16(uint8_t* data, short& value)
{
	value = ((short)data[0] << 8) + (short)data[1];
	return data + 2;
}

uint8_t* Comm::readInt32(uint8_t* data, long& value)
{
	value = ((long)data[0] << 24) + ((long)data[1] << 16) + ((long)data[2] << 8) + (long)data[3];
	return data + 4;
}

uint8_t* Comm::readFloat(uint8_t* data, float& value)
{
	long iValue = 0;
	uint8_t* res = readInt32(data, iValue);
	value = *((float*)&iValue);
	return res;
}

uint8_t* Comm::writeInt8(uint8_t* data, uint8_t value)
{
	data[0] = value;
	return data + 1;
}

uint8_t* Comm::writeInt16(uint8_t* data, short value)
{
	data[0] = (uint8_t)((short)value >> 8);
	data[1] = (uint8_t)((short)value % 256);
	return data + 2;
}

uint8_t* Comm::writeInt32(uint8_t* data, long value)
{
	data[0] = (long)value >> 24;
	data[1] = ((long)value >> 16) % 256;
	data[2] = ((long)value >> 8) % 256;
	data[3] = value % 256;
	return data + 4;
}

uint8_t* Comm::writeFloat(uint8_t* data, float value)
{
	long iValue = *((long*)&value);
	uint8_t* res = writeInt32(data, iValue);
	return res;
}

void Comm::sendParameters()
{
	//TODO
	uint8_t data[8*4]; //height parameters on 4 uint8_t
	uint8_t* dataPtr = &(data[0]);
	//dataPtr = writeFloar(dataPtr, 42.2);
	//...

	//protocol.sendMessage(INSTR_SEND_PARAMETERS, 8*4, data);
}

void Comm::sendAR(uint8_t instruction, bool ok)
{
	uint8_t data[2];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt8(dataPtr, instruction);
	dataPtr = writeInt8(dataPtr, ok ? 1 : 0);

	protocol.sendMessage(INSTR_AR, 2, data);

}

void Comm::sendSonars(int ag, int ad, int rg, int rd)
{
	uint8_t data[4];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt8(dataPtr, ag < 255 ? ag : 255);
	dataPtr = writeInt8(dataPtr, ad < 255 ? ad : 255);
	dataPtr = writeInt8(dataPtr, rg < 255 ? rg : 255);
	dataPtr = writeInt8(dataPtr, rd < 255 ? rd : 255);

	protocol.sendMessage(INSTR_AVOIDING, 4, data);
}

void Comm::sendMicroswitch(bool left, bool right)
{
	uint8_t data[2];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt8(dataPtr, left ? 1 : 0);
	dataPtr = writeInt8(dataPtr, right ? 1 : 0);

	protocol.sendMessage(INSTR_SEND_MICROSWITCH, 2, data);
}

void Comm::sendPosition()
{
	uint8_t dir = robot->quelSens();

	short orientation = robot->position.theta * ANGLE_FACTOR;

	uint8_t data[7];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt16(dataPtr, (short)robot->position.x);
	dataPtr = writeInt16(dataPtr, (short)robot->position.y);
	dataPtr = writeInt16(dataPtr, (short)orientation);
	dataPtr = writeInt8(dataPtr, dir);

	protocol.sendMessage(INSTR_COORD, 7, data);
}

void Comm::sendConsigne()
{
	float orientation = robot->pointSuivant.theta;
	orientation *= ANGLE_FACTOR;

	uint8_t data[6];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt16(dataPtr, robot->pointSuivant.x);
	dataPtr = writeInt16(dataPtr, robot->pointSuivant.y);
	dataPtr = writeInt16(dataPtr, orientation);

	protocol.sendMessage(INSTR_CONSIGNE, 6, data);
}

void Comm::sendGo(bool isBlue)
{
	uint8_t data[1];
	uint8_t* dataPtr = &(data[0]);
	dataPtr = writeInt8(dataPtr, isBlue ? 1 : 0);

	protocol.sendMessage(INSTR_GO, 1, data);
}

void Comm::restart()
{
	uint8_t data[0];
	protocol.sendMessage(INSTR_RESTART, 0, data);
}

void Comm::quit()
{
	uint8_t data[0];
	protocol.sendMessage(INSTR_QUIT, 0, data);
}

void Comm::sendIsArrived()
{
	uint8_t data[0];
	protocol.sendMessage(INSTR_ISARRIVED, 0, data);
}

void Comm::sendIsBlocked()
{
	uint8_t data[0];
	protocol.sendMessage(INSTR_ISBLOCKED, 0, data);
}

void Comm::sendLog(const String& text)
{
	int len = text.length();
	char data[len];
	text.toCharArray(data, len);

	protocol.sendMessage(INSTR_LOG, len, (uint8_t*)data);
}

void Comm::sendParameterNames()
{
	//TO COMPLETE
	String names = "Position Kp;;"
			"Position Kd;;"
			"Angle Kd;;"
			"Angle Kp;;"
			"coeff1;;"
			"coeff2";

	int len = names.length();
	char data[len];
	names.toCharArray(data, len);

	protocol.sendMessage(INSTR_SEND_PARAMETERS_NAMES, len, (uint8_t*)data);
}

bool Comm::process_message(uint8_t data[], uint8_t instruction, uint8_t length)
{
	bool ok = false;

	if (instruction == INSTR_DEST_ADD && length == 10)
	{
		//add a point
		Point p;
		short x, y, theta;
		uint8_t typeAsserv;
		uint8_t typeDeplacement;
		uint8_t speed;

		data = readInt16(data, x);
		data = readInt16(data, y);
		data = readInt16(data, theta);
		data = readInt8(data, typeAsserv);
		data = readInt8(data, typeDeplacement);
		data = readInt8(data, speed);

		p.x = x;
		p.y = y;
		p.theta = theta / ANGLE_FACTOR;
		p.typeAsserv = (Point::TypeAsserv)typeAsserv;
		p.pointArret = data[9] == 1;
		p.vitessMax = (float) speed;
		p.typeDeplacement = (Point::TypeDeplacement) typeDeplacement;

                if (p.typeDeplacement == Point::TournePuisAvance)
                {
                  p.pointArret = true;
                }

		robot->ajoutPoint(p);
		ok = true;
		Serial.print("AddPos received : ");
		Serial.println(x);
		Serial.println(y);
		Serial.println(p.theta);
		Serial.println((Point::TypeAsserv)typeAsserv);
		Serial.println(p.pointArret);
                Serial.println(p.typeDeplacement);
	}
	else if (instruction == INSTR_SET_POS && length == 6)
	{
		//set the start point
		short x, y, thetaInt;

		data = readInt16(data, x);
		data = readInt16(data, y);
		data = readInt16(data, thetaInt);
		float theta = thetaInt / ANGLE_FACTOR;

		Point p;
		p.x = x;
		p.y = y;
		p.theta = theta;

		robot->teleport(p);
		ok = true;
		Serial.print("SetPos received : ");
		Serial.print(x);
		Serial.print(y);
		Serial.println(p.theta);
	}
	else if (instruction == INSTR_FLUSH)
	{
		//stop the robot and flush the remaining list of point
		robot->stop();
		ok = true;
		Serial.println("Flush received");
	}
	else if (instruction == INSTR_ACTION && length == 3)
	{
		uint8_t actionType;
		uint8_t parameter;
		data = readInt8(data, parameter);
		data = readInt8(data, actionType);
                
                switch (parameter)
                {
                  case (0) :
                    robot->MAJContaineur(true, actionType);
                  break;
                  case (1) :
                    robot->MAJContaineur(false, actionType);
                  break; 
                  case (2) :
                    robot->MAJContaineur(true, actionType);
                    robot->MAJContaineur(false, actionType);
                  break; 
                }
                
                

	}
	else if (instruction == INSTR_SET_PARAMETERS)
	{
		Serial.println("Set parameters");
		//TODO
		uint8_t nbParameters = 0;
		data = readInt8(data, nbParameters);
		for(int i = 0; i < nbParameters; ++i)
		{
			float value;
			//data = readParameter(data, value);
		}
		ok = true;
	}
	else if (instruction == INSTR_ASK_PARAMETERS)
	{
		Serial.println("Ask parameters");
		sendParameters();
		ok = true;
	}
	else if (instruction == INSTR_PING)
	{
		Serial.println("PING");
                robot->_pingReceived = true;
		ok = true;
	}

	return ok;
}

void Comm::comm_read()
{
	if (protocol.read())
	{
		bool ok = process_message(protocol.getData(), protocol.getInstruction(), protocol.getLength());
		sendAR(protocol.getInstruction(), ok);
		comm_read();
	}
}