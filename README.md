# BadUSB-Detection
Proof of Concept Bad USB detection code.

BadUSB is an attack method used to gain code execution on systems (particularly "air-gapped" networks).

As computers inherently trust humans, BadUSB emulates a Human Input Device (HID) and automates the human input to cause malicious activity to occure.  

The most common method of BadUSB is to emulate a keyboard and use it to automatically input keystrokes, particularly to type out and run a script (e.g. powershell).  This cannot be prevented without impacting human experience (i.e. denying legitimate inputs from keyboards) and the attack method is very accessible (RubberDucky is available cheaply from Hak5).

Most attempts to prevent BadUSB uses VID and PID white listing to ensure only authorised devices can generate input.  This actually has no affect at all on targeted BadUSB attacks as it is trivial to change the VID and PID of the USB to match the authorised keyboard.  I actually tested this with RubberDucky on a system using one of these products to prove the simplicity of it.  Needless to say the test was successful.

This proof of concept intercepts all keystrokes from any attached keyboards and checks the rate at which a person is typing.  Brief tests and statistics from wikipedia indicate that moderate-fast human type speed is 1 keystroke every 100 milliseconds.  

BadUSB is not limited by human type speed and can be as fast as the attacker likes.  The slower BadUSB types, the more noticeable it is to the unsuspecting user that plugged in the device, it is therefore in the attackers interest to make the keystrokes as quick as possible which then differentiates the behaviour from normal user activity.

A small use of BadUSB (the example from RubberDucky) takes over 20 key strokes to open notepad and type "Hello World!", with around 30 milliseconds between keystrokes.  Larger applications take many more keystrokes, for example, to use powershell to download a file from the Internet and run it, took 324 keystrokes at a rate of 2 milliseconds per stroke and there was still a short flash of the powershell prompt visible to the user.

While BadUSB isn't a very good attack vector if the attack doesn't want to get caught, it can still be used by an Insider (i.e. paid or volunteers to plug in the USB) and as of yet can't be mitigated.  This proof of concept monitors keystroke rate and prints a message when the key rate is less than 35 milliseconds between strokes.

There is a flaw in this method of detection in that a user sitting on the keyboard (or bashing it) will trigger it, and it is true an attacker can slow down keystrokes but that isn't in the attackers interest, particularly with a large payload such as would be needed against an air-gapped network.

Future development
----------------------------------------------
- get the alert printed to event log (much better)
- setup up some sort of autostart/install mechanism
- make the keystroke rate a variable so users can customise
- check the key strokes are realtively unique (overcome bum->keyboard issues)
