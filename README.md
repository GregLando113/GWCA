# GWCA++ #

Another C++ API to interface with the Guild Wars client.

by GWCA++ team (4D 1,gigi,reduf) initially for use in GWToolbox++

### Credits to: ###

ACB,Harboe - For their work in the original GWCA project, most agentarray and ctogs functions are derived from this work

ACB,_rusty,miracle44 - GWLP:R work with StoC packets, StoC handler derived form the GWLP Dumper

Miracle444,TheArkanaProject - Work done in the GWA2 Project.


## Usage ##

### Including into project ###

Start up a project in visual studio and init a git repo inside for it. Then use:

```
git submodule add https://github.com/GregLando113/GWCA
```

To clone the repo. From here, include the project (.vcxproj) in your solution using Add->Existing project, add a dependency to your main project, And add the project as a reference in your main project. Now when your main project compiles, GWCA++ will compile into it. From here just include "APIMain.h" into your main file.

### Using in code ###

You must always start with calling the GWCA::Initialize() function, this function is what scans memory and places hooks, creates objects, etc. It will return a boolean on if the Initialize with sucessful.

Once this has been done, create a local GWCA object in whatever function you are accessing GWCA from. **Do NOT make the GWCA object a class member, global variable, or allocate the object on heap (new operator).** With normal usage, creating this object will pause execution until the calling thread can obtain "api ownership" then it will proceed. For that functions execution, the thread will be the only one accessing the api under normal circumstances to avoid concurrency issues.

From there you can retrieve different submodules such as Agents,Items,Skillbar,Effects,Map,etc. Using the -> operator on the GWCA object.
