#include <vector>

using namespace std;

namespace nms
{
	class SceneGraphNode
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
};