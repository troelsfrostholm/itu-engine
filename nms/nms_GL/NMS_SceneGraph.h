#include <vector>

using namespace std;


	class __declspec(dllexport) SceneGraphNode
	{
	private:
		vector<SceneGraphNode> children;

	public:
		SceneGraphNode();
		void traverse();
	};

	class TransformationNode : public SceneGraphNode
	{

	};

	class GeometryNode : public SceneGraphNode
	{

	};