# Pixelifier - Pixelate images
## Command line arguments
### -i &lt;path/to/file&gt;
Use to specify image file.
### -c &lt;path/to/file&gt;
Use to specify colors file.
### -r &lt;width&gt;x&lt;height&gt; || source
Use to specify resolution. Both width and height need to be between 1 and 2048. The resolution won't override the dimensions of the source image. If "source" is specified the source resolution will be used.
### -s
Save Pixelated image.
### -S
Save Pixelated_Colored image.
### -n
Keep the name of the source image when saving.
### -t
Print out the time it takes for certain operations during setup.
### -headless
Use to run the program headless (without a GUI).
###
## Key bindings
### 1
Switch to "Normal" mode.
### 2
Switch to "Pixelated" mode.
### 3
Switch to "Pixelated_Colored" mode.
### F1
Toggle debug mode.
### F12
Take screenshot.
## Examples
### ./Pixelifier -i "example/fishin.jpg" -c "example/ocean_sunset.txt" -r 128x128
Loads an image and some colors with an output resolution of 128x128.
### ./Pixelifier -i "example/serenity.jpg" -c "example/peanut_butter.txt" -r source
This time the resolution is the same as the source.
### ./Pixelifier -i "example/commute.jpg" -c "example/forest_magic.txt" -r 160x90 -s -S -n -t -headless
Now the program runs headless, meaning no window will be created. The program creates and saves the source image both as Pixelated and Pixelated_Colored, saving with the filename of the source image plus the mode used. It also prints out the time it takes for each major operation to execute during setup.
## Notes
### Image file
From the SFML 2.6.0 wiki:<br>
"The supported image formats are bmp, png, tga, jpg, gif, psd, hdr, pic and pnm. Some format options are not supported, like jpeg with arithmetic coding or ASCII pnm."
### Colors file
The colors file is just a text file where each line represents a hex color. A line needs to start with a '#' and contain exactly 6 additional characters to be considered as a valid color.<br>
### Saves and screenshots
Saves are saved in a directory called "saves" which will be created (with force) if it doesn't exist. The same goes for screenshots, but in a directory called "screenshots".
### Bugs
There might be some bugs.
## About
Made for fun with
<img src="https://www.sfml-dev.org/download/goodies/sfml-logo-small.png" height=15>
.
