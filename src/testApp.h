#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "tile.h"
#include "ofxVectorMath.h"

struct Player {
    ofxVec2f position;
    ofxVec2f v;
    bool onFloor;
    int tile;
};


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
    vector<Tile> tiles;
    Player player;
    
    ofxVec2f keyForce;
    
    bool debug;

};

#endif
