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
		if (_result) return _result;

		if (_level == 2) {
			return _result;
		}
		else {
			// Find the 9 overlapping sub-squares
			Node* nw, * nn, * ne, * ww, * cc, * ee, * sw, * ss, * se;
			nw = find_nw(this); nn = find_nn(this); ne = find_ne(this);
			ww = find_ww(this); cc = find_cc(this); ee = find_ee(this);
			sw = find_sw(this); ss = find_ss(this); se = find_se(this);

			// Calculate their results (first generation)
			nw = nw->nextGeneration(); nn = nn->nextGeneration(); ne = ne->nextGeneration();
			ww = ww->nextGeneration(); cc = cc->nextGeneration(); ee = ee->nextGeneration();
			sw = sw->nextGeneration(); ss = ss->nextGeneration(); se = se->nextGeneration();

			// Construct intermediate overlapping squares and calculate (second generation)
			nw = find(nw, nn, ww, cc)->nextGeneration();
			ne = find(nn, ne, cc, ee)->nextGeneration();
			sw = find(ww, cc, sw, ss)->nextGeneration();
			se = find(cc, ee, ss, se)->nextGeneration();

			// Memoize and return the result
			_result = find(nw, ne, sw, se);
			return _result;
		}
	}

private:
	Node* _sw = nullptr;
	Node* _se = nullptr;
	Node* _nw = nullptr;
	Node* _ne = nullptr;

	Node* _result = nullptr;  // Cached result for this node
	int _level = 0;

	static Node _nd_0[2];      // Level 0: 2 states (dead/alive)
	static Node _nd_1[16];     // Level 1: 16 states (2x2 grid)
	static Node _nd_2[256];    // Level 2: 256 states (4x4 grid)

private: // search facilities
	static Node* find_nw(Node* sq) { return sq->_nw; }
	static Node* find_ne(Node* sq) { return sq->_ne; }
	static Node* find_sw(Node* sq) { return sq->_sw; }
	static Node* find_se(Node* sq) { return sq->_se; }

	static Node* find_nn(Node* sq) {
		return find(sq->_nw->_ne, sq->_ne->_nw,
			sq->_nw->_se, sq->_ne->_sw);
	}
	static Node* find_ww(Node* sq) {
		return find(sq->_nw->_sw, sq->_nw->_se,
			sq->_sw->_nw, sq->_sw->_ne);
	}
	static Node* find_cc(Node* sq) {
		return find(sq->_nw->_se, sq->_ne->_sw,
			sq->_sw->_ne, sq->_se->_nw);
	}
	static Node* find_ee(Node* sq) {
		return find(sq->_ne->_sw, sq->_ne->_se,
			sq->_se->_nw, sq->_se->_ne);
	}
	static Node* find_ss(Node* sq) {
		return find(sq->_sw->_ne, sq->_se->_nw,
			sq->_sw->_se, sq->_se->_sw);
	}
	// Hash-consing: find or create a node with given children
	static Node* find(Node* nw, Node* ne, Node* sw, Node* se);

private: // GoL rules
	static bool life_rule(Node* nw, Node* nn, Node* ne,
		Node* ww, Node* cc, Node* ee,
		Node* sw, Node* ss, Node* se) {
		unsigned count = alive(nw) + alive(nn) + alive(ne)
			+ alive(ww) + alive(ee)
			+ alive(sw) + alive(ss) + alive(se);
		return (count == 2 ? alive(cc) : count == 3);
	}

	static bool alive(Node* nd) { return nd - _nd_0; }

private: // init facilities
	static void init_nd_0();
	static void init_nd_1();
	static void init_nd_2();
};