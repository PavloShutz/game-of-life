/*
* This class represents a Node in a quadtree data structure
* used to implement HashLife algorithm.
*/
class Node {
public:
	Node(Node* sw = nullptr, Node* se = nullptr,
			 Node* nw = nullptr, Node* ne = nullptr)
		: _sw(sw), _se(se), _nw(nw), _ne(ne) { }

	Node* nextGeneration() {
		Node* cached = find(this);
		if (cached != nullptr) return cached;

		if (_level == 2) {
			return add(this, computeBase());
		}
		else {
			return new Node(_nw->nextGeneration(), _ne->nextGeneration(),
				_sw->nextGeneration(), _se->nextGeneration());
		}
	}

private:
	Node* _sw = nullptr;
	Node* _se = nullptr;
	Node* _nw = nullptr;
	Node* _ne = nullptr;

	Node* _result = nullptr;  // Cached result for this node
	int _level = 0;

private:
	Node* unite(Node* sw, Node* se, Node* nw, Node* ne) {
		Node* temp = new Node(sw, se, nw, ne);
		temp->_level = _level - 1;
		return temp->nextGeneration();
	}

	Node* find(Node* key);
	Node* add(Node* key, Node* result);

	Node* computeBase();
};