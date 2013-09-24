PXL-video-app
=============

An openframeworks app for creating videos to play on RGB pixel displays


Building
-------------------------

The app requires some external addons. Install them in the addons folder

- [ofxXmlSettings] (included with OF)
- [ofxSimpleGuiToo]
- [ofxMSAInteractiveObject]

When these have been installed the app should compile


Usage
-------------------------

Press O or 'Load video'. Adjust the target width and height of the video + adjust the speed if necessary (acts as a mutiplier to the orgiginal video FPS). Press 's' to start saving video. Note that the file name must be in DOS format: 8.3, AAAAAAAA.BBB


File format
-------------------------

The generated file format structure contains a file header with some video metadata and then the raw video data. The emphasis has been to make the video as easy as possible to read without compression etc

Endianess is LSB

<table>
	<tr>
		<th>Field</th>
		<th>Size (bytes)</th>
		<th>Descrption</th>
	</tr>
	<tr>
		<td>PXL </td>
		<td>4</td>
		<td>File format indicator, always equals 0x50584C20 (PXL )</td>
	</tr>
	<tr>
		<td>v</td>
		<td>1</td>
		<td>File version indicator</td>
	</tr>
	<tr>
		<td>1</td>
		<td>1</td>
		<td>File version</td>
	</tr>
	<tr>
		<td>Width</td>
		<td>1</td>
		<td>Video width</td>
	</tr>
	<tr>
		<td>Height</td>
		<td>1</td>
		<td>Video height</td>
	</tr>
	<tr>
		<td>Number or frames</td>
		<td>2</td>
		<td>Total number of frames in video</td>
	</tr>
	<tr>
		<td>Layout</td>
		<td>1</td>
		<td>Generate LED layout/wiring</td>
	</tr>
	<tr>
		<td>Frame rate</td>
		<td>1</td>
		<td>Frame rate (original framerate * multiplier)</td>
	</tr>
	<tr><td colspan="3">Data</td></tr>
	<tr>
		<td>Red</td>
		<td>1</td>
		<td>First LED red component</td>
	</tr>
	<tr>
		<td>Green</td>
		<td>1</td>
		<td>First LED greeb component</td>
	</tr>
	<tr>
		<td>Blue</td>
		<td>1</td>
		<td>First LED blue component</td>
	</tr>
	<tr>
		<td>Red</td>
		<td>1</td>
		<td>Second LED red component</td>
	</tr>
	<tr>
		<td>Green</td>
		<td>1</td>
		<td>Second LED green component</td>
	</tr>
	<tr>
		<td>Blue</td>
		<td>1</td>
		<td>Second LED blue component</td>
	</tr>
	<tr>
		<td colspan="3">...</td>
	</tr>
</table>


[ofxXmlSettings]: http://www.openframeworks.cc/documentation/ofxXmlSettings/ofxXmlSettings.html
[ofxSimpleGuiToo]: https://github.com/memo/ofxSimpleGuiToo
[ofxMSAInteractiveObject]: https://github.com/memo/ofxMSAInteractiveObject
