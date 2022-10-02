#!/usr/local/bin/python3
import os

print("Status: 200\r\nContent-Type: text/html\r\n\r\n<font size=+10>Environment</font><br>")

for param in os.environ.keys():
    print("<b>%20s</b>: %s<br>" % (param, os.environ[param]))