# Send Whatsapp to Display

:hammer_and_wrench: Still under development :heavy_exclamation_mark:

![blank screen](images/blank_screen.png)

Send a Whatsapp message to twilio and it will be displayed on the led screen. The led matrix also shows the time alternatingly.

## :rocket: Set up

- Open Wifi settings and scan for ESP32AP and connect.
- The Password is: password
- Open browser and type 192.168.4.1 in the search bar.
- Configure Wifi
- Select your Wifi AP and enter your Password.
- If it successfully connected it will show the welcome message and time.

### :page_facing_up: Send Whatsapp message
- Open Whatsapp and send `join soap-completely` to twilio. This is neccessary because we're still using the developer service. Every 3 days we need to send the message again to activate twilio.
- Now send any message with latin characters to the display.

### Advanced
- Try sending `help` it list a the avialable commands.
- Wtih `off` you can turn the display off. Just send a new message and it will be on again.

![finish](images/finish.png)

![demo](images/demo.gif)

