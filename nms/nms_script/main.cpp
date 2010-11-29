#include "nms_script.h"

int main(int argc, char* argv[])
{   
	engine = NMS_Framework();
	engine.NMSInit(800, 600, 16, "Demo 2", false);
	nms_script s;
	//s.Run("test.js");
	s.RunScripts();
	return 0;
}