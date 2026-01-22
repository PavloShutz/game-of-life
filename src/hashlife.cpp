/*
* This class represents a Node in a quadtree data structure
* used to implement HashLife algorithm.
*/
class Node {
public:
	Node(Node* sw = nullptr, Node* se = nullptr,
			 Node* nw = nullptr, Node* ne = nullptr)
		: _sw(sw), _se(se), _nw(nw), _ne(ne) { }

private:
	Node* _sw = nullptr;
	Node* _se = nullptr;
	Node* _nw = nullptr;
	Node* _ne = nullptr;
};