# Opening garage gate using moble device
### Here is a photo of the PCB with all the components:

<p align="center">
  <img src="https://user-images.githubusercontent.com/91852227/160214054-e8bdfa3a-4759-4913-baae-5f40c5f5d374.jpeg" width="400" />
  <img src="https://user-images.githubusercontent.com/91852227/160235305-a313f449-fc87-4af5-ac50-d9d225e946a5.jpg" width="400" /> 
</p>

In the picture you can see:

- WIFI Wemos D1 mini
- Voltage Converter MT3608*
- Remote control to remotely open gates




### Here is a photo of the device inserted in 3D printed custom case, in which it is mounted on the ceiling and protected from dust:

<p align="center">
  <img 
    width="400"
    src="https://user-images.githubusercontent.com/91852227/160234905-a8faa70e-ab95-402a-9351-dccefb0b5ffa.jpeg"
  >
</p>

## Basics of the project

Main goal of this project was opening garage gate using phone, so that I could do this from anywhere in the world. I don't have to carry my remote pilot with me anymore.
Connecting to the device is possible thanks to connecting to a host website that stores IP of the device and passes the requests.
Depending on what page you are trying to acces, diffrent things may happen.

### Opening gates is done when accessing page as follows:

    host_name.hopto.org:port/open_gate?auth=xxxxxxxxxx&open_gate=both


Depending on argument 'open_gate' (which can be: front, garage, both) corresponging gate will be opened.

### Opening gates using voice assistant
One of the best features of this device is the fact that you can make your voice assistant open the gate for you!
You can just simply say:

    Hey Siri, open both gates.

After this command, your phone using "Shortcuts" application will send http request to a specified web page.

### Info on the device

If you access a info link, you can get short info on how the device operates:

    Turned on: 6-2-2022,  Sunday,   20:05:02
    Compiled on: Jun 24 2021 21:45:09
    Last front gate opening:  26-3-2022,  Saturday,   14:18:04
    Last garage gate opening: 26-3-2022,  Saturday,   12:58:23
    Front gate openings: 735
    Garage gate openings: 109

Number of front gate openings is way bigger than number of garage gate openings, becuse majority of my neighbours who use this device park their cars outside.


### Upload code remotely

The code also provides the opportunity to compile and run new code over WIFI. This feature was implemented because the device is in the garage and I didn't want to go down there every time I wanted to add some corrects.


*Voltage Converter is needed, because D1 mini is powered by 5 V and remote control is powered by 12 V.
