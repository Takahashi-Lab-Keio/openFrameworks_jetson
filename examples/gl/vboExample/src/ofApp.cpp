#include "ofApp.h"

/*
 This code demonstrates the difference between using an ofMesh and an ofVboMesh.
 The ofMesh is uploaded to the GPU once per frame, while the ofVboMesh is
 uploaded once. This makes it much faster to draw multiple copies of an
 ofVboMesh than multiple copies of an ofMesh.
 */

/*
 These functions are for adding quads and triangles to an ofMesh -- either
 vertices, or texture coordinates.
 */
//--------------------------------------------------------------
void ofApp::addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void ofApp::addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
void ofApp::addTexCoords(ofMesh& mesh, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

//--------------------------------------------------------------
void ofApp::addTexCoords(ofMesh& mesh, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d) {
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}

/*
 The 3d data is stored in an image where alpha represents depth. Here we create
 a 3d point from the current x,y image position.
 */
//--------------------------------------------------------------
glm::vec3 ofApp::getVertexFromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) {
		float z = ofMap(color.a, 0, 255, -480, 480);
		return glm::vec3(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return glm::vec3(0, 0, 0);
	}
}

//--------------------------------------------------------------
void ofApp::setup() {

	#ifdef TARGET_OPENGLES
	// While this will will work on normal OpenGL as well, it is
	// required for OpenGL ES because ARB textures are not supported.
	// If this IS set, then we conditionally normalize our
	// texture coordinates below.
	ofEnableNormalizedTexCoords();
	#endif

	img.load("linzer.png");

	// OF_PRIMITIVE_TRIANGLES means every three vertices create a triangle
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 10;	// this controls the resolution of the mesh

	int width = img.getWidth();
	int height = img.getHeight();

	glm::vec2 imageSize(width,height);

	glm::vec3 zero(0, 0, 0);
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			/*
			 To construct a mesh, we have to build a collection of quads made up of
			 the current pixel, the one to the right, to the bottom right, and
			 beneath. These are called nw, ne, se and sw. To get the texture coords
			 we need to use the actual image indices.
			 */
			glm::vec3 nw = getVertexFromImg(img, x, y);
			glm::vec3 ne = getVertexFromImg(img, x + skip, y);
			glm::vec3 sw = getVertexFromImg(img, x, y + skip);
			glm::vec3 se = getVertexFromImg(img, x + skip, y + skip);
			glm::vec2 nwi(x, y);
			glm::vec2 nei(x + skip, y);
			glm::vec2 swi(x, y + skip);
			glm::vec2 sei(x + skip, y + skip);

			// ignore any zero-data (where there is no depth info)
			if(nw != zero && ne != zero && sw != zero && se != zero) {
				addFace(mesh, nw, ne, se, sw);

				// Normalize our texture coordinates if normalized
				// texture coordinates are currently enabled.
				if(ofGetUsingNormalizedTexCoords()) {
					nwi /= imageSize;
					nei /= imageSize;
					sei /= imageSize;
					swi /= imageSize;
				}

				addTexCoords(mesh, nwi, nei, sei, swi);
			}
		}
	}

	vboMesh = mesh;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(64), ofColor(0));
	cam.begin();
	ofEnableDepthTest();

	ofRotateYDeg(ofGetElapsedTimef() * 30); // slowly rotate the model

	ofScale(1, -1, 1); // make y point down
	ofScale(.5, .5, .5); // make everything a bit smaller

	img.bind(); // bind the image to begin texture mapping
	int n = 5; // make a 5x5 grid
	glm::vec2 spacing(img.getWidth(), img.getHeight()); // spacing between meshes
	ofTranslate(-spacing.x * n / 2, -spacing.y * n / 2, 0); // center the grid
	for(int i = 0; i < n; i++) { // loop through the rows
		for(int j = 0; j < n; j++) { // loop through the columns
			ofPushMatrix();
			ofTranslate(i * spacing.x, j * spacing.y); // position the current mesh
			ofTranslate(spacing.x / 2, spacing.y / 2); // center the mesh
			if(ofGetKeyPressed()) {
				vboMesh.draw(); // draw a vboMesh (faster) when a key is pressed
			} else {
				mesh.draw(); // draw an ofMesh (slower) when no key is pressed
			}
			ofPopMatrix();
		}
	}
	img.unbind();

	ofDisableDepthTest();
	cam.end();

	// draw the framerate in the top left corner
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	ofDrawBitmapString("Hold any key for ofVboMesh mode.", 10, 40);
	string mode = (ofGetKeyPressed() ? "ofVboMesh" : "ofMesh");
	ofDrawBitmapString("Current mode: " + mode, 10, 60);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == ' ') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
