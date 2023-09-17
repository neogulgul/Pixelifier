# Pixelifier - Pixelate images
## Command line arguments
### -i path/to/file
Use to specify image file.
### -c path/to/file
Use to specify colors file.
### -r width:height
Use to specify resolution. Both width and height need to be between 1 and 2048. The resolution won't override the dimensions of the source image.
### -s
Save pixelated image.
### -S
Save pixelated_colored image.
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
### ./Pixelifier -i example/fishin.jpg -c example/colors.txt -r 128:128
Loads an example image and some colors with a resolution of 128x128.
### ./Pixelifier -i example/fishin.jpg -c example/colors.txt -r 128:128 -s -S -headless
Does the same as the previous command, but runs it headless and saves the image both as pixelated and pixelated_colored.
## Notes
### Image file
From the SFML 2.6.0 wiki:<br>
"The supported image formats are bmp, png, tga, jpg, gif, psd, hdr, pic and pnm. Some format options are not supported, like jpeg with arithmetic coding or ASCII pnm."
### Colors file
The colors file is just a text file where each line represents a hex color.
### Saves and screenshots
Files are saved with a filename of the current timestamp in a directory called "saves" which will be created (with force) if it doesn't exist. The same goes for screenshots, but in a directory called "screenshots".
### Bugs
Bugs might occur.
