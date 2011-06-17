///////////////////////
//	VMATH Demo Readme
///////////////////////

* The sample code presented here refer to the article
"Designing Fast Cross-Platform SIMD Vector Libraries"

* The binaries for each version are calle "VMathDemoIntelRELEASE.exe" and "VMathDemoMicrosoftRELEASE.exe"

* The code is not clean since it's a modification of one of D3D9 samples (CustomUI) and uses
namespaces and repetitions heavily to be able to test all libaries together.
I had almost no time to write :(

* To compile with Intel you must download their compiler from:
http://software.intel.com/en-us/intel-compilers/ 

* I copied portions of XNAMath inside the code since it wouldn't compile with the
Intel compiler and would conflict with VMath declarations. 

* There is a slider to the bottom of the screen saying "Calculation Repetitions". You can
adjust the amount of calculations forcing the CPU to spend most of the time on SIMD/FPU operations

* Use the buttons "Audio Demo" and "Cloth Demo" to see the different demos.

* The radio buttons saying "VMath", "XNAMath", "VClass" and "FPU" switch between the current
math library used by the current demo.

* The button "Toggle Auto Test" will cycle through the various libraries used on the calculations.
Notice that is the same demo, but using differente math libraries

* The full VMath Library is not available for download since it's proprietary code.
However the core technology used in VMath can be found on "vmath.h" and explained in the article

* On the audio demo press the "Play" button to start the demo and hear some cool guitar :)


Gustavo M. Oliveira
info@guitarv.com


