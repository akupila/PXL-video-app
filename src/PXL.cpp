//
//  PXL.cpp
//  PXL
//
//  Created by Antti Kupila on 9/23/13.
//
//

#include "PXL.h"

#include "ofxSimpleGuiToo.h"

void PXL::setup()
{
	guiVisible = true;
	layout = 0;
	
	gui.addButton("Load video", loadVideoButtonPressed);
	gui.addSlider("Target width", width, 1, 32);
	gui.addSlider("Target height", height, 1, 16);
	gui.addSlider("Video speed %", speed, 50, 200);
	
	gui.loadFromXML();
	
	gui.show();
}

void PXL::update()
{
	if (loadVideoButtonPressed)
	{
		openFile();
		loadVideoButtonPressed = false;
	}
	
	if (saving)
	{
		videoPlayer.setFrame(currentFrame);
	}
	
	if (videoPlayer.isLoaded())
	{
		if (prevSpeed != speed)
		{
			videoPlayer.setSpeed(speed / 100.0f);
			prevSpeed = speed;
		}
		videoPlayer.update();
		
		frame.setFromPixels(videoPlayer.getPixelsRef());
		frame.resize(width, height);
	}
	
	if (saving)
	{
		int red = 255;
		int green = 0;
		int blue = 0;
		switch (layout)
		{
			case 0:
			{
				for (int x = 0; x < width; x++)
				{
					for (int y = height - 1; y >= 0; y--)
					{
						ofColor color = frame.getColor(x, y);
						output.write((char *)&color.r, 1);
						output.write((char *)&color.g, 1);
						output.write((char *)&color.b, 1);
					}
				}
				break;
			}
		}
		
		if (++currentFrame >= videoPlayer.getTotalNumFrames())
		{
			saving = false;
			output.close();
		}
	}
}

void PXL::draw()
{
	float aspectRatio = (float)videoPlayer.width / videoPlayer.height;
	int videoWidth = 320;
	int videoHeight = videoWidth / aspectRatio;
	
	ofBackground(16);
	
	if (saving)
	{
		ofColor(32, 32, 32);
	}
	else
	{
		ofColor(255, 255, 255);
	}
	videoPlayer.draw(0, 0, videoWidth, videoHeight);
	
	float xScale = (float)width / videoPlayer.width;
	float yScale = (float)height / videoPlayer.height;
	
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(0, videoHeight);
	float pixelWidth = videoWidth / (float)width;
	float pixelHeight = videoHeight / (float)height;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			ofColor color = frame.getColor(x, y);
			ofSetColor(color);
			ofCircle((x + 0.5) * pixelWidth, (y + 0.5) * pixelHeight, min(pixelWidth, pixelHeight) * 0.5f * 0.95f);
		}
	}
	ofPopStyle();
	ofPopMatrix();
	
	if (saving)
	{
		ofBackground(16);
		string str = "Saving " + ofToString(currentFrame * 100.0f / videoPlayer.getTotalNumFrames(), 1) + "%..";
		ofColor(32);
		videoPlayer.draw(0, 0, videoWidth, videoHeight);
		ofColor(255);
		ofDrawBitmapString(str, 20, 20);
	}
	else
	{
		gui.draw();
	}
}

void PXL::keyReleased(int key)
{
	if (saving) return;
	switch (key)
	{
		case 'o':
			openFile();
			break;
		case OF_KEY_TAB:
			guiVisible = !guiVisible;
			if (guiVisible)
			{
				gui.show();
			}
			else
			{
				gui.hide();
			}
			break;
		case ' ':
			videoPlaying = !videoPlaying;
			if (videoPlaying)
			{
				videoPlayer.play();
			}
			else
			{
				videoPlayer.stop();
			}
			break;
		case 's':
			save();
			break;
	}
}

void PXL::mouseMoved(int x, int y)
{
	if (videoPlayer.isLoaded() && !gui.isOn() && !videoPlaying)
	{
		float xRatio = (float)x / ofGetWidth();
		if (xRatio < 0) xRatio = 0;
		if (xRatio > 1.0f) xRatio = 1.0;
		
		int frameNum = xRatio * videoPlayer.getTotalNumFrames();
		videoPlayer.setFrame(frameNum);
	}
}

#pragma Private

bool PXL::openFile()
{
	ofFileDialogResult result = ofSystemLoadDialog("Select video file");
	if (result.bSuccess)
	{
		cout << "Success: " << result.getPath() << endl;
		bool videoOk = videoPlayer.loadMovie(result.getPath()) && videoPlayer.getTotalNumFrames() > 1;
		
		if (!videoOk)
		{
			ofSystemAlertDialog("That's not a valid video file");
			videoPlayer.close();
		}
		else
		{
			videoPlayer.setVolume(0.0f);
			videoPlayer.firstFrame();
			videoPlayer.setLoopState(OF_LOOP_NORMAL);
			currentFrame = 0;
			return true;
		}
	}
	return false;
}

void PXL::save()
{
	if (!videoPlayer.isLoaded()) return;
	
	ofFileDialogResult result = ofSystemSaveDialog("video.pxl", "Save video to SD card");
	if (result.bSuccess)
	{
		output.open(result.getPath(), ofFile::WriteOnly, true);
		
		uint16_t numFrames = videoPlayer.getTotalNumFrames();
		uint8_t frameRate = speed / 100.0f / (videoPlayer.getDuration() / videoPlayer.getTotalNumFrames());
		
		cout << "Frame rate: " << (int)frameRate << endl;
		
		cout << "FRAMES: " << numFrames << endl;

		output << "PXL v" << 1 << " | " << __DATE__ << " " << __TIME__ << endl;
		output.write((char *)&width, 1);
		output.write((char *)&height, 1);
		output.write((char *)&numFrames, 2);
		output.write((char *)&layout, 1);
		output.write((char *)&frameRate, 1);
		output << endl;
		
		saving = true;
		
		videoPlayer.stop();
	}
	
}