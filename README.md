# dmc-stack

Web Stack for C and C++ developers


**Include:**

* Docker
* MongoDB
* [Crow](https://github.com/ipkn/crow)


### Important

Note: (1) The following software is distributed for educational purposes only, and by no means is ready for production!

Note: (2) The following software was tested on various Debian-based distributions of Linux(incl. Ubuntu), and some of them require to run docker commands as <code>sudo</code>. No guarantees in Win or Mac workability! 

Requirments: cmake, gcc, docker ce

Prerequisites: Knowledge of basic back end development, prior experience with docker is recommended

Best way to get docker is docker's official [site](https://docs.docker.com/install/)

Easiest way to get g++ on Ubuntu or Debian (most of the time) is the following command

```
sudo apt-get install build-essential
```

One way to get cmake is cmake's official [site](https://cmake.org/download/)


### Docs (beta 0.0.1)

#### Getting Started

1 Make sure you have all the requirments satisfied prior to continue

2 Build playbox image (from playbox folder)

```
docker build -t playbox .
```

3 Now you can launch your stack (from server-source folder)

```
docker-compose up 
```

**OR** if you need to rebuild image after initial build

```
docker-compose up 
```

This will run Crow on port <code>8080</code>, and Mongo on <code>27017</code>


#### Common issues

Error: <code>docker-compose up</code> generates the following error:

```
Starting mongo ... error

ERROR: for mongo  Cannot start service mongo: driver failed programming external connectivity on endpoint mongo (ff8a6f574a0991ee95e34c9d460f0f76b8e78341ccb803fca375b8014a188c47): Error starting userland proxy: listen tcp 0.0.0.0:27017: bind: address already in use
```

Solution: Mongo is already running on port 27017, and you simply need to stop it

```
sudo service mongodb stop
```