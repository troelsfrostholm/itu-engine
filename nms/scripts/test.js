Log("this is the initial log file..");

setGravity(10);

var md2 = new MD2();

md2.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
/*
var root = getRoot();

var node = new GeometryNode(md2);

root.addChild(node);
*/
function Update()
{
    Log("This is a log file");
}