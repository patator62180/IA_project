#include "BehaviourTree.h"
#include "../BT/Node.h"
#include "../BT/Selector.h"
#include "../BT/Sequencer.h"
#include "../BT/Leaf/Deplacement.h"
#include "../BT/Leaf/NbTiles.h"


BehaviourTree::BehaviourTree() {

	NbTiles* nbTiles = new NbTiles(9);
	Deplacement* dep1 = new Deplacement(Tile::NW);
	//Deplacement* dep2 = new Deplacement(Tile::SW);
	//Sequencer* seq = new Sequencer();
	//seq->children.push_back(nbTiles);
	//seq->children.push_back(dep1);
	//root.children.push_back(seq);
	//root.children.push_back(dep2);

}