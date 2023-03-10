#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofLight areaLight;
		ofPlanePrimitive plane;
		ofEasyCam camera;
		ofMaterial materialPlane;
		ofMesh logoMesh;
		ofMaterial materialMesh;
		
		int mode = 0;
		float orbitSpeed = 0.0;
		float highwaySpeed = 0.;
		
		float bump = 0.;
		float bumpHeight = 50.0;
};
