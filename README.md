# SystemsProject

Building my first website using fundamentals of network programming in C extending the CMU TINY web server. 

Main files are the netp/tiny/cgi-bin/megillah.c which is the cgi executable program, as well as netp/tiny/home.html which is the home page. 

With thanks to the existing CMU code, the open-source JSMN JSON parser module, and W3 Schools HTML. 

To run:

cd ~/csproj/netp/tiny
<br>./tiny 6133 (port must be 6133 unless you modify the links in home.html for each of the megillah buttons)

Access on: http://ada.sterncs.net:6133/home.html
<br>To access a specific megillah: http://ada.sterncs.net:6133/cgi-bin/megillah?[[validMegillah]]

