#include <iostream>
#include <falcon/core/FalconDevice.h>
#include <falcon/grip/FalconGripFourButton.h>
#include <falcon/firmware/FalconFirmwareNovintSDK.h>
#include <falcon/util/FalconFirmwareBinaryNvent.h>
#include <falcon/kinematic/stamper/StamperUtils.h>
#include <falcon/core/FalconGeometry.h>
#include <falcon/gmtl/gmtl.h>

#include "falcon/kinematic/FalconKinematicStamper.h"


using namespace std;
using namespace libnifalcon;
using namespace StamperKinematicImpl;

void pause()
{
	cout << "press RETURN" << endl; cin.get();
}

int main()
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
					//Completely close and reopen
					//falcon.close();
					//if(!falcon.open(m_varMap["device_index"].as<int>()))
					//{
					//	std::cout << "Cannot open falcon device index " << m_varMap["device_index"].as<int>() << " - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
					//	return false;
					//}
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
		std::cout << "No firmware loaded to device, and no firmware specified to load (--nvent_firmware, --test_firmware, etc...). Cannot continue" << std::endl;
		//return false;
	}
	else
	{
		//return true;
	}
	if(!firmware_loaded || !falcon.isFirmwareLoaded())
	{
		//std::cout << "No firmware loaded to device, cannot continue" << std::endl;
		//return false;
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
		/*cout << "Falcon Encoder value : ("  
			<<  encoderPos[0] << ", "
			<<  encoderPos[1] << ", "
			<<  encoderPos[2] << ")"
			<< endl;*/
		cout << "Falcon kinematic position : ("  
			<< fixed << vector3[0] - origin[0] << ", "
			<< fixed << vector3[1] - origin[1] << ", "
			<< fixed << vector3[2] - origin[2] << ")"
			<< endl;
	}

	pause();
	return 0;
}