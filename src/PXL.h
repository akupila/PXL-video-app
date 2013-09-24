//
//  PXL.h
//  PXL
//
//  Created by Antti Kupila on 9/23/13.
//
//

#pragma once

#include "ofMain.h"

using namespace std;

class PXL : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	
private:
	bool openFile();
	void save();

	bool guiVisible;
	
	ofFile output;
	
	ofVideoPlayer videoPlayer;
	ofImage frame;
	bool loadVideoButtonPressed;
	string videoPath;
	
	int width;
	int height;
	int layout;
	bool videoPlaying;
	int speed;
	int prevSpeed;
	
	bool saving;
	int currentFrame;
};
