#include "NMS_Framework.h"
#include "MD2Model.h"
#include <cmath>

#define WIDTH  640
#define HEIGHT  480
NMS_Framework engine = NMS_Framework();

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);
	engine.run();
	return 0;
}