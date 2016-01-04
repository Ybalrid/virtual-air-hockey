#include <iostream>

//novint falcon includes
#include <hdl/hdl.h>
#include <hdlu/hdlu.h>

using namespace std;

//pause console
void pause()
{
	std::cout << "Press RETURN to continue" << std::endl;
	std::cin.get();
}

class HapticClass;	

HDLServoOpExitCode ContactCB(void *data)
{
	//cout << "contact" << endl;
	//HapticClass *l_pHapticData = static_cast< HapticClass * >( data );

	double position[3];
	//hdlToolPosition(position);

	//cout << "Position : (" << position[0] << ", " << position[1] << ", " << position[2] << ")" << endl;

	return HDL_SERVOOP_CONTINUE;
}


class HapticClass { 
public:
	friend HDLServoOpExitCode ContactCB(void *data);
HDLOpHandle m_hContactCB; 
};


int main(void)
{

	cout << "Init Haptic device 0" << endl;
	HDLDeviceHandle hHandle = hdlInitNamedDevice((const char*)0);
	cout << "call hdlStart()" << endl;
	hdlStart();
	cout << "Make device 0 current" << endl;
	hdlMakeCurrent(hHandle);
	cout << "get application worspace dimentions" << endl;
	double appWorkspaceDims[6];
	hdlDeviceWorkspace(appWorkspaceDims); //supplied by client app

	std::cout << "app worspace content : " << std::endl;
	for(unsigned char i(0); i < 6; i++)
		cout << appWorkspaceDims[i] << endl;

	//HDLOpHandle op = hdlCreateServoOp(ContactCB, nullptr, false);


	while(true)
	{
			double position[3];
	hdlToolPosition(position);

	cout << "Position : (" << position[0] << ", " << position[1] << ", " << position[2] << ")" << endl;

	}

	pause();
	return 0;
}