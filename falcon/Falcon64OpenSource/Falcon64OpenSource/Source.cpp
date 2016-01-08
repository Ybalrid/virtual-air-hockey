#include <iostream>
#include <Windows.h>

#include "FalconController.hpp"

using namespace std;

void pause()
{
	cout << "press RETURN" << endl; cin.get();
}

FalconController::FalconVect3 testForce = {0, 0, 20};

class tester
{
public:
	tester()
	{}
	void update()
	{
		cout << "hello world" << endl;
	}
};

DWORD WINAPI threadFunction(LPVOID address)
{
	tester* object = static_cast<tester*>(address);
	while(true) object->update();
}

int main()
{
	cout.precision(8);
	auto Falcon(new FalconController);
	pause();

	Falcon->startUpdateThread();

	while(true)
	{
		//system("cls");
		FalconController::FalconVect3 v =  Falcon->getPosition();
		cout << "Position : (" 
			<< fixed << v[0] << ", " 
			<< fixed << v[1] << ", " 
			<< fixed << v[2] << ")"
			<< endl;

		/*if(Falcon->getButtonState(FalconController::FalconGripButton::PRINCIPAL))
		Falcon->setForce(testForce);
		else
		Falcon->setZeroForce();*/

		FalconController::FalconVect3 force = {0, 0, 0};
		force[2] = (+0.07-v[2])*50;
		Falcon->setForce(force);

		cout << "Spring extent : " << fixed <<  0.02-v[2] << endl;
		cout << "Force : F=(" << fixed << force[0] << ", " << fixed << force[1] << ", " << fixed << force[2] << ")"  << endl;

		//Sleep((1.0f/60.0f)*1000);//simulate DK1 framerate
		Falcon->setLED(FalconController::RED,Falcon->getButtonState(FalconController::LEFT));
		Falcon->setLED(FalconController::GREEN,Falcon->getButtonState(FalconController::UP));
		Falcon->setLED(FalconController::BLUE,Falcon->getButtonState(FalconController::RIGHT));

		//Falcon->update();
		Sleep(1000/60);
	}
//	else
	//	cout << "The novint Falcon hasn't been initialized properly. Check USB connexion " << endl;

	tester* obj = new tester;

	CreateThread(NULL, 0, threadFunction, obj, 0, NULL);
	pause();

	delete Falcon;
	return 0;
}

int oldmain()
{
	cout.precision(5);

	FalconDevice falcon;
	falcon.setFalconFirmware<FalconFirmwareNovintSDK>();

	cout << "Setting up comm interface for Falcon comms" << endl;

	unsigned int count;
	falcon.getDeviceCount(count);
	cout << "Connected Device Count: " << count << endl;

	//Open the device number:
	int deviceNum = 0;
	cout << "Attempting to open Falcon device:  " << deviceNum << endl;
	if(!falcon.open(deviceNum))
	{
		cout << "Cannot open falcon device index " << deviceNum << " - Lib Error Code: " << falcon.getErrorCode() << " Device Error Code: " << falcon.getFalconComm()->getDeviceErrorCode() << endl;
		return false;
	}
	else
	{
		cout << "Connected to Falcon device " << deviceNum << endl ;
	}



	bool skip_checksum = false;
	//See if we have firmware
	bool firmware_loaded = false;
	firmware_loaded = falcon.isFirmwareLoaded();
	if(!firmware_loaded)
	{
		std::cout << "Loading firmware" << std::endl;
		uint8_t* firmware_block;
		long firmware_size;
		{

			firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE);
			firmware_size = NOVINT_FALCON_NVENT_FIRMWARE_SIZE;


			for(int i = 0; i < 10; ++i)
			{
				if(!falcon.getFalconFirmware()->loadFirmware(skip_checksum, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))

				{
					cout << "Firmware loading try failed";
				}
				else
				{
					firmware_loaded = true;
					break;
				}
			}
		}
	}
	else if(!firmware_loaded)
	{
		std::cout << "No firmware loaded to device" << std::endl;
	}
	std::cout << "Firmware loaded" << std::endl;

	//Seems to be important to run the io loop once to be sure of sensible values next time:
	falcon.runIOLoop();

	falcon.getFalconFirmware()->setHomingMode(true);
	falcon.setFalconKinematic<libnifalcon::FalconKinematicStamper>();
	falcon.setFalconGrip<FalconGripFourButton>();


	std::array<double, 3> origin;
	falcon.getFalconKinematic()->getWorkspaceOrigin(origin);
	cout << "Origin is : ";
	cout << fixed << origin[0] << ", "
		<< fixed << origin[1] << ", "
		<< fixed << origin[2] << ")"
		<< endl;
	pause();

	cout << "led should be red" << endl;
	int led(0);
	led |= FalconFirmware::RED_LED;
	falcon.getFalconFirmware()->setLEDStatus(led);
	falcon.runIOLoop();
	pause();

	falcon.getFalconFirmware()->setHomingMode(true);
	falcon.runIOLoop();
	pause();

	array<double, 3> zero = {0, 0, 0};
	array<double, 3> force;
	force[0] = 0;
	force[1] = 0;
	force[2] = 9.81*4;
	std::array<int, 3> encoderPos;
	std::array<double, 3> vector3;

	while(true)
	{
		falcon.runIOLoop();
		if(falcon.getFalconGrip()->getDigitalInput(2))
			falcon.setForce(force);
		else
			falcon.setForce(zero);
		encoderPos = falcon.getFalconFirmware()->getEncoderValues();
		falcon.getFalconKinematic()->getPosition(encoderPos, vector3);

		cout << "Falcon kinematic position : ("  
			<< fixed << vector3[0] - origin[0] << ", "
			<< fixed << vector3[1] - origin[1] << ", "
			<< fixed << vector3[2] - origin[2] << ")"
			<< endl;
	}

	pause();
	return 0;
}