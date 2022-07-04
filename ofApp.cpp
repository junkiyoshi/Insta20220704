#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(24);
	ofBackground(0);
	ofSetWindowTitle("openFrameworks");

	ofSetCircleResolution(72);
	this->fbo.allocate(ofGetWidth(), ofGetHeight() / 2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);

	// Draw Sun To Fbo
	ofPushMatrix();
	ofTranslate(this->fbo.getWidth() / 3, this->fbo.getHeight() / 5 * 2);

	ofSetColor(255, 255, 0);
	ofDrawCircle(glm::vec3(), 100);

	ofPopMatrix();

	// Draw Rain
	ofSetColor(255);
	for (int i = 0; i < 50; i++) {

		int x = ofRandom(ofGetWidth());
		int y = (int)(ofRandom(this->fbo.getHeight()) + ofGetFrameNum() * ofRandom(10, 30)) % (int)this->fbo.getHeight();

		ofDrawLine(x, y, x, y + ofRandom(10, 30));
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	// Draw Fbo 
	ofSetColor(255, 255);
	this->fbo.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);

	// Draw Shadow
	ofPixels pixels;
	this->fbo.readToPixels(pixels);

	for (int y = 0; y < pixels.getHeight(); y++) {

		int noise_x = ofMap(ofNoise(y * 0.05 + ofGetFrameNum() * 0.05), 0, 1, -20, 20);
		noise_x = noise_x * ofMap(y, 0, pixels.getHeight(), 1, 0.1);

		if (noise_x > 0) {

			for (int x = pixels.getWidth() - 1; x > noise_x; x--) {

				pixels.setColor(x, y, pixels.getColor(x - noise_x, y));
			}
			for (int x = 0; x < noise_x; x++) {

				pixels.setColor(x, y, ofColor(39));
			}
		}
		else if (noise_x < 0) {

			for (int x = 0; x < pixels.getWidth() + noise_x; x++) {

				pixels.setColor(x, y, pixels.getColor(x - noise_x, y));
			}
			for (int x = pixels.getWidth() + noise_x; x < pixels.getWidth(); x++) {

				pixels.setColor(x, y, ofColor(39));
			}
		}

	}
	ofImage image;
	image.setFromPixels(pixels);

	ofRotate(180);
	ofRotateY(180);
	ofSetColor(255, 64);
	image.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);

	ofDrawLine(-360, 0, 360, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}