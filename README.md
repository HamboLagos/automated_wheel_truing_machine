# Automated Bicycle Wheel Truing Stand
## Hamilton Little
hamilton.little@gmail.com

#### Project Overview

Proposed project for ME405 - Mechatronics class at Cal Poly, San Luis Obispo,
Winter Quarter 2013. Objective of the project is to design, build, and test 
prototype automated bicycle wheel truing stand. The prototype was succesfully
built and tested, automating the process of truing a bicycle wheel, and capable
of measuring wheel descrepancies to a professional cyclist's standards.

#### Code Overview

To begin, a detailed account of the project is given in the .pdf file located in
the project report folder.

The source files listed in this repository represent the code developed
specifically for this project. The code was developed in C++ for an Atmel MEGA
series chip, on a custom deveopment board. The board included RF functionality
(unused) as well as two H-Bridge motor drivers. The code uses freeRTOS to
schedule the various tasks of the machine; each task's source code is preceeded
by _task_, and then the name of the task. The code in /lib was developed by Dr.
John Ridgley, the course's instructor, and was instrumental in reducing the
development process time, allowing the project to be completed in only 5 weeks.
