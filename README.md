# Opening garage gate using http request
### Here is a photo of the PCB with all the components:

<p align="center">
  <img src="https://user-images.githubusercontent.com/91852227/160214054-e8bdfa3a-4759-4913-baae-5f40c5f5d374.jpeg" width="400" />
  <img src="https://user-images.githubusercontent.com/91852227/160235305-a313f449-fc87-4af5-ac50-d9d225e946a5.jpg" width="400" /> 
</p>

In the picture you can see:

- WIFI Wemos D1 mini
- Voltage Converter MT3608
- Remote control to remotely open gates




### Here is a photo of the device hidden in 3D printed custom case, in which it is mounted on the ceiling and protected from dust:

<p align="center">
  <img 
    width="400"
    src="https://user-images.githubusercontent.com/91852227/160234905-a8faa70e-ab95-402a-9351-dccefb0b5ffa.jpeg"
  >
</p>

## Basics of the project

Main goal of this project was opening garage gate using phone, so that I could do this from anywhere in the world. I don't have to carry my remote pilot with me anymore.
Connecting to the device is possible thanks to connecting to a host website that stores IP of the device and passes the requests.
Depending on what page you are trying to acces, diffrent thing may happen.

### Opening gates is done when accessing page as follows:

    host_name.hopto.otg:port/open_gate?auth=xxxxxxxxxx&open_gate=both


Depending on argument 'open_gate' (which can be: front, garage, both) corresponging gate will be opend.
